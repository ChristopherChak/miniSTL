#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H

#include <algorithm>
#include "algorithm.h"
#include "construct.h"
#include "uninitialized.h"

namespace ministl
{
	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator uninitialized_fill_n_impl(ForwardIterator first, Size size, const T &value, true_type)
	{
		return fill_n(first, size, value);
	}

	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator uninitialized_fill_n_impl(ForwardIterator first, Size size, const T &value, false_type)
	{
		ForwardIterator current = first;
		try
		{
			for (; size > 0; --size, ++current)
				construct(&*current, value);
			return current;
		}
		catch (...)
		{
			destroy(first, current);
			throw;
		}
	}

	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator uninitialized_fill_n(ForwardIterator first, Size size, const T &value)
	{
		typedef typename iterator_traits<ForwardIterator>::value_type value_type;
		return uninitialized_fill_n_impl(first, size, value, typename type_traits<value_type>::is_POD_type());
	}

	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator uninitialized_copy_impl(InputIterator first, InputIterator last,
	                                        ForwardIterator d_first,
	                                        true_type)
	{
		return std::copy(first, last, d_first);
	}

	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator uninitialized_copy_impl(InputIterator first, InputIterator last,
	                                        ForwardIterator d_first,
	                                        false_type)
	{
		ForwardIterator current = d_first;
		try
		{
			for (; first != last; ++first, ++current)
				construct(&*current, *first);
			return current;
		}
		catch (...)
		{
			destroy(d_first, current);
			throw;
		}
	}

	char *uninitialized_copy(const char *first, const char *last, char *d_first)
	{
		std::memmove(d_first, first, last - first);
		return d_first + (last - first);
	}

	wchar_t *uninitialized_copy(const wchar_t *first, const wchar_t *last, wchar_t *d_first)
	{
		std::memmove(d_first, first, sizeof(wchar_t) * (last - first));
		return d_first + (last - first);
	}

	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
	                                   ForwardIterator d_first)
	{
		typedef typename iterator_traits<ForwardIterator>::value_type value_type;
		return uninitialized_copy_impl(first, last, d_first, typename type_traits<value_type>::is_POD_type());
	}

	template <typename ForwardIterator, typename T>
	void uninitialized_fill_impl(ForwardIterator first, ForwardIterator last, const T &value, true_type)
	{
		return fill(first, last, value);
	}

	template <typename ForwardIterator, typename T>
	void uninitialized_fill_impl(ForwardIterator first, ForwardIterator last, const T &value, false_type)
	{
		ForwardIterator current = first;
		try
		{
			for (; current != last; ++current)
				construct(&*current, value);
		}
		catch (...)
		{
			destroy(first, current);
			throw;
		}
	}

	template <typename ForwardIterator, typename T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &value)
	{
		typedef typename iterator_traits<ForwardIterator>::value_type value_type;
		uninitialized_fill_impl(first, last, value, typename type_traits<value_type>::is_POD_type());
	}
}

#endif