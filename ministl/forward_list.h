#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include "allocator.h"
#include "iterator.h"
#include "type_traits.h"

namespace ministl
{
	/// ForwardListNodeBase
	///
	/// The ForwardListNodeBase is a standalone struct so that operations
	/// on a forward list can be done without templates. An empty forward
	/// list can have a ForwardListNodeBase and not create any instance of
	/// T
	struct ForwardListNodeBase
	{
		ForwardListNodeBase *mpNext;
	};

	/// ForwardListNode
	///
	template <typename T>
	struct ForwardListNode: public ForwardListNodeBase
	{
		T mValue;
	};


	/// ForwardListIterator
	///
	template <typename T, typename Pointer, typename Reference>
	struct ForwardListIterator
	{
		typedef ptrdiff_t                                   difference_type;
		typedef T                                           value_type;
		typedef Pointer                                     pointer;
		typedef Reference                                   reference;
		typedef forward_iterator_tag                        iterator_category;
		typedef ForwardListNode<T>                          node_type;
		typedef ForwardListIterator<T, T*, T&>              iterator;
		typedef ForwardListIterator<T, const T*, const T&>  const_iterator;
		typedef ForwardListIterator<T, Pointer, Reference>  this_type;

	public:
		ForwardListIterator();
		ForwardListIterator(const ForwardListNodeBase *p);
		ForwardListIterator(const iterator &it);

		reference   operator*()const;
		pointer     operator->()const;

		this_type&  operator++();
		this_type   operator++(int);

	public:
		node_type *mpNode;
	};

	/// ForwardListBase
	///
	/// See VectorBase (class vector) for an explanation of why I create
	/// this separate base class
	template <typename T, typename Allocator>
	class ForwardListBase
	{
	public:
		typedef ptrdiff_t           difference_type;
		typedef size_t              size_type;
		typedef ForwardListNodeBase base_node_type;
		typedef ForwardListNode<T>  node_type;
		typedef Allocator           allocator_type;

	protected:
		base_node_type mNode;
		allocator_type mAllocator;

	public:
		const allocator_type &get_allocator()const;
		allocator_type       &get_allocator();
		void                  set_allocator(const allocator_type &alloc);

	protected:
		ForwardListBase();
		ForwardListBase(const allocator_type &alloc);
		~ForwardListBase();

		node_type *AllocateNode();
		void       FreeNode(node_type *pNode);
		ForwardListNodeBase *EraseAfter(ForwardListNodeBase *pNode);
		ForwardListNodeBase *EraseAfter(ForwardListNodeBase *pNode, ForwardListNodeBase *pLast);
	};

	template <typename T, typename Allocator = alloc>
	class forward_list: public ForwardListBase<T, Allocator>
	{
		typedef ForwardListBase<T, Allocator>               base_type;
		typedef forward_list<T, Allocator>                  this_type;

	public:
		typedef T                                           value_type;
		typedef T*                                          pointer;
		typedef const T*                                    const_pointer;
		typedef T&                                          reference;
		typedef const T&                                    const_reference;
		typedef ForwardListIterator<T, T*, T&>              iterator;
		typedef ForwardListIterator<T, const T*, const T&>  const_iterator;
		typedef ministl::reverse_iterator<iterator>         reverse_iterator;
		typedef ministl::reverse_iterator<const_iterator>   const_reverse_iterator;
		typedef typename base_type::size_type               size_type;
		typedef typename base_type::difference_type         difference_type;
		typedef typename base_type::allocator_type          allocator_type;
		typedef typename base_type::node_type               node_type;
		typedef typename base_type::base_node_type          base_node_type;

		using base_type::mNode;
		using base_type::mAllocator;
		using base_type::AllocateNode;
		using base_type::FreeNode;
		using base_type::get_allocator;
		using base_type::EraseAfter;

	public:
		forward_list();
		forward_list(const allocator_type &alloc);
		explicit forward_list(size_type n, const allocator_type &alloc = allocator_type());
		forward_list(size_type n, const value_type &value, const allocator_type &alloc = allocator_type());
		forward_list(const this_type &other);
		forward_list(const this_type &other, const allocator_type &alloc);
		forward_list(forward_list &&other);
		forward_list(std::initializer_list<T> init, const allocator_type &alloc = allocator_type());
		template <typename InputIterator>
		forward_list(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type());

