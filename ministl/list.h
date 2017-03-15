#ifndef LIST_H
#define LIST_H

#include <initializer_list>
#include "allocator.h"
#include "iterator.h"
#include "type_traits.h"

namespace ministl
{
	/// ListNodeBase
	struct ListNodeBase
	{
		void insert(ListNodeBase *pNext);
		void remove();
		void splice(ListNodeBase *pFirst, ListNodeBase *pLast);
		void reverse();
		void swap(ListNodeBase &other);

		void insert_range(ListNodeBase *pFirst, ListNodeBase *pLast);
		static void remove_range(ListNodeBase *pFirst, ListNodeBase *pLast);

		ListNodeBase *mpPrev;
		ListNodeBase *mpNext;
	};


	template <typename T>
	struct ListNode: public ListNodeBase
	{
		T mValue;
	};


	template <typename T, typename Reference, typename Pointer>
	struct ListIterator
	{
		typedef ptrdiff_t                           difference_type;
		typedef T                                   value_type;
		typedef Pointer                             pointer;
		typedef Reference                           reference;
		typedef bidirectional_iterator_tag          iterator_category;
		typedef ListNode<T>                         node_type;
		typedef ListIterator<T, T&, T*>             iterator;
		typedef ListIterator<T, const T&, const T*> const_iterator;
		typedef ListIterator                        this_type;

		ListIterator(const ListNodeBase *pNode = nullptr);
		ListIterator(const iterator &it);

		this_type &operator++();
		this_type  operator++(int);
		this_type &operator--();
		this_type  operator--(int);
		reference  operator*()const;
		pointer    operator->()const;

		node_type *mpNode;
	};


	template <typename T, typename Allocator>
	class ListBase
	{
	public:
		typedef ptrdiff_t    difference_type;
		typedef size_t       size_type;
		typedef ListNodeBase base_node_type;
		typedef ListNode<T>  node_type;
		typedef Allocator    allocator_type;

		const allocator_type &get_allocator()const;
		allocator_type       &get_allocator();
		void                  set_allocator(const allocator_type &alloc);

	protected:
		ListBase();
		ListBase(const allocator_type &alloc);
		~ListBase();

		node_type *AllocateNode();
		void       FreeNode(node_type *pNode);

		void Init();
		void Clear();

	protected:
		base_node_type mNode;
		allocator_type mAllocator;
		size_type      mSize;
	};


	template <typename T, typename Allocator = alloc>
	class list: public ListBase<T, Allocator>
	{
		typedef ListBase<T, Allocator>                    base_type;
		typedef list<T, Allocator>                        this_type;

	public:
		typedef T                                         value_type;
		typedef T*                                        pointer;
		typedef const T*                                  const_pointer;
		typedef T&                                        reference;
		typedef const T&                                  const_reference;
		typedef ListIterator<T, T&, T*>                   iterator;
		typedef ListIterator<T, const T&, const T*>       const_iterator;
		typedef ministl::reverse_iterator<iterator>       reverse_iterator;
		typedef ministl::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef typename base_type::size_type             size_type;
		typedef typename base_type::difference_type       difference_type;
		typedef typename base_type::allocator_type        allocator_type;
		typedef typename base_type::node_type             node_type;
		typedef typename base_type::base_node_type        base_node_type;

		using base_type::mNode;
		using base_type::mAllocator;
		using base_type::AllocateNode;
		using base_type::FreeNode;
		using base_type::Clear;
		using base_type::Init;
		using base_type::get_allocator;
		using base_type::mSize;

	public:
		list();
		list(const allocator_type &alloc);
		explicit list(size_type n, const allocator_type &alloc = Allocator());
		list(size_type n, const value_type &value, const allocator_type &alloc = Allocator());
		list(const this_type &other);
		list(const this_type &other, const allocator_type &alloc);
		list(std::initializer_list<T> ilist, const allocator_type &alloc = Allocator());

		template <typename InputIterator>
		list(InputIterator first, InputIterator last);

