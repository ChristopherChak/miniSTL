#include <exception>
#include "allocator.h"
#include "uninitialized.h"
#include "iterator.h"
#include "type_traits.h"

namespace ministl
{
	template <typename T, typename Allocator>
	struct VectorBase
	{
		typedef ptrdiff_t           difference_type;
		typedef size_t              size_type;
		typedef Allocator           allocator_type;

	public:
		const allocator_type& get_allocator()const;
		allocator_type&       get_allocator();
		void                  set_allocator(const allocator_type &alloc);

	protected:
		VectorBase();
		VectorBase(const allocator_type &alloc);
		VectorBase(size_type n, const allocator_type &alloc = allocator_type());
		~VectorBase();

	protected:
		T *mpBegin;
		T *mpEnd;
		T *mpCapacity;
		allocator_type mAllocator;

	protected:
		T*        Allocate(size_type n);
		void      Free(T *p, size_type n);
		size_type GetNewCapacity(size_type currentCapacity);

	};

	template <typename T, typename Allocator>
	VectorBase<T, Allocator>::VectorBase()
		: mpBegin(NULL),
		  mpEnd(NULL),
		  mpCapacity(NULL)
	{
		// empty
	}

	template <typename T, typename Allocator>
	VectorBase<T, Allocator>::VectorBase(const allocator_type &alloc)
		: mpBegin(NULL),
		  mpEnd(NULL),
		  mpCapacity(NULL),
		  mAllocator(alloc)
	{
		// empty
	}

	template <typename T, typename Allocator>
	VectorBase<T, Allocator>::VectorBase(size_type n, const allocator_type &alloc)
		: mAllocator(alloc)
	{
		mpBegin = Allocate(n);
		mpEnd = mpBegin;
		mpCapacity = mpBegin + n;
	}

	template <typename T, typename Allocator>
	VectorBase<T, Allocator>::~VectorBase()
	{
		Free(mpBegin, mpEnd - mpBegin);
	}

	template <typename T, typename Allocator>
	inline typename VectorBase<T, Allocator>::allocator_type&
	VectorBase<T, Allocator>::get_allocator()
	{
		return mAllocator;
	}

	template <typename T, typename Allocator>
	inline const typename  VectorBase<T, Allocator>::allocator_type&
	VectorBase<T, Allocator>::get_allocator()const
	{
		return mAllocator;
	}

	template <typename T, typename Allocator>
	inline T *VectorBase<T, Allocator>::Allocate(size_type n)
	{
		return (T*)allocate_memory(mAllocator, n * sizeof(T));
	}

	template <typename T, typename Allocator>
	inline void VectorBase<T, Allocator>::Free(T *p, size_type n)
	{
		MINISTLFree(mAllocator, p, n * sizeof(T));
	}

	template <typename T, typename Allocator>
	inline typename VectorBase<T, Allocator>::size_type
	VectorBase<T, Allocator>::GetNewCapacity(size_type currentCapacity)
	{
		return (currentCapacity == 0) ? 1 : currentCapacity << 1;
	}




	template <typename T, typename Allocator = alloc>
	class vector: public VectorBase<T, Allocator>
	{
		typedef VectorBase<T, Allocator>            base_type;
		typedef vector<T, Allocator>                this_type;

	public:
		typedef T                                            value_type;
		typedef T*                                           pointer;
		typedef const T*                                     const_pointer;
		typedef T&                                           reference;
		typedef const T&                                     const_reference;
		typedef T*                                           iterator;
		typedef const T*                                     const_iterator;
		typedef ministl::reverse_iterator<iterator>          reverse_iterator;
		typedef ministl::reverse_iterator<const_iterator>    const_reverse_iterator;
		typedef typename base_type::size_type                size_type;
		typedef typename base_type::difference_type          difference_type;
		typedef typename base_type::allocator_type           allocator_type;

		using base_type::mpBegin;
		using base_type::mpEnd;
		using base_type::mpCapacity;
		using base_type::mAllocator;
		using base_type::GetNewCapacity;
		using base_type::Allocate;
		using base_type::Free;

	public:
		vector();
		explicit vector(const allocator_type &alloc);
		explicit vector(size_type n, const allocator_type &alloc = allocator_type());
		vector(size_type n, const T &value, const allocator_type &alloc = allocator_type());
		vector(std::initializer_list<T> ilist, const allocator_type &alloc = allocator_type());
		vector(const this_type &other);
		vector(const this_type &other, const allocator_type &alloc);
		template <typename InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type());