		this_type &operator=(const this_type &other);
		this_type &operator=(this_type &&other);
		this_type &operator=(std::initializer_list<T> init);

		void assign(size_type count, const T &value);
		template <typename InputIterator>
		void assign(InputIterator first, InputIterator last);
		void assign(std::initializer_list<T> ilist);

		reference        front();
		const_reference  front()const;

		iterator       before_begin();
		const_iterator before_begin()const;
		const_iterator cbefore_begin()const;

		iterator       begin();
		const_iterator begin()const;
		const_iterator cbegin()const;

		iterator       end();
		const_iterator end()const;
		const_iterator cend()const;

		bool empty()const;
		size_type max_size()const;

		void clear();
		iterator insert_after(const_iterator pos, const T &value);
		iterator insert_after(const_iterator pos, T&& value);
		iterator insert_after(const_iterator pos, size_type count, const T &value);
		template <typename InputIterator>
		iterator insert_after(const_iterator pos, InputIterator first, InputIterator last);
		iterator insert_after(const_iterator pos, std::initializer_list<T> ilist);
		iterator erase_after(const_iterator pos);
		iterator erase_after(const_iterator first, const_iterator last);
		void push_front(const T &value);
		void push_front(T &&value);
		void pop_front();
		void resize(size_type n);
		void resize(size_type n, const T &value);
		void swap(this_type &other);

		void merge(this_type &other);
		template <typename Compare>
		void merge(this_type &other, Compare comp);
		void splice_after(const_iterator pos, this_type &other);
		void splice_after(const_iterator pos, this_type &other, const_iterator it);
		void splice_after(const_iterator pos, this_type &other, const_iterator first, const_iterator last);
		void remove(const T &value);
		template <typename UnaryPredicate>
		void remove_if(UnaryPredicate p);
		void reverse();
		void unique();
		template <typename BinaryPredicate>
		void unique(BinaryPredicate p);
		void sort();
		template <typename Compare>
		void sort(Compare comp);

	protected:
		node_type *CreateNode(const T &value);
		node_type *CreateNode(T &&value);
		template <typename...Args>
		node_type *CreateNode(Args&&...args);

		node_type *InsertValueAfter(ForwardListNodeBase *p, const T &value);
		node_type *InsertValuesAfter(ForwardListNodeBase *p, size_type count, const T &value);
		template <typename Integer>
		node_type *InsertAfter(ForwardListNodeBase *node, Integer n, Integer value, true_type);
		template <typename InputIterator>
		node_type *InsertAfter(ForwardListNodeBase *node, InputIterator first, InputIterator last, false_type);

		void AssignValues(size_type count, const T &value);
		template <typename Integer>
		void Assign(Integer n, Integer value, true_type);
		template <typename InputIterator>
		void Assign(InputIterator first, InputIterator last, false_type);
	};


	///////////////////////////////////////////////////////////////////////
	/// ForwardListNodeBase
	///////////////////////////////////////////////////////////////////////

	/// This is some helper functions for manipulating forward list nodes.
	/// Operations on forward list such as insert, splice, reverse, etc
	/// has nothing to do with the instance (mValue) in their nodes.

	// insert the node pointed by pNode into the postion following pPrev
	inline ForwardListNodeBase*
	ForwardListInsertAfter(ForwardListNodeBase *pPrev, ForwardListNodeBase *pNode)
	{
		pNode->mpNext = pPrev->mpNext;
		pPrev->mpNext = pNode;
		return pNode;
	}

	inline ForwardListNodeBase*
	ForwardListGetPrevious(ForwardListNodeBase *pBegin, ForwardListNodeBase *pNode)
	{
		while (pBegin && pBegin->mpNext != pNode)
			pBegin = pBegin->mpNext;
		return pBegin;
	}

	inline ForwardListNodeBase*
	ForwardListReverse(ForwardListNodeBase *pNode)
	{
		ForwardListNodeBase *pNext = pNode->mpNext;
		pNode->mpNext = nullptr;

		while (pNext)
		{
			ForwardListNodeBase *pTemp = pNext->mpNext;
			pNext->mpNext = pNode;
			pNode = pNext;
			pNext = pTemp;
		}

		return pNode;
	}