		this_type &operator=(const this_type &other);
		this_type &operator=(std::initializer_list<T> ilist);

		void assign(size_type count, const T &value);

		template <typename InputIterator>
		void assign(InputIterator first, InputIterator last);

		void assign(std::initializer_list<T> ilist);

		reference front();
		const_reference front()const;
		reference back();
		const_reference back()const;

		iterator begin();
		const_iterator begin()const;
		const_iterator cbegin()const;
		iterator end();
		const_iterator end()const;
		const_iterator cend()const;
		reverse_iterator rbegin();
		const_reverse_iterator rbegin()const;
		const_reverse_iterator crbegin()const;
		reverse_iterator rend();
		const_reverse_iterator rend()const;
		const_reverse_iterator crend()const;

		bool empty()const;
		size_type size()const;

		void clear();
		iterator insert(const_iterator pos, const T &value);
		iterator insert(const_iterator pos, T&& value);
		iterator insert(const_iterator pos, size_type mSize, const T &value);
		template <typename InputIterator>
		iterator insert(const_iterator pos, InputIterator first, InputIterator last);
		iterator insert(const_iterator pos, std::initializer_list<T> ilist);
		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);
		void push_back(const T &value);
		void push_back(T &&value);
		void pop_back();
		void push_front(const T &value);
		void push_front(T &&value);
		void pop_front();
		void swap(list &other);

		void merge(list &other);
		template <typename Compare>
		void merge(list &other, Compare comp);
		void splice(const_iterator pos, list &other);
		void splice(const_iterator pos, list &other, const_iterator it);
		void splice(const_iterator pos, list &other, const_iterator first, const_iterator last);
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
		void AssignValues(size_type count, const T &value);

		template <typename Integer>
		void Assign(Integer n, Integer value, true_type);

		template <typename InputIterator>
		void Assign(InputIterator first, InputIterator last, false_type);

		node_type *CreateNode(const T &value);

		node_type *CreateNode(T &&value);

		template <typename...Args>
		node_type *CreateNode(Args&&...args);

		void InsertValue(ListNodeBase *pNode, const T &value);

		void InsertValue(ListNodeBase *pNode, T &&value);

		template <typename...Args>
		void InsertValue(ListNodeBase *pNode, Args&&...args);

		void InsertValues(ListNodeBase *pNode, size_type count, const T &value);

		template <typename Integer>
		void Insert(ListNodeBase *pNode, Integer n, Integer value, true_type);

		template <typename InputIterator>
		void Insert(ListNodeBase *pNode, InputIterator first, InputIterator last, false_type);