		~vector();

		vector &operator=(const this_type &other);
		vector &operator=(std::initializer_list<T> ilist);

		void assign(size_type n, const T &value);
		template <typename InputIterator>
		void assign(InputIterator first, InputIterator last);
		void assign(std::initializer_list<T> ilist);

		iterator               begin();
		const_iterator         begin()const;
		const_iterator         cbegin()const;

		reverse_iterator       rbegin();
		const_reverse_iterator rbegin()const;
		const_reverse_iterator crbegin()const;

		iterator               end();
		const_iterator         end()const;
		const_iterator         cend()const;

		reverse_iterator       rend();
		const_reverse_iterator rend()const;
		const_reverse_iterator crend()const;

		reference       at(size_type pos);
		const_reference at(size_type pos)const;
		reference       operator[](size_type pos);
		const_reference operator[](size_type pos)const;
		reference       front();
		const_reference front()const;
		reference       back();
		const_reference back()const;

		T *data();
		const T *data()const;

		size_type  size()const;
		bool       empty()const;
		size_type  capacity()const;
		void       reserve(size_type n);
		void       shrink_to_fit();

		void clear();
		iterator insert(const_iterator pos, const value_type &value);
		iterator insert(const_iterator pos, value_type &&value);
		iterator insert(const_iterator pos, size_type count, const value_type &value);
		iterator insert(const_iterator pos, std::initializer_list<T> ilist);
		template <typename InputIterator>
		iterator insert(const_iterator pos, InputIterator first, InputIterator last);
		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);
		void push_back(const T &value);
		void push_back(T &&value);

	protected:
		template <typename Integer>
		void Init(Integer n, Integer value, true_type);

		template <typename InputIterator>
		void Init(InputIterator first, InputIterator last, false_type);

		template <typename InputIterator>
		void InitFromIterator(InputIterator first, InputIterator last, input_iterator_tag);

		template <typename ForwardIterator>
		void InitFromIterator(ForwardIterator first, ForwardIterator last, forward_iterator_tag);

		void GetMoreMemory();
	};

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector()
		: base_type()
	{
		// empty
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(const allocator_type &alloc)
		: base_type(alloc)
	{
		// empty
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(size_type n, const allocator_type &alloc)
		: base_type(n, alloc)
	{
		mpEnd = uninitialized_fill_n(mpBegin, n, value_type());
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(size_type n, const T &value, const allocator_type &alloc)
		: base_type(n, alloc)
	{
		mpEnd = uninitialized_fill_n(mpBegin, n, value);
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(const vector &other)
		: base_type(other.size())
	{
		mpEnd = uninitialized_copy(other.begin(), other.end(), mpBegin);
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(const vector &other, const allocator_type &alloc)
		: base_type(other.size(), alloc)
	{
		mpEnd = uninitialized_copy(other.begin(), other.end(), mpBegin);
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(std::initializer_list<T> ilist, const allocator_type &alloc)
		: base_type(ilist.size(), alloc)
	{
		mpEnd = uninitialized_copy(ilist.begin(), ilist.end(), mpBegin);
	}

	template <typename T, typename Allocator>
	template <typename InputIterator>
	vector<T, Allocator>::vector(InputIterator first, InputIterator last, const allocator_type &alloc)
		: base_type(alloc)
	{
		Init(first, last, is_integral<InputIterator>());
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::~vector()
	{
		destroy(mpBegin, mpEnd);
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::size_type
	vector<T, Allocator>::size()const
	{
		return mpEnd - mpBegin;
	}

	template <typename T, typename Allocator>
	inline bool
	vector<T, Allocator>::empty()const
	{
		return mpBegin == mpEnd;
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::size_type
	vector<T, Allocator>::capacity()const
	{
		return mpCapacity - mpBegin;
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::iterator
	vector<T, Allocator>::begin()
	{
		return mpBegin;
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_iterator
	vector<T, Allocator>::begin()const
	{
		return mpBegin;
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_iterator
	vector<T, Allocator>::cbegin()const
	{
		return mpBegin;
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::iterator
	vector<T, Allocator>::end()
	{
		return mpEnd;
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_iterator
	vector<T, Allocator>::end()const
	{
		return mpEnd;
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_iterator
	vector<T, Allocator>::cend()const
	{
		return mpEnd;
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::reverse_iterator
	vector<T, Allocator>::rbegin()
	{
		return reverse_iterator(mpEnd);
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reverse_iterator
	vector<T, Allocator>::rbegin()const
	{
		return const_reverse_iterator(mpEnd);
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reverse_iterator
	vector<T, Allocator>::crbegin()const
	{
		return const_reverse_iterator(mpEnd);
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::reverse_iterator
	vector<T, Allocator>::rend()
	{
		return reverse_iterator(mpBegin);
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reverse_iterator
	vector<T, Allocator>::rend()const
	{
		return const_reverse_iterator(mpBegin);
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reverse_iterator
	vector<T, Allocator>::crend()const
	{
		return const_reverse_iterator(mpBegin);
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::reference
	vector<T, Allocator>::at(size_type n)
	{
		if (n >= size())
			throw std::out_of_range("index out of range");
		return mpBegin[n];
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reference
	vector<T, Allocator>::at(size_type n)const
	{
		if (n >= size())
			throw std::out_of_range("index out of range");
		return mpBegin[n];
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::reference
	vector<T, Allocator>::operator[](size_type n)
	{
		return mpBegin[n];
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reference
	vector<T, Allocator>::operator[](size_type n)const
	{
		return mpBegin[n];
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::reference
	vector<T, Allocator>::front()
	{
		return *mpBegin;
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reference
	vector<T, Allocator>::front()const
	{
		return *mpBegin;
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::reference
	vector<T, Allocator>::back()
	{
		return *(mpEnd - 1);
	}

	template <typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reference
	vector<T, Allocator>::back()const
	{
		return *(mpEnd - 1);
	}

	template <typename T, typename Allocator>
	inline T *vector<T, Allocator>::data()
	{
		return mpBegin;
	}

	template <typename T, typename Allocator>
	inline const T *vector<T, Allocator>::data()const
	{
		return mpBegin;
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::push_back(const T &value)
	{
		if (mpEnd < mpCapacity)
			GetMoreMemory();

		try
		{
			new(mpEnd)value_type(value);
			++mpEnd;
		}
		catch (...)
		{
			// do nothing
		}
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::push_back(T &&value)
	{
		if (mpEnd < mpCapacity)
			GetMoreMemory();

		try
		{
			new(mpEnd)value_type(std::move(value));
			++mpEnd;
		}
		catch (...)
		{
			// do nothing
		}
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::GetMoreMemory()
	{
		size_type newSize = GetNewCapacity(size());

		T *new_mpBegin = Allocate(newSize);
		uninitialized_copy(mpBegin, mpEnd, new_mpBegin);
		destroy(mpBegin, mpEnd);
		Free(mpBegin, size());

		mpBegin = new_mpBegin;
		mpCapacity = mpBegin + newSize;
		mpEnd = mpCapacity;
	}

	template <typename T, typename Allocator>
	template <typename Integer>
	void vector<T, Allocator>::Init(Integer n, Integer value, true_type)
	{
		mpBegin = Allocate((size_type)n);
		mpCapacity = mpBegin + n;
		mpEnd = mpCapacity;
		uninitialized_fill_n(mpBegin, n, (value_type)value);
	}

	template <typename T, typename Allocator>
	template <typename InputIterator>
	void vector<T, Allocator>::Init(InputIterator first, InputIterator last, false_type)
	{
		// we need to dispatch init work to different functions according to the category of
		// InputIterator
		return InitFromIterator(first, last, typename iterator_traits<InputIterator>::iterator_category());
	}

	template <typename T, typename Allocator>
	template <typename InputIterator>
	void vector<T, Allocator>::InitFromIterator(InputIterator first, InputIterator last, input_iterator_tag)
	{
		// we can only iterate through the range [first, last) once by input iterator definition
		for (; first != last; ++first)
			push_back(*first);
	}

	template <typename T, typename Allocator>
	template <typename ForwardIterator>
	void vector<T, Allocator>::InitFromIterator(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
	{
		typename iterator_traits<ForwardIterator>::difference_type n = distance(first, last);
		mpBegin = Allocate((size_type)n);
		mpCapacity = mpBegin + n;
		mpEnd = mpCapacity;
		uninitialized_copy(first, last, mpBegin);
	}





}