	// insert all elements in the range (pBeforeBegin, pLast) after the element pointed
	// by pNode. Note that only non-empty ranges are considered, and this operation has
	// no effect if pNode = pBeforeBegin
	inline void
	ForwardListSpliceAfter(ForwardListNodeBase *pNode, ForwardListNodeBase *pBeforeFirst, ForwardListNodeBase *pLast)
	{
		if (pNode != pBeforeFirst && pBeforeFirst != pLast && pBeforeFirst->mpNext != pLast)
		{
			ForwardListNodeBase *pPrevLast = ForwardListGetPrevious(pBeforeFirst, pLast);

			ForwardListNodeBase *pFirstNext = pBeforeFirst->mpNext;
			pBeforeFirst->mpNext = pLast;

			pPrevLast->mpNext = pNode->mpNext;
			pNode->mpNext = pFirstNext;
		}
	}

	inline void
	ForwardListSpliceAfter(ForwardListNodeBase *pNode, ForwardListNodeBase *pNodeBase)
	{
		if (pNode != pNodeBase && pNodeBase->mpNext != pNode)
		{
			ForwardListNodeBase *pNext = pNodeBase->mpNext;
			pNodeBase->mpNext = pNext->mpNext;
			pNext->mpNext = pNode->mpNext;
			pNode->mpNext = pNext;
		}
	}

	inline size_t ForwardListGetSize(ForwardListNodeBase *pNode)
	{
		size_t n = 0;
		for (; pNode; pNode = pNode->mpNext)
			++n;
		return n;
	}

	///////////////////////////////////////////////////////////////////////
	/// ForwardListIterator
	///////////////////////////////////////////////////////////////////////

	template <typename T, typename Pointer, typename Reference>
	ForwardListIterator<T, Pointer, Reference>::ForwardListIterator()
		: mpNode(nullptr)
	{
		// empty
	}

	template <typename T, typename Pointer, typename Reference>
	ForwardListIterator<T, Pointer, Reference>::ForwardListIterator(const ForwardListNodeBase *p)
		: mpNode((node_type*)const_cast<ForwardListNodeBase*>(p))
	{
		// empty
	}

	template <typename T, typename Pointer, typename Reference>
	ForwardListIterator<T, Pointer, Reference>::ForwardListIterator(const iterator &it)
		: mpNode(it.mpNode)
	{
		// empty
	}

	template <typename T, typename Pointer, typename Reference>
	typename ForwardListIterator<T, Pointer, Reference>::reference
	ForwardListIterator<T, Pointer, Reference>::operator*()const
	{
		return mpNode->mValue;
	}

	template <typename T, typename Pointer, typename Reference>
	typename ForwardListIterator<T, Pointer, Reference>::pointer
	ForwardListIterator<T, Pointer, Reference>::operator->()const
	{
		return &mpNode->mValue;
	}

	template <typename T, typename Pointer, typename Reference>
	typename ForwardListIterator<T, Pointer, Reference>::this_type&
	ForwardListIterator<T, Pointer, Reference>::operator++()
	{
		mpNode = static_cast<node_type*>(mpNode->mpNext);
		return *this;
	}

	template <typename T, typename Pointer, typename Reference>
	typename ForwardListIterator<T, Pointer, Reference>::this_type
	ForwardListIterator<T, Pointer, Reference>::operator++(int)
	{
		this_type temp = *this;
		++*this;
		return temp;
	}

	template <typename T, typename PointerA, typename ReferenceA, typename PointerB, typename ReferenceB>
	bool operator==(const ForwardListIterator<T, PointerA, ReferenceA> &lhs,
	                const ForwardListIterator<T, PointerB, ReferenceB> &rhs)
	{
		return lhs.mpNode == rhs.mpNode;
	}

	template <typename T, typename PointerA, typename ReferenceA, typename PointerB, typename ReferenceB>
	bool operator!=(const ForwardListIterator<T, PointerA, ReferenceA> &lhs,
	                const ForwardListIterator<T, PointerB, ReferenceB> &rhs)
	{
		return lhs.mpNode != rhs.mpNode;
	}


	///////////////////////////////////////////////////////////////////////
	/// ForwardListBase
	///////////////////////////////////////////////////////////////////////

	template <typename T, typename Allocator>
	ForwardListBase<T, Allocator>::ForwardListBase()
		: mNode(),
		  mAllocator()
	{

	}