		void Erase(node_type *pNode);
	};


	//////////////////////////////////////////////////////////
	// ListBase
	//////////////////////////////////////////////////////////

	void ListNodeBase::insert(ListNodeBase *pNext)
	{
		mpNext = pNext;
		mpPrev = pNext->mpPrev;
		pNext->mpPrev->mpNext = this;
		pNext->mpPrev = this;
	}


	void ListNodeBase::remove()
	{
		mpNext->mpPrev = mpPrev;
		mpPrev->mpNext = mpNext;
	}


	void ListNodeBase::splice(ListNodeBase *pFirst, ListNodeBase *pLast)
	{
		ListNodeBase *pTemp = pLast->mpPrev;
		pFirst->mpPrev->mpNext = pLast;
		pLast->mpPrev = pFirst->mpPrev;

		pFirst->mpPrev = mpPrev;
		pTemp->mpNext = this;
		mpPrev->mpNext = pFirst;
		mpPrev = pTemp;
	}


	void ListNodeBase::reverse()
	{
		ListNodeBase *pNode = this;
		do
		{
			ListNodeBase *pTemp = pNode->mpNext;
			pNode->mpNext = pNode->mpPrev;
			pNode->mpPrev = pTemp;
			pNode = pTemp;
		}
		while (pNode != this);
	}


	void ListNodeBase::remove_range(ListNodeBase *pFirst, ListNodeBase *pFinal)
	{
		pFirst->mpPrev->mpNext = pFinal->mpNext;
		pFinal->mpNext->mpPrev = pFirst->mpPrev;
	}


	void ListNodeBase::insert_range(ListNodeBase *pFirst, ListNodeBase *pFinal)
	{

	}


	//////////////////////////////////////////////////////////
	// ListIterator
	//////////////////////////////////////////////////////////

	template <typename T, typename Reference, typename Pointer>
	ListIterator<T, Reference, Pointer>::ListIterator(const ListNodeBase *pNode)
		: mpNode((node_type*)const_cast<ListNodeBase*>(pNode))
	{
		// empty
	}


	template <typename T, typename Reference, typename Pointer>
	ListIterator<T, Reference, Pointer>::ListIterator(const iterator &it)
		: mpNode(it.mpNode)
	{
		// empty
	}


	template <typename T, typename Reference, typename Pointer>
	typename ListIterator<T, Reference, Pointer>::this_type&
	ListIterator<T, Reference, Pointer>::operator++()
	{
		mpNode = static_cast<node_type*>(mpNode->mpNext);
		return *this;
	}


	template <typename T, typename Reference, typename Pointer>
	typename ListIterator<T, Reference, Pointer>::this_type
	ListIterator<T, Reference, Pointer>::operator++(int)
	{
		this_type temp = *this;
		++*this;
		return temp;
	}


	template <typename T, typename Reference, typename Pointer>
	typename ListIterator<T, Reference, Pointer>::this_type&
	ListIterator<T, Reference, Pointer>::operator--()
	{
		mpNode = static_cast<node_type*>(mpNode->mpPrev);
		return *this;
	}


	template <typename T, typename Reference, typename Pointer>
	typename ListIterator<T, Reference, Pointer>::this_type
	ListIterator<T, Reference, Pointer>::operator--(int)
	{
		this_type temp = *this;
		--*this;
		return temp;
	}



	template <typename T, typename Reference, typename Pointer>
	typename ListIterator<T, Reference, Pointer>::reference
	ListIterator<T, Reference, Pointer>::operator*()const
	{
		return mpNode->mValue;
	}


	template <typename T, typename Reference, typename Pointer>
	typename ListIterator<T, Reference, Pointer>::pointer
	ListIterator<T, Reference, Pointer>::operator->()const
	{
		return &mpNode->value;
	}


	template <typename T, typename ReferenceA, typename PointerA, typename ReferenceB, typename PointerB>
	bool operator==(const ListIterator<T, ReferenceA, PointerA> &lhs, const ListIterator<T, ReferenceB, PointerB> &rhs)
	{
		return lhs.mpNode == rhs.mpNode;
	}


	template <typename T, typename ReferenceA, typename PointerA, typename ReferenceB, typename PointerB>
	bool operator!=(const ListIterator<T, ReferenceA, PointerA> &lhs, const ListIterator<T, ReferenceB, PointerB> &rhs)
	{
		return lhs.mpNode != rhs.mpNode;
	}


	////////////////////////////////////////
	// ListBase
	////////////////////////////////////////

	template <typename T, typename Allocator>
	ListBase<T, Allocator>::ListBase()
		: mNode(),
		  mAllocator(Allocator()),
		  mSize(0)
	{
		Init();
	}


	template <typename T, typename Allocator>
	ListBase<T, Allocator>::ListBase(const allocator_type &alloc)
		: mNode(),
		  mAllocator(alloc),
		  mSize(0)
	{
		Init();
	}


	template <typename T, typename Allocator>
	ListBase<T, Allocator>::~ListBase()
	{
		Clear();
	}


	template <typename T, typename Allocator>
	const typename ListBase<T, Allocator>::allocator_type&
	ListBase<T, Allocator>::get_allocator()const
	{
		return mAllocator;
	}


	template <typename T, typename Allocator>
	typename ListBase<T, Allocator>::allocator_type&
	ListBase<T, Allocator>::get_allocator()
	{
		return mAllocator;
	}


	template <typename T, typename Allocator>
	void ListBase<T, Allocator>::set_allocator(const allocator_type &alloc)
	{
		mAllocator = alloc;
	}


	template <typename T, typename Allocator>
	typename ListBase<T, Allocator>::node_type*
	ListBase<T, Allocator>::AllocateNode()
	{
		node_type *new_node = (node_type*)allocate_memory(mAllocator, sizeof(node_type));
		return new_node;
	}


	template <typename T, typename Allocator>
	void ListBase<T, Allocator>::FreeNode(node_type *pNode)
	{
		MINISTLFree(mAllocator, pNode, sizeof(node_type));
	}


	template <typename T, typename Allocator>
	void ListBase<T, Allocator>::Init()
	{
		mNode.mpNext = &mNode;
		mNode.mpPrev = &mNode;
	}


	template <typename T, typename Allocator>
	void ListBase<T, Allocator>::Clear()
	{
		node_type *pNode = static_cast<node_type*>(mNode.mpNext);

		while (pNode != &mNode)
		{
			node_type *temp = static_cast<node_type*>(pNode->mpNext);
			pNode->~node_type();
			MINISTLFree(mAllocator, pNode, sizeof(node_type));
			pNode = temp;
		}
	}


	///////////////////////////////////////////////////////////////////////
	// list
	///////////////////////////////////////////////////////////////////////


	template <typename T, typename Allocator>
	list<T, Allocator>::list()
		: base_type()
	{
		// empty
	}


	template <typename T, typename Allocator>
	list<T, Allocator>::list(const allocator_type& alloc)
		: base_type(alloc)
	{
		// Empty
	}


	template <typename T, typename Allocator>
	list<T, Allocator>::list(size_type n, const allocator_type& alloc)
		: base_type(alloc)
	{
		InsertValues(&mNode, n, value_type());
	}


	template <typename T, typename Allocator>
	list<T, Allocator>::list(size_type n, const value_type &value, const allocator_type& alloc)
		: base_type(alloc)
	{
		InsertValues(&mNode, n, value);
	}


	template <typename T, typename Allocator>
	list<T, Allocator>::list(const this_type& x)
		: base_type(x.mAllocator)
	{
		Insert(&mNode, const_iterator(x.mNode.mpNext), const_iterator(&x.mNode), false_type());
	}


	template <typename T, typename Allocator>
	list<T, Allocator>::list(std::initializer_list<value_type> ilist, const allocator_type& alloc)
		: base_type(alloc)
	{
		Insert(&mNode, ilist.begin(), ilist.end(), false_type());
	}


	template <typename T, typename Allocator>
	template <typename InputIterator>
	list<T, Allocator>::list(InputIterator first, InputIterator last)
		: base_type()
	{
		Insert(&mNode, first, last, is_integral<InputIterator>());
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::this_type&
	list<T, Allocator>::operator=(const this_type &other)
	{
		if (this != &other)
			Assign(other.begin(), other.end(), false_type());
		return *this;
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::this_type&
	list<T, Allocator>::operator=(std::initializer_list<T> ilist)
	{
		Assign(ilist.begin(), ilist.end(), false_type());
		return *this;
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::assign(size_type n, const T &value)
	{
		AssignValues(n, value);
	}


	template <typename T, typename Allocator>
	template <typename InputIterator>
	void list<T, Allocator>::assign(InputIterator first, InputIterator last)
	{
		Assign(first, last, is_integral<InputIterator>());
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::assign(std::initializer_list<T> ilist)
	{
		Assign(ilist.begin(), ilist.end(), false_type());
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::reference
	list<T, Allocator>::front()
	{
		return static_cast<node_type*>(mNode.mpNext)->mValue;
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::const_reference
	list<T, Allocator>::front()const
	{
		return static_cast<node_type*>(mNode.mpNext)->mValue;
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::reference
	list<T, Allocator>::back()
	{
		return static_cast<node_type*>(mNode.mpPrev)->mValue;
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::const_reference
	list<T, Allocator>::back()const
	{
		return static_cast<node_type*>(mNode.mpPrev)->mValue;
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::iterator
	list<T, Allocator>::begin()
	{
		return iterator(mNode.mpNext);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::const_iterator
	list<T, Allocator>::begin()const
	{
		return const_iterator(mNode.mpNext);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::const_iterator
	list<T, Allocator>::cbegin()const
	{
		return const_iterator(mNode.mpNext);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::iterator
	list<T, Allocator>::end()
	{
		return iterator(&mNode);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::const_iterator
	list<T, Allocator>::end()const
	{
		return const_iterator(&mNode);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::const_iterator
	list<T, Allocator>::cend()const
	{
		return const_iterator(&mNode);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::reverse_iterator
	list<T, Allocator>::rbegin()
	{
		return reverse_iterator(&mNode);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::const_reverse_iterator
	list<T, Allocator>::rbegin()const
	{
		return const_reverse_iterator(&mNode);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::const_reverse_iterator
	list<T, Allocator>::crbegin()const
	{
		return const_reverse_iterator(&mNode);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::reverse_iterator
	list<T, Allocator>::rend()
	{
		return reverse_iterator(mNode.mpNext);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::const_reverse_iterator
	list<T, Allocator>::rend()const
	{
		return const_reverse_iterator(mNode.mpNext);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::const_reverse_iterator
	list<T, Allocator>::crend()const
	{
		return const_reverse_iterator(mNode.mpNext);
	}


	template <typename T, typename Allocator>
	bool list<T, Allocator>::empty()const
	{
		return mNode.mpNext == mNode.mpPrev;
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::size_type
	list<T, Allocator>::size()const
	{
		return mSize;
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::clear()
	{
		Clear();
		Init();
		mSize = 0;
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::iterator
	list<T, Allocator>::insert(const_iterator pos, const T &value)
	{
		++pos;
		InsertValue(pos.mpNode->mpPrev, value);
		return iterator(pos.mpNode);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::iterator
	list<T, Allocator>::insert(const_iterator pos, T && value)
	{
		++pos;
		InsertValue(pos.mpNode->mpPrev, std::move(value));
		return iterator(pos.mpNode);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::iterator
	list<T, Allocator>::insert(const_iterator pos, size_type mSize, const T &value)
	{
		for (; mSize > 0; --mSize)
			pos = insert(pos, value);
		return iterator(pos.mpNode);
	}


	template <typename T, typename Allocator>
	template <typename InputIterator>
	typename list<T, Allocator>::iterator
	list<T, Allocator>::insert(const_iterator pos, InputIterator first, InputIterator last)
	{
		for (; first != last; ++first)
			pos = insert(pos, *first);
		return iterator(pos.mpNode);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::iterator
	list<T, Allocator>::insert(const_iterator pos, std::initializer_list<T> ilist)
	{
		return insert(pos, ilist.begin(), ilist.end());
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::iterator
	list<T, Allocator>::erase(const_iterator pos)
	{
		ListNodeBase *pNext = pos.mpNode->mpNext;
		Erase(pos.mpNode);
		return iterator(pNext);
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::iterator
	list<T, Allocator>::erase(const_iterator first, const_iterator last)
	{
		while (first != last)
			first = erase(first);
		return iterator(last.mpNode);
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::push_back(const T &value)
	{
		InsertValue(&mNode, value);
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::push_back(T &&value)
	{
		InsertValue(&mNode, std::move(value));
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::pop_back()
	{
		Erase((node_type*)mNode.mpPrev);
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::push_front(const T &value)
	{
		InsertValue(mNode.mpNext, value);
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::push_front(T && value)
	{
		InsertValue(mNode.mpNext, std::move(value));
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::pop_front()
	{
		Erase((node_type*)mNode.mpNext);
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::swap(list &other)
	{

	}

	template <typename T, typename Allocator>
	void list<T, Allocator>::reverse()
	{
		mNode.reverse();
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::merge(list &other)
	{
		if (this != &other)
		{
			iterator first1 = begin();
			iterator first2 = other.begin();
			iterator last1 = end();
			iterator last2 = other.end();

			while (first1 != last1 && first2 != last2)
			{
				if (*first2 < *first1)
				{
					iterator next = first2;
					splice(first1, other, first2);
					first2 = ++next;
				}
				else
					++first1;
			}

			if (first2 != last2)
				splice(last1, other, first2, last2);
		}
	}


	template <typename T, typename Allocator>
	template <typename Compare>
	void list<T, Allocator>::merge(list &other, Compare comp)
	{
		if (this != &other)
		{
			iterator first1 = begin();
			iterator first2 = other.begin();
			iterator last1 = end();
			iterator last2 = other.end();

			while (first1 != last1 && first2 != last2)
			{
				if (comp(*first2, *first1))
				{
					iterator next = first2;
					splice(first1, other, first2);
					first2 = ++next;
				}
				else
					++first1;
			}

			if (first2 != last2)
				splice(last1, other, first2, last2);
		}
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::splice(const_iterator pos, list &other)
	{
		// TODO: Consider the case when these two are not equal
		if (!other.empty())
		{
			pos.mpNode->splice(other.mNode.mpNext, &other.mNode);
			mSize += other.mSize;
			other.mSize = 0;
		}
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::splice(const_iterator pos, list &other, const_iterator it)
	{
		if (!other.empty() && it != other.end())
		{
			pos.mpNode->splice(it.mpNode, it.mpNode->mpNext);
			++mSize;
			--other.mSize;
		}
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::splice(const_iterator pos, list &other, const_iterator first, const_iterator last)
	{
		if (first != last)
		{
			difference_type n = distance(first, last);
			pos.mpNode->splice(first.mpNode, last.mpNode);
			mSize += n;
			other.mSize -= n;
		}
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::remove(const T &value)
	{
		node_type *pNode = (node_type*)mNode.mpNext;
		while (pNode != &mNode)
		{
			if (pNode->mValue == value)
			{
				node_type *pNext = (node_type*)pNode->mpNext;
				Erase(pNode);
				pNode = pNext;
			}
			else
				pNode = (node_type*)pNode->mpNext;
		}
	}


	template <typename T, typename Allocator>
	template <typename UnaryPredicate>
	void list<T, Allocator>::remove_if(UnaryPredicate p)
	{
		node_type *pNode = (node_type*)mNode.mpNext;
		while (pNode != &mNode)
		{
			if (p((pNode->mValue)))
			{
				node_type *pNext = (node_type*)pNode->mpNext;
				Erase(pNode);
				pNode = pNext;
			}
			else
				pNode = (node_type*)pNode->mpNext;
		}
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::unique()
	{
		node_type *pSecond = (node_type*)mNode.mpNext;
		node_type *pFirst = (node_type*)pSecond->mpNext;

		while (pFirst != &mNode)
		{
			if (pFirst->mValue == pSecond->mValue)
			{
				node_type *pNext = (node_type*)pFirst->mpNext;
				Erase(pFirst);
				pFirst = pNext;
			}
			else
			{
				pSecond = pFirst;
				pFirst = (node_type*)pFirst->mpNext;
			}
		}
	}


	template <typename T, typename Allocator>
	template <typename BinaryPredicate>
	void list<T, Allocator>::unique(BinaryPredicate p)
	{
		node_type *pSecond = (node_type*)mNode.mpNext;
		node_type *pFirst = (node_type*)pSecond->mpNext;

		while (pFirst != &mNode)
		{
			if (p(pSecond->mValue, pFirst->mValue))
			{
				node_type *pNext = (node_type*)pFirst->mpNext;
				Erase(pFirst);
				pFirst = pNext;
			}
			else
			{
				pSecond = pFirst;
				pFirst = (node_type*)pFirst->mpNext;
			}
		}
	}

	template <typename T, typename Allocator>
	void list<T, Allocator>::AssignValues(size_type count, const T &value)
	{
		iterator first(begin());
		iterator last(end());

		for (; first != last && count > 0; ++first, --count)
			*first = value;

		if (first != last)
			erase(first, last);
		else
			InsertValues(&mNode, count, value);
	}


	template <typename T, typename Allocator>
	template <typename Integer>
	void list<T, Allocator>::Assign(Integer n, Integer value, true_type)
	{
		return AssignValues(static_cast<size_type>(n), static_cast<value_type>(value));
	}


	template <typename T, typename Allocator>
	template <typename InputIterator>
	void list<T, Allocator>::Assign(InputIterator first, InputIterator last, false_type)
	{
		iterator iBegin(begin());
		iterator iEnd(end());

		for (; iBegin != iEnd && first != last; ++iBegin, ++first)
			*iBegin = *first;

		if (iBegin != iEnd)
			erase(iBegin, iEnd);
		else
			Insert(&mNode, first, last, false_type());
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::node_type *
	list<T, Allocator>::CreateNode(const T &value)
	{
		node_type *new_node = AllocateNode();
		try
		{
			new(&new_node->mValue)value_type(value);
		}
		catch (...)
		{
			FreeNode(new_node);
			throw;
		}
		return new_node;
	}


	template <typename T, typename Allocator>
	typename list<T, Allocator>::node_type *
	list<T, Allocator>::CreateNode(T &&value)
	{
		node_type *new_node = AllocateNode();
		try
		{
			new(&new_node->mValue)value_type(std::move(value));
		}
		catch (...)
		{
			FreeNode(new_node);
			throw;
		}
		return new_node;
	}


	template <typename T, typename Allocator>
	template <typename...Args>
	typename list<T, Allocator>::node_type *
	list<T, Allocator>::CreateNode(Args&&...args)
	{
		node_type *new_node = AllocateNode();
		try
		{
			new(&new_node->mValue)value_type(std::forward<Args>(args)...);
		}
		catch (...)
		{
			FreeNode(new_node);
			throw;
		}
		return new_node;
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::InsertValue(ListNodeBase *pNode, const T &value)
	{
		++mSize;
		node_type *pNewNode = CreateNode(value);
		pNewNode->insert(pNode);
	}

	template <typename T, typename Allocator>
	void list<T, Allocator>::InsertValue(ListNodeBase *pNode, T &&value)
	{
		++mSize;
		node_type *pNewNode = CreateNode(std::move(value));
		pNewNode->insert(pNode);
	}


	template <typename T, typename Allocator>
	template<typename... Args>
	void list<T, Allocator>::InsertValue(ListNodeBase *pNode, Args&&... args)
	{
		++mSize;
		node_type *pNewNode = CreateNode(std::forward<Args>(args)...);
		pNewNode->insert(pNode);
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::InsertValues(ListNodeBase *pNode, size_type n, const T& value)
	{
		for (; n > 0; --n)
			InsertValue(pNode, value);
	}


	template <typename T, typename Allocator>
	template <typename Integer>
	void list<T, Allocator>::Insert(ListNodeBase* pNode, Integer n, Integer value, true_type)
	{
		InsertValues(pNode, static_cast<size_type>(n), static_cast<value_type>(value));
	}


	template <typename T, typename Allocator>
	template <typename InputIterator>
	void list<T, Allocator>::Insert(ListNodeBase* pNode, InputIterator first, InputIterator last, false_type)
	{
		for (; first != last; ++first)
			InsertValue(pNode, *first);
	}


	template <typename T, typename Allocator>
	void list<T, Allocator>::Erase(node_type *pNode)
	{
		--mSize;
		pNode->remove();
		pNode->~node_type();
		FreeNode(pNode);
	}
}

#endif