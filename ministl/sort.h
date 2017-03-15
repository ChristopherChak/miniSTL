#ifndef HEAP_H
#define HEAP_H

#include "heap.h"
#include "iterator.h"

namespace ministl
{
	template <typename ForwardIterator>
	ForwardIterator is_sorted_until(ForwardIterator first, ForwardIterator last)
	{
		if (first == last)
			return last;

		ForwardIterator next = first;
		while (++next != last && *next < *first)
			first = next;

		return next;
	}


	template <typename ForwardIterator>
	bool is_sorted(ForwardIterator first, ForwardIterator last)
	{
		return is_sorted_until(first, last) == last;
	}

	/*template <typename RandomAccessIterator>
	void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last)
	{
		// sort the range [first, middle) into a max heap, we assume the
		// smallest elements are in the range [first, middle)
		ministl::make_heap(first, middle);

		// for every element in range [midddle, last), if it is smaller than the maximum
		// element in the max heap, which disobeys the assumption, then make it be a part of
		// max heap
		for (RandomAccessIterator it = middle; middle != last; ++middle)
		{
			if (*it < *first)
			{
				ministl::iter_swap(middle, it);
				ministl::pop_heap(first, middle + 1);
			}
		}

		// sort the max heap in order
		ministl::sort_heap(first, middle);
	}*/


	/*template <typename RandomAccessIterator>
	inline void
	partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last)
	{
		using difference_type = typename std::iterator_traits<RandomAccessIterator>::difference_type;
		using va

		ministl::make_heap(first, middle);

		for (RandomAccessIterator it = middle; it < last; ++it)
		{
			if (*it < *first)
			{
				ministl::pop_heap(first, );
			}
		}

		ministl::sort_heap(first, middle);
	}*/

	// the other version of partial_sort, which additionally takes a binary comparion
	// function.
	//
	// template <typename RandomAccessIterator, typename Compare>
	// inline void
	// partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last,
	//              Compare comp)
	// {
	//   ...
	//   change if (*it < *first) to
	//          if (p(*it < *first))
	//   ...
	// }

	// for simplicty, I won't implement such functions(which additionally takes a binary
	// comparion function or a unnary prediate).


	// insertion sort. This function uses operator< to compare different elements
	// in the range [first, last).
	template <typename BidirectionalIterator>
	void insert_sort(BidirectionalIterator first, BidirectionalIterator last)
	{
		if (first == last)
			return;

		using value_type = typename std::iterator_traits<BidirectionalIterator>::value_type;

		BidirectionalIterator it_current, it_next, it_sorted = first;
		for (++it_sorted; it_sorted != last; ++it_sorted)
		{
			value_type value = *it_sorted;

			it_current = it_next = it_sorted;
			for (--it_current; it_current != first && value < *it_current; --it_next, --it_current)
				*it_next = *it_current;

			if (it_current == first && value < *it_current)
			{
				*it_next = *it_current;
				--it_next;
			}

			*it_next = value;
		}
	}

	template <typename Size>
	inline Size __lg(Size size)
	{
		Size k = 0;
		for (; size > 1; size >>= 1)
			++k;
		return k;
	}

	template <typename RandomAccessIterator, typename T>
	void __ungarded_insert(RandomAccessIterator last, const T &value)
	{
		RandomAccessIterator it_current = last - 1;
		for (; value < *it_current; --it_current)
			*(it_current + 1) = *(it_current);
		*(it_current + 1) = value;
	}

	/// Note that this version of insertion sort will be used by quick sort
	/// when the the amount of elements in range [first, last) is less than
	/// a particular value(usually between 5-15).
	template <typename RandomAccessIterator>
	void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		// we only consider the cases where first < last
		if (first == last)
			return;

		using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;

		// it_sorted < last only holds when it_sorted and last are all random
		// access iterators.
		for (RandomAccessIterator it_sorted = first + 1; it_sorted < last; ++it_sorted)
		{
			value_type value = *it_sorted;

			// Note that this is a common trick used to achieve higher effeciency
			// (1) when the inserted value is less than *first, copy directly the
			//     values in range [first, it_sorted) to range [first + 1, it_sorted + 1)
			// (2) if (1) doesn't hold, then we can compare value with elements in [first, it_sorted)
			//     without doing bound test, which saves a lot of comparisions when the algorithm
			//     operates on a huge data set.
			if (value < *first)
			{
				std::copy_backward(first, it_sorted, it_sorted + 1);
				*first = value;
			}
			else
				__ungarded_insert(it_sorted, value);
		}
	}

	template <typename T>
	const T&
	__median(const T &a, const T &b, const T &c)
	{
		if (a < b)
		{
			if (b < c)
				return b;
			else
			{
				if (c < a)
					return a;
				else
					return  c;
			}
		}
		else
		{
			if (c < b)
				return b;
			else
			{
				if (c < a)
					return c;
				else
					return a;
			}
		}
	}

	template <typename RandomAccessIterator, typename T>
	RandomAccessIterator __ungarded_partition(RandomAccessIterator first, RandomAccessIterator last, T pivot)
	{
		while (true)
		{
			// stop when *first >= pivot
			while (*first < pivot)
				++first;

			// stop when *last <= pivot
			--last;
			while (pivot < *last)
				--last;

			if (first >= last)
				return first;

			std::iter_swap(first, last);
			++first;
		}
	}

	/// quick sort. This function uses operator< to compare different elements in range
	/// [first, last).

	/// This implementation uses the following optimization approaches:
	/// (1) do nothing for small ranges which satifies last - first <= integer
	/// (2) use median-of-three partitioning. That is, choose the median of the first element, last element,
	///     and the middle element in the range [first, last) as the partition pivot.
	/// (3) use ungarded partition.
	template <typename RandomAccessIterator, typename Integer>
	void __quick_sort(RandomAccessIterator first,
	                  RandomAccessIterator last,
	                  Integer integer)
	{
		if (last - first < integer)
			return;

		const typename std::iterator_traits<RandomAccessIterator>::value_type &median =
		    __median(*first,
		             *(first + (last - first) / 2),
		             *(last - 1));

		RandomAccessIterator it = __ungarded_partition(first, last, median);

		__quick_sort(first, it, integer);
		__quick_sort(it, last, integer);

	}

	template <typename RandomAccessIterator>
	void sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		__quick_sort(first, last, 10);
		__insertion_sort(first, last);
	}

	template <typename RandomAccessIterator, typename Integer>
	void __merge_sort(RandomAccessIterator first, RandomAccessIterator last,
	                  Integer threhold)
	{
		if (last - first < threhold)
			return __insertion_sort(first, last);

		RandomAccessIterator middle = first + ((last - first) >> 1);
		__merge_sort(first, middle, threhold);
		__merge_sort(middle, last, threhold);

		if (*middle < * (middle - 1))
			std::inplace_merge(first, middle, last);
	}

	template <typename RandomAccessIterator>
	void stable_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		return __merge_sort(first, last, 10);
	}

	template <typename RandomAccessIterator>
	void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last)
	{
		typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

		while (last - first > 5)
		{
			value_type median = __median(*first,
			                             *(first + (last - first) / 2),
			                             *(last - 1));
			RandomAccessIterator it = __ungarded_partition(first, last, median);

			if (it <= nth)
				first = it;
			else
				last = it;
		}

		__insertion_sort(first, last);
	}
}

#endif // sort.h