	template <typename T, typename Allocator>
	ForwardListBase<T, Allocator>::ForwardListBase(const allocator_type &alloc)
		: mNode(),
		  mAllocator(alloc)
	{

	}

	template <typename T, typename Allocator>
	ForwardListBase<T, Allocator>::~ForwardListBase()
	{
		EraseAfter(&mNode, nullptr);
	}

	template <typename T, typename Allocator>
	inline const typename ForwardListBase<T, Allocator>::allocator_type&
	ForwardListBase<T, Allocator>::get_allocator()const
	{
		return mAllocator;
	}

	template <typename T, typename Allocator>
	inline typename ForwardListBase<T, Allocator>::allocator_type&
	ForwardListBase<T, Allocator>::get_allocator()
	{
		return mAllocator;
	}

	template <typename T, typename Allocator>
	void ForwardListBase<T, Allocator>::set_allocator(const allocator_type &alloc)
	{
		mAllocator = alloc;
	}

	template <typename T, typename Allocator>
	inline typename ForwardListBase<T, Allocator>::node_type*
	ForwardListBase<T, Allocator>::AllocateNode()
	{
		return (node_type*)allocate_memory(mAllocator, sizeof(node_type));
	}


	template <typename T, typename Allocator>
	inline void ForwardListBase<T, Allocator>::FreeNode(node_type *pNode)
	{
		MINISTLFree(mAllocator, pNode, sizeof(node_type));
	}

	template <typename T, typename Allocator>
	inline ForwardListNodeBase*
	ForwardListBase<T, Allocator>::EraseAfter(ForwardListNodeBase *pNode)
	{
		node_type *pNext = static_cast<node_type*>(pNode->mpNext);
		pNode->mpNext = pNext->mpNext;
		pNext->~node_type();
		FreeNode(pNext);
		return pNode->mpNext;
	}

	template <typename T, typename Allocator>
	inline ForwardListNodeBase*
	ForwardListBase<T, Allocator>::EraseAfter(ForwardListNodeBase *pNode, ForwardListNodeBase *pLast)
	{
		if (pNode != pLast && pNode->mpNext != pLast)
		{
			while (pNode->mpNext != pLast)
				EraseAfter(pNode);
		}
		return pLast;
	}


	///////////////////////////////////////////////////////////////////////
	/// forward_list
	///////////////////////////////////////////////////////////////////////

	template <typename T, typename Allocator>
	forward_list<T, Allocator>::forward_list()
		: base_type()
	{
		// empty
	}

	template <typename T, typename Allocator>
	forward_list<T, Allocator>::forward_list(const allocator_type &alloc)
		: base_type(alloc)
	{
		// empty
	}

	template <typename T, typename Allocator>
	forward_list<T, Allocator>::forward_list(size_type n, const allocator_type &alloc)
		: base_type(alloc)
	{
		InsertValuesAfter(&mNode, n, value_type());
	}

	template <typename T, typename Allocator>
	forward_list<T, Allocator>::forward_list(size_type n, const value_type &value, const allocator_type &alloc)
		: base_type(alloc)
	{
		InsertValuesAfter(&mNode, n, value);
	}

	template <typename T, typename Allocator>
	forward_list<T, Allocator>::forward_list(const this_type &other)
		: base_type(other.mAllocator)
	{
		InsertAfter(&mNode, other.begin(), other.end(), false_type());
	}

	template <typename T, typename Allocator>
	forward_list<T, Allocator>::forward_list(const this_type &other, const allocator_type &alloc)
		: base_type(alloc)
	{
		InsertAfter(&mNode, other.begin(), other.end(), false_type());
	}

	template <typename T, typename Allocator>
	template <typename InputIterator>
	forward_list<T, Allocator>::forward_list(InputIterator first, InputIterator last, const allocator_type &alloc)
		: base_type(alloc)
	{
		InsertAfter(&mNode, first, last, is_integral<InputIterator>());
	}

