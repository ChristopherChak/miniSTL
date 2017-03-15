#ifndef ARRAY_H
#define ARRAY_H

#include "algorithm.h"
#include "iterator.h"

namespace ministl
{
	template <typename T, size_t N = 1>
	struct array
	{
		typedef array<T, N>                                   this_type;
		typedef T                                            value_type;
		typedef T*                                           pointer;
		typedef const T*                                     const_pointer;
		typedef T&                                           reference;
		typedef const T&                                     const_reference;
		typedef T*                                           iterator;
		typedef const T*                                     const_iterator;
		typedef ministl::reverse_iterator<iterator>          reverse_iterator;
		typedef ministl::reverse_iterator<const_iterator>    const_reverse_iterator;
		typedef size_t                                       size_type;
		typedef ptrdiff_t                                    difference_type;

	public:
		value_type mValue[N ? N : 1];

	public:
		reference       at(size_type i);
		const_reference at(size_type i)const;
		reference       operator[](size_type i);
		const_reference operator[](size_type i)const;

		reference       front();
		const_reference front()const;

		reference       back();
		const_reference back()const;

		T*       data();
		const T* data()const;

		iterator       begin();
		const_iterator begin()const;
		const_iterator cbegin()const;
		iterator       end();
		const_iterator end()const;
		const_iterator cend()const;

		reverse_iterator       rbegin();
		const_reverse_iterator rbegin()const;
		const_reverse_iterator crbegin()const;
		reverse_iterator       rend();
		const_reverse_iterator rend()const;
		const_reverse_iterator crend()const;

		bool empty()const;
		size_type size()const;

		void fill(const T &value);
		void swap(this_type &other);
	};

	template <typename T, size_t N>
	inline typename array<T, N>::reference
	array<T, N>::at(size_type i)
	{
		if (i >= size())
			throw std::out_of_range("array::at(size_type i) out of range");
		return mValue[i];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_reference
	array<T, N>::at(size_type i)const
	{
		if (i >= size())
			throw std::out_of_range("array::at(size_type i) out of range");
		return mValue[i];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::reference
	array<T, N>::operator[](size_type i)
	{
		return mValue[i];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_reference
	array<T, N>::operator[](size_type i)const
	{
		return mValue[i];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::reference
	array<T, N>::front()
	{
		return mValue[0];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_reference
	array<T, N>::front()const
	{
		return mValue[0];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::reference
	array<T, N>::back()
	{
		return mValue[N - 1];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_reference
	array<T, N>::back()const
	{
		return mValue[N - 1];
	}

	template <typename T, size_t N>
	inline T*
	array<T, N>::data()
	{
		return mValue;
	}

	template <typename T, size_t N>
	inline const T*
	array<T, N>::data()const
	{
		return mValue;
	}

	template <typename T, size_t N>
	inline typename array<T, N>::iterator
	array<T, N>::begin()
	{
		return &mValue[0];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_iterator
	array<T, N>::begin()const
	{
		return &mValue[0];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_iterator
	array<T, N>::cbegin()const
	{
		return &mValue[0];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::iterator
	array<T, N>::end()
	{
		return &mValue[N];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_iterator
	array<T, N>::end()const
	{
		return &mValue[N];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_iterator
	array<T, N>::cend()const
	{
		return &mValue[N];
	}

	template <typename T, size_t N>
	inline typename array<T, N>::reverse_iterator
	array<T, N>::rbegin()
	{
		return reverse_iterator(&mValue[N]);
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_reverse_iterator
	array<T, N>::rbegin()const
	{
		return const_reverse_iterator(&mValue[N]);
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_reverse_iterator
	array<T, N>::crbegin()const
	{
		return const_reverse_iterator(&mValue[N]);
	}

	template <typename T, size_t N>
	inline typename array<T, N>::reverse_iterator
	array<T, N>::rend()
	{
		return reverse_iterator(&mValue[0]);
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_reverse_iterator
	array<T, N>::rend()const
	{
		return const_reverse_iterator{&mValue[0]};
	}

	template <typename T, size_t N>
	inline typename array<T, N>::const_reverse_iterator
	array<T, N>::crend()const
	{
		return const_reverse_iterator(&mValue[0]);
	}

	template <typename T, size_t N>
	inline bool
	array<T, N>::empty()const
	{
		return N == 0;
	}

	template <typename T, size_t N>
	inline typename array<T, N>::size_type
	array<T, N>::size()const
	{
		return (size_type)N;
	}

	template <typename T, size_t N>
	inline void
	array<T, N>::fill(const T &value)
	{
		ministl::fill_n(&mValue[0], N, value);
	}

	template <typename T, size_t N>
	inline void
	array<T, N>::swap(this_type &other)
	{
		ministl::swap_ranges(&mValue[0], &mValue[N], &other.mValue[0]);
	}

	///////////////////////////////////////////////////////////
	// non-member functions
	///////////////////////////////////////////////////////////

	template <typename T, size_t N>
	inline bool operator==(const array<T, N> &lhs, const array<T, N> &rhs)
	{
		return ministl::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <typename T, size_t N>
	inline bool operator!=(const array<T, N> &lhs, const array<T, N> &rhs)
	{
		return !ministl::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <typename T, size_t N>
	inline bool operator<(const array<T, N> &lhs, const array<T, N> &rhs)
	{
		return ministl::lexicographical_compare(&lhs.mValue[0], &lhs.mValue[N], &rhs.mValue[0], &rhs.mValue[N]);
	}

	template <typename T, size_t N>
	inline bool operator<=(const array<T, N> &lhs, const array<T, N> &rhs)
	{
		return !ministl::lexicographical_compare(&rhs.mValue[0], &rhs.mValue[N], &lhs.mValue[0], &lhs.mValue[N]);
	}

	template <typename T, size_t N>
	inline bool operator>(const array<T, N> &lhs, const array<T, N> &rhs)
	{
		return ministl::lexicographical_compare(&rhs.mValue[0], &rhs.mValue[N], &lhs.mValue[0], &lhs.mValue[N]);
	}

	template <typename T, size_t N>
	inline bool operator>=(const array<T, N> &lhs, const array<T, N> &rhs)
	{
		return !ministl::lexicographical_compare(&lhs.mValue[0], &lhs.mValue[N], &rhs.mValue[0], &rhs.mValue[N]);
	}

	template <typename T, size_t N>
	void swap(array<T, N> &lhs, array<T, N> &rhs)
	{
		lhs.swap(rhs);
	}

}

#endif