	template <typename T, typename Allocator>
	forward_list<T, Allocator>::forward_list(std::initializer_list<T> ilist, const allocator_type &alloc)
		: base_type(alloc)
	{
		InsertAfter(&mNode, ilist.begin(), ilist.end(), false_type());
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::this_type&
	forward_list<T, Allocator>::operator=(const this_type &other)
	{
		if (this != &other)
			Assign(other.begin(), other.end(), false_type());
		return *this;
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::this_type&
	forward_list<T, Allocator>::operator=(std::initializer_list<T> ilist)
	{
		Assign(ilist.begin(), ilist.end(), false_type());
		return *this;
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::assign(size_type n, const T &value)
	{
		AssignValues(n, value);
	}

	template <typename T, typename Allocator>
	template <typename InputIterator>
	void forward_list<T, Allocator>::assign(InputIterator first, InputIterator last)
	{
		Assign(first, last, is_integral<InputIterator>());
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::assign(std::initializer_list<T> ilist)
	{
		Assign(ilist.begin(), ilist.end(), false_type());
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::reference
	forward_list<T, Allocator>::front()
	{
		return static_cast<node_type*>(mNode.mpNext)->mValue;
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::const_reference
	forward_list<T, Allocator>::front()const
	{
		return static_cast<node_type*>(mNode.mpNext)->mValue;
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::iterator
	forward_list<T, Allocator>::before_begin()
	{
		return iterator(&mNode);
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::const_iterator
	forward_list<T, Allocator>::before_begin()const
	{
		return const_iterator(&mNode);
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::const_iterator
	forward_list<T, Allocator>::cbefore_begin()const
	{
		return const_iterator(&mNode);
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::iterator
	forward_list<T, Allocator>::begin()
	{
		return iterator(mNode.mpNext);
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::const_iterator
	forward_list<T, Allocator>::begin()const
	{
		return const_iterator(mNode.mpNext);
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::const_iterator
	forward_list<T, Allocator>::cbegin()const
	{
		return const_iterator(mNode.mpNext);
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::iterator
	forward_list<T, Allocator>::end()
	{
		return iterator(nullptr);
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::const_iterator
	forward_list<T, Allocator>::end()const
	{
		return const_iterator(nullptr);
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::const_iterator
	forward_list<T, Allocator>::cend()const
	{
		return const_iterator(nullptr);
	}

	template <typename T, typename Allocator>
	bool forward_list<T, Allocator>::empty()const
	{
		return mNode.mpNext == nullptr;
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::clear()
	{
		EraseAfter(&mNode, nullptr);
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::iterator
	forward_list<T, Allocator>::insert_after(const_iterator pos, const T &value)
	{
		return iterator(InsertValueAfter(pos.mpNode, value));
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::iterator
	forward_list<T, Allocator>::insert_after(const_iterator pos, T &&value)
	{
		return iterator(InsertValueAfter(pos.mpNode, std::move(value)));
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::iterator
	forward_list<T, Allocator>::insert_after(const_iterator pos, size_type count, const T &value)
	{
		return iterator(InsertValuesAfter(pos.mpNode, count, value));
	}

	template <typename T, typename Allocator>
	template <typename InputIterator>
	typename forward_list<T, Allocator>::iterator
	forward_list<T, Allocator>::insert_after(const_iterator pos, InputIterator first, InputIterator last)
	{
		return iterator(InsertAfter(pos.mpNode, first, last, is_integral<InputIterator>()));
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::iterator
	forward_list<T, Allocator>::insert_after(const_iterator pos, std::initializer_list<T> ilist)
	{
		return iterator(InsertAfter(pos.mpNode, ilist.begin(), ilist.end(), false_type()));
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::iterator
	forward_list<T, Allocator>::erase_after(const_iterator pos)
	{
		return iterator(EraseAfter(pos.mpNode));
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::iterator
	forward_list<T, Allocator>::erase_after(const_iterator first, const_iterator last)
	{
		return iterator(EraseAfter(first.mpNode, last.mpNode));
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::push_front(const T &value)
	{
		InsertValueAfter(&mNode, value);
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::push_front(T &&value)
	{
		InsertValueAfter(&mNode, std::move(value));
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::pop_front()
	{
		EraseAfter(&mNode);
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::resize(size_type n)
	{
		resize(n, value_type());
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::resize(size_type n, const T &value)
	{
		ForwardListNodeBase *pNode = &mNode;
		for (; pNode->mpNext && n > 0; --n, pNode = pNode->mpNext)
			;
		if (n > 0)
			InsertValuesAfter(pNode, n, value);
		else
			EraseAfter(pNode, nullptr);
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::swap(this_type &other)
	{
		using std::swap;
		swap(mNode, other.mNode);
		swap(mAllocator, other.mAllocator);
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::merge(this_type &other)
	{
		ForwardListNodeBase *pBeforeBegin1 = &mNode;
		node_type *pNext1 = static_cast<node_type*>(pBeforeBegin1->mpNext);
		ForwardListNodeBase *pBeforeBegin2 = &other.mNode;
		node_type *pNext2 = static_cast<node_type*>(pBeforeBegin2->mpNext);

		while (pNext1 && pNext2)
		{
			if (pNext2->mValue < pNext1->mValue)
			{
				pBeforeBegin2->mpNext = pNext2->mpNext;
				ForwardListInsertAfter(pBeforeBegin1, pNext2);
				pBeforeBegin1 = pBeforeBegin1->mpNext;
				pNext2 = static_cast<node_type*>(pBeforeBegin2->mpNext);
			}
			else
			{
				pBeforeBegin1 = pBeforeBegin1->mpNext;
				pNext1 = static_cast<node_type*>(pBeforeBegin1->mpNext);
			}
		}

		if (pNext2)
			InsertAfter(pBeforeBegin1, other.begin(), other.end(), false_type());
	}

	template <typename T, typename Allocator>
	template <typename Compare>
	void forward_list<T, Allocator>::merge(this_type &other, Compare comp)
	{
		ForwardListNodeBase *pBeforeBegin1 = &mNode;
		node_type *pNext1 = static_cast<node_type*>(pBeforeBegin1->mpNext);
		ForwardListNodeBase *pBeforeBegin2 = &other.mNode;
		node_type *pNext2 = static_cast<node_type*>(pBeforeBegin2->mpNext);

		while (pNext1 && pNext2)
		{
			if (comp(pNext2->mValue, pNext1->mValue))
			{
				pBeforeBegin2->mpNext = pNext2->mpNext;
				ForwardListInsertAfter(pBeforeBegin1, pNext2);
				pBeforeBegin1 = pBeforeBegin1->mpNext;
				pNext2 = static_cast<node_type*>(pBeforeBegin2->mpNext);
			}
			else
			{
				pBeforeBegin1 = pBeforeBegin1->mpNext;
				pNext1 = static_cast<node_type*>(pBeforeBegin1->mpNext);
			}
		}

		if (pNext2)
			InsertAfter(pBeforeBegin1, other.begin(), other.end(), false_type());
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::splice_after(const_iterator pos, this_type &other)
	{
		// TODO: consider what will happen when mAllocator != other.mAllocator
		if (this != &other)
			ForwardListSpliceAfter(pos.mpNode, &other.mNode, nullptr);
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::splice_after(const_iterator pos, this_type &other, const_iterator it)
	{
		ForwardListSpliceAfter(pos.mpNode, it.mpNode);
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::splice_after(const_iterator pos, this_type &other, const_iterator first, const_iterator last)
	{
		ForwardListSpliceAfter(pos.mpNode, first.mpNode, last.mpNode);
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::remove(const T &value)
	{
		ForwardListNodeBase *pNode = &mNode;

		while (pNode && pNode->mpNext)
		{
			if (static_cast<node_type*>(pNode->mpNext)->mValue == value)
				EraseAfter(pNode);
			else
				pNode = pNode->mpNext;
		}
	}

	template <typename T, typename Allocator>
	template <typename UnaryPredicate>
	void forward_list<T, Allocator>::remove_if(UnaryPredicate p)
	{
		ForwardListNodeBase *pNode = &mNode;

		while (pNode && pNode->mpNext)
		{
			if (p(static_cast<node_type*>(pNode->mpNext)->mValue))
				EraseAfter(pNode);
			else
				pNode = pNode->mpNext;
		}
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::reverse()
	{
		if (mNode.mpNext)
			mNode.mpNext = ForwardListReverse(mNode.mpNext);
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::unique()
	{
		node_type *pNext = static_cast<node_type*>(mNode.mpNext);

		while (pNext && pNext->mpNext)
		{
			if (pNext->mValue == static_cast<node_type*>(pNext->mpNext)->mValue)
				EraseAfter(pNext);
			else
				pNext = static_cast<node_type*>(pNext->mpNext);
		}
	}

	template <typename T, typename Allocator>
	template <typename BinaryPredicate>
	void forward_list<T, Allocator>::unique(BinaryPredicate p)
	{
		node_type *pNext = static_cast<node_type*>(mNode.mpNext);

		while (pNext && pNext->mpNext)
		{
			if (p(pNext->mValue, static_cast<node_type*>(pNext->mpNext)->mValue))
				EraseAfter(pNext);
			else
				pNext = static_cast<node_type*>(pNext->mpNext);
		}
	}


	///////////////////////////////////////////////////////////////////////
	/// helper functions
	///////////////////////////////////////////////////////////////////////

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::node_type*
	forward_list<T, Allocator>::CreateNode(const T &value)
	{
		node_type *pNode = AllocateNode();
		try
		{
			new(&pNode->mValue)value_type(value);
		}
		catch (...)
		{
			FreeNode(pNode);
			throw;
		}
		return pNode;
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::node_type*
	forward_list<T, Allocator>::CreateNode(T &&value)
	{
		node_type *pNode = AllocateNode();
		try
		{
			new(&pNode->mValue)value_type(std::move(value));
		}
		catch (...)
		{
			FreeNode(pNode);
			throw;
		}
		return pNode;
	}

	template <typename T, typename Allocator>
	template <typename...Args>
	typename forward_list<T, Allocator>::node_type*
	forward_list<T, Allocator>::CreateNode(Args&&...args)
	{
		node_type *pNode = AllocateNode();
		try
		{
			new(&pNode->mValue)value_type(std::forward<Args>(args)...);
		}
		catch (...)
		{
			FreeNode(pNode);
			throw;
		}
		return pNode;
	}

	template <typename T, typename Allocator>
	inline typename forward_list<T, Allocator>::node_type*
	forward_list<T, Allocator>::InsertValueAfter(ForwardListNodeBase *pNode, const T &value)
	{
		node_type *pNewNode = CreateNode(value);
		ForwardListInsertAfter(pNode, pNewNode);
		return pNewNode;
	}

	template <typename T, typename Allocator>
	typename forward_list<T, Allocator>::node_type*
	forward_list<T, Allocator>::InsertValuesAfter(ForwardListNodeBase *pNode, size_type n, const T &value)
	{
		for (; n > 0; --n)
			pNode = InsertValueAfter(pNode, value);
		return static_cast<node_type*>(pNode);
	}

	template <typename T, typename Allocator>
	template <typename Integer>
	typename forward_list<T, Allocator>::node_type*
	forward_list<T, Allocator>::InsertAfter(ForwardListNodeBase *pNode, Integer n, Integer value, true_type)
	{
		return InsertValuesAfter(pNode, static_cast<size_type>(n), static_cast<value_type>(value));
	}

	template <typename T, typename Allocator>
	template <typename InputIterator>
	typename forward_list<T, Allocator>::node_type*
	forward_list<T, Allocator>::InsertAfter(ForwardListNodeBase *pNode, InputIterator first, InputIterator last, false_type)
	{
		for (; first != last; ++first)
			pNode = InsertValueAfter(pNode, *first);
		return static_cast<node_type*>(pNode);
	}

	template <typename T, typename Allocator>
	void forward_list<T, Allocator>::AssignValues(size_type n, const T &value)
	{
		ForwardListNodeBase *pNode = &mNode;
		for (; pNode->mpNext && n > 0; --n, pNode = pNode->mpNext)
			static_cast<node_type*>(pNode->mpNext)->mValue = value;
		if (n > 0)
			InsertValuesAfter(pNode, n, value);
		else
			EraseAfter(pNode, nullptr);
	}

	template <typename T, typename Allocator>
	template <typename InputIterator>
	void forward_list<T, Allocator>::Assign(InputIterator first, InputIterator last, false_type)
	{
		ForwardListNodeBase *pNode = &mNode;
		for (; pNode->mpNext && first != last; ++first, pNode = pNode->mpNext)
			static_cast<node_type*>(pNode->mpNext)->mValue = *first;
		if (first != last)
			InsertAfter(pNode, first, last, false_type());
		else
			EraseAfter(pNode, nullptr);
	}

	template <typename T, typename Allocator>
	template <typename Integer>
	void forward_list<T, Allocator>::Assign(Integer n, Integer value, true_type)
	{
		return AssignValues(static_cast<size_type>(n), static_cast<value_type>(value));
	}
}

#endif