#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "iterator.h"
#include "sort.h"

namespace ministl
{
	/************************************** Non-modifying sequence operations ***********************************/

	// Checks if unary predicate p returns true for all elements in the
	// range [first, last).
	template <typename InputIterator, typename UnaryPrediate>
	bool all_of(InputIterator first, InputIterator last, UnaryPrediate p)
	{
		for (; first != last; ++first)
		{
			if (!p(*first)) 
				return false;
		}
		return true;
	}

	// Checks if unary predicate p returns true for at least one element in
	// the range [first, last).
	template <typename InputIterator, typename UnaryPrediate>
	bool any_of(InputIterator first, InputIterator last, UnaryPrediate p)
	{
		for (; first != last; ++first)
		{
			if (p(*first)) 
				return true;
		}
		return false;
	}

	// Checks if unary predicate p returns true for no elements in the range
	// [first, last).
	template <typename InputIterator, typename UnaryPrediate>
	bool none_of(InputIterator first, InputIterator last, UnaryPrediate p)
	{
		for (; first != last; ++first)
		{
			if (p(*first)) 
				return false;
		}
		return true;
	}

	// Applies the given function object f to the result of dereferencing every
	// iterator in the range [first, last), in order.
	template <typename InputIterator, typename UnaryFunction>
	UnaryFunction for_each(InputIterator first, InputIterator last, UnaryFunction f)
	{
		for (; first != last; ++first) 
			f(*first);
		return f;
	}

	// Returns the number of elements in the range [first, last) satisfying specific
	// criteria.

	// counts the elements that are equal to value.
	template <typename InputIterator, typename T>
	typename iterator_traits<InputIterator>::difference_type
	count(InputIterator first, InputIterator last, const T &value)
	{
		typename iterator_traits<InputIterator>::difference_type count = 0;
		for (; first != last; ++first)
		{
			if (*first == value)
				++count;
		}
		return count;
	}

	// counts elements for which predicate p returns true
	template <typename InputIterator, typename UnaryPrediate>
	typename iterator_traits<InputIterator>::difference_type
	count_if(InputIterator first, InputIterator last, UnaryPrediate p)
	{
		typename std::iterator_traits<InputIterator>::difference_type count = 0;
		for (; first != last; ++first)
		{
			if (p(*first)) ++count;
		}
		return count;
	}

	// Returns the first mismatching pair of elements from two ranges: one
	// defined by [first1, last1) and another defined by [first2,last2)
	// where last2 denotes first2 + (last1 - first1).

	// Elements are compared using operator==.
	template <typename InputIterator1, typename InputIterator2>
	std::pair<InputIterator1, InputIterator2>
	mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		while (first1 != last1 && *first1 == *first2)
		{
			++first1, ++first2;
		}
		return std::pair<InputIterator1, InputIterator2> {first1, first2};
	}

	// Elements are compared using the given binary predicate p.
	template <typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
	std::pair<InputIterator1, InputIterator2>
	mismatch(InputIterator1 first1, InputIterator1 last1,
	         InputIterator2 first2,
	         BinaryPredicate P)
	{
		while (first1 != last1 && p(*first1, *first2))
		{
			++first1, ++first2;
		}
		return std::pair<InputIterator1, InputIterator2> {first1, first2};
	}

	// Return true if range [first1,last1] and [first2,first2 + last1 - first1],
	// is equal, false otherwise.
	template <typename InputIterator1, typename InputIterator2>
	bool equal(InputIterator1 first1, InputIterator1 last1,
	           InputIterator2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!(*first1 == *first2)) return false;
		}
		return true;
	}

	// Same as the the first version, but use binary predicate p.
	template <typename InputIterator1, typename InputIterator2,
	          typename BinaryOperator>
	bool equal(InputIterator1 first1, InputIterator1 last1,
	           InputIterator2 first2,
	           BinaryOperator p)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!p(*first1, *first2)) return false;
		}
		return true;
	}

	// Returns the first element in the range [first, last) that satisfies specific
	// criteria:

	// find searches for an element equal to value.
	template <typename InputIterator, typename T>
	InputIterator find(InputIterator first, InputIterator last, const T &value)
	{
		while (first != last && *first != value) ++first;
		return first;
	}

	// find_if searches for an element for which predicate p returns true.
	template <typename InputIterator, typename UnaryPrediate>
	InputIterator find_if(InputIterator first, InputIterator last, UnaryPrediate p)
	{
		while (first != last && !p(*first)) ++first;
		return first;
	}

	// find_if_not searches for an element for which predicate p returns false
	template <typename InputIterator, typename UnaryPrediate>
	InputIterator find_if_not(InputIterator first, InputIterator last, UnaryPrediate p)
	{
		while (first != last && p(*first)) ++first;
		return first;
	}

	// Searches for the last subsequence of elements [s_first, s_last) in the range
	// [first, last).
	template <typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator1 find_end(ForwardIterator1 first, ForwardIterator1 last,
	                          ForwardIterator2 s_first, ForwardIterator2 s_last)
	{
		if (s_first == s_last)
			return last;

		ForwardIterator1 result = last;
		while (true)
		{
			ForwardIterator1 new_result = search(first, last, s_first, s_last);
			if (new_result == last)
				return result;
			else
			{
				result = new_result;
				first = ++new_result;
			}
		}
	}

	// Searches the range [first, last) for any of the elements in the range
	// [s_first, s_last).

	// Elements are compared using operator==.
	template <typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator1 find_first_of(ForwardIterator1 first, ForwardIterator1 last,
	                               ForwardIterator2 s_first, ForwardIterator2 s_last)
	{
		for (; first != last; ++first)
		{
			for (ForwardIterator2 s_it = s_first; s_it != s_last; ++s_it)
			{
				if (*s_it == *first) return first;
			}
		}
		return last;
	}

	// Elements are compared using the given binary predicate p.
	template <typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
	ForwardIterator1 find_first_of(ForwardIterator1 first, ForwardIterator1 last,
	                               ForwardIterator2 s_first, ForwardIterator2 s_last,
	                               BinaryPredicate p)
	{
		for (; first != last; ++first)
		{
			for (ForwardIterator2 s_it = s_first; s_it != s_last; ++s_it)
			{
				if (p(*s_it, *first)) return first;
			}
		}
		return last;
	}

	// Searches the range [first, last) for two consecutive identical elements.

	// Elements are compared using operator==.
	template <typename ForwardIterator>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last)
	{
		if (first == last) return last;

		ForwardIterator next = first;
		while (++next != last)
		{
			if (*first == *next) return first;
			first = next;
		}
		return last;
	}

	// Elements are compared using the given binary predicate p.
	template <typename ForwardIterator, typename BinaryPredicate>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
	                              BinaryPredicate p)
	{
		if (first == last) return last;

		ForwardIterator next = first;
		while (++next != last)
		{
			if (p(*first, *next)) return first;
			first = next;
		}
		return last;
	}

	// Searches for the first occurrence of the subsequence of elements
	// [s_first, s_last) in the range [first, last - (s_last - s_first)).

	// Elements are compared using operator==.
	template <typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator1 search(ForwardIterator1 first, ForwardIterator1 last,
	                        ForwardIterator2 s_first, ForwardIterator2 s_last)
	{
		for (;; ++first)
		{
			ForwardIterator1 it = first;
			for (ForwardIterator2 s_it = s_first;; ++it, ++s_it)
			{
				if (s_it == s_last)
					return first;
				if (it == last)
					return last;
				if (!(*it == *s_it))
					break;
			}
		}
	}

	// Elements are compared using the given binary predicate p.
	template <typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
	ForwardIterator1 search(ForwardIterator1 first, ForwardIterator1 last,
	                        ForwardIterator2 s_first, ForwardIterator2 s_last,
	                        BinaryPredicate p)
	{
		for (;; ++first)
		{
			ForwardIterator1 it = first;
			for (ForwardIterator2 s_it = s_first;; ++it, ++s_it)
			{
				if (s_it == s_last)
					return first;
				if (it == last)
					return last;
				if (!p(*it, *s_it))
					break;
			}
		}
	}

	// Searches the range [first, last) for the first sequence of count identical elements,
	// each equal to the given value value.

	// Elements are compared using operator==.
	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T &value)
	{
		for (;; ++first)
		{
			ForwardIterator it = first;
			for (Size c = count;; --count, ++it)
			{
				if (c == 0)
					return first;
				if (it == last)
					return last;
				if (!(*first == value))
					break;
			}
		}
	}

	// Elements are compared using the given binary predicate p.
	template <typename ForwardIterator, typename Size, typename T, typename BinaryPredicate>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T &value,
	                         BinaryPredicate p)
	{
		for (;; ++first)
		{
			ForwardIterator it = first;
			for (Size c = count;; --c, ++it)
			{
				if (c == 0)
					return first;
				if (it == last)
					return last;
				if (!p(*first, value))
					break;
			}
		}
	}

	// Assigns the given value to the elements in the range [first, last).
	template <typename ForwardIterator, typename T>
	void fill(ForwardIterator first, ForwardIterator end, const T &value)
	{
		for (; first != end; ++first) *first = value;
	}

	// Assigns the given value to the first count elements in the range beginning
	// at first if count > 0. Does nothing otherwise
	template <typename OutputIterator, typename Size, typename T>
	OutputIterator fill_n(OutputIterator first, Size count, const T &value)
	{
		for (; count > 0; ++first, --count)
		{
			*first = value;
		}
		return first;
	}

	// Swaps the values of the elements the given iterators are pointing to.
	template <typename ForwardIterator1, typename ForwardIterator2>
	void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
	{
		using std::swap;
		swap(*a, *b);
	}

	/******************************* Minimum / Maximum operations ***********************************/
	template <typename T>
	const T &max(const T &a, const T &b)
	{
		return a < b ? b : a;
	}

	template <typename T, typename Compare>
	const T &max(const T &a, const T &b, Compare comp)
	{
		return comp(a, b) ? b : a;
	}

	// Finds the greatest element in the range [first, last).

	// Elements are compared using operator<.
	template <typename ForwardIterator>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
	{
		if (first == last) return first;

		ForwardIterator largest = first;
		while (++first != last)
		{
			if (*largest < *first) largest = first;
		}
		return largest;
	}

	// Elements are compared using the given binary comparison function comp.
	template <typename ForwardIterator, typename Compare>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp)
	{
		if (first == last) return first;

		ForwardIterator largest = first;
		while (++first != last)
		{
			if (comp(*largest, *first)) largest = first;
		}
		return largest;
	}

	// Returns the smaller of the given values.
	template <typename T>
	const T &min(const T &a, const T &b)
	{
		return a < b ? a : b;
	}

	template <typename T, typename Compare>
	const T &min(const T &a, const T &b, Compare comp)
	{
		return comp(a, b) ? a : b;
	}

	// Finds the smallest element in the range [first, last).

	// Elements are compared using operator >.
	template <typename ForwardIterator>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
	{
		if (first == last) return first;

		ForwardIterator smallest = first;
		while (++first != last)
		{
			if (*first < *smallest) smallest = first;
		}
		return smallest;
	}

	// Elements are compared using the given binary comparison function comp.
	template <typename ForwardIterator, typename Compare>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp)
	{
		if (first == last) return first;

		ForwardIterator smallest = first;
		while (++first != last)
		{
			if (comp(*first, *smallest)) smallest = first;
		}
		return smallest;
	}

	// Returns the lowest and the greatest of the given values.

	// Returns references to the smaller and the greater of a and b.
	template <typename T>
	std::pair<const T&, const T&> minmax(const T &a, const T &b)
	{
		return (a < b) ? std::pair<const T&, const T&> (a, b)
		       : std::pair<const T&, const T&> (b, a);
	}

	template <typename T, typename Compare>
	std::pair<const T&, const T&> minmax(const T &a, const T &b, Compare comp)
	{
		return comp(a, b) ? std::pair<const T&, const T&> (a, b)
		       : std::pair<const T&, const T&> (b, a);
	}


	// Checks if the first range [first1, last1) is lexicographically less than the
	// second range [first2, last2).

	// Elemnents are compared using <
	template <typename InputIterator1, typename InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
	                             InputIterator2 first2, InputIterator2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (*first1 < *first2) return true;
			if (*first1 > *first2) return false;
		}
		return first1 == last1 && first2 != last2;
	}

	// Elements are compared using the given binary comparison function comp.
	template <typename InputIterator1, typename InputIterator2, typename Compare>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
	                             InputIterator2 first2, InputIterator2 last2,
	                             Compare comp)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (comp(*first1, *first2)) return true;
			if (comp(*first2, *first1)) return false;
		}
		return first1 == last1 && first2 != last2;
	}

	/******************************************* partitioning operations ************************************/

	// returns true if all elements in the range [first, last) that satify the predicate
	// p appear before all elements that don't. Also returns true if [first, last) is empty.
	template <typename InputIterator, typename UnaryPredicate>
	bool is_partitioned(InputIterator first, InputIterator last,
	                    UnaryPredicate p)
	{
		for (; first != last; ++first)
			if (!p(*first))
				break;
		for (; first != last; ++first)
			if (p(first))
				return false;
		return true;
	}

	// partition the range [first, last) so that unary predicate p returns true for
	// every element in range [first, i) while false in range [i, last), i is returned
	// after the function is called.
	template <typename ForwardIterator, typename UnaryPredicate>
	ForwardIterator partition(ForwardIterator first, ForwardIterator last,
	                          UnaryPredicate p)
	{
		for (ForwardIterator it = first; it != last; ++it)
		{
			if (p(*it))
			{
				iter_swap(first, it);
				++first;
			}
		}
		return first;
	}

	/******************************************* Set operations *****************************************/

	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
	                     InputIterator2 first2, InputIterator2 last2,
	                     OutputIterator d_first)
	{
		for (; first1 != last1; ++d_first)
		{
			if (first2 == last2)
				return copy(first1, last1, d_first);

			if (!(*first2 < *first1))
			{
				*d_first = *first1;
				++first1;
			}
			else
			{
				*d_first = *first2;
				++first2;
			}
		}

		return copy(first2, last2, d_first);
	}

	template <typename InputIterator1, typename InputIterator2,
	          typename OutputIterator,
	          typename Compare>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
	                     InputIterator2 first2, InputIterator2 last2,
	                     OutputIterator d_first,
	                     Compare comp)
	{
		for (; first1 != last1; ++d_first)
		{
			if (first2 == last2)
				return copy(first1, last1, d_first);

			if (!comp(*first2, *first1))
			{
				*d_first = *first1;
				++first1;
			}
			else
			{
				*d_first = *first2;
				++first2;
			}
		}
		return copy(first2, last2, d_first);
	}

	// Returns true if every element from the sorted range [first2, last2) is
	// found within the sorted range [first1, last1). Also returns true if
	// [first2, last2) is empty.

	// Both ranges must be sorted with operator<.
	template <typename InputIterator1, typename InputIterator2>
	bool includes(InputIterator1 first1, InputIterator1 last1,
	              InputIterator2 first2, InputIterator2 last2)
	{
		for (; first2 != last2; ++first1)
		{
			if (first1 == last1 || *first2 < *first1) return false;
			if (!(*first1 < *first2)) ++first2;
		}
		return true;
	}

	// Both ranges must be sorted with the given comparison function comp.
	template <typename InputIterator1, typename InputIterator2,
	          typename Compare>
	bool includes(InputIterator1 first1, InputIterator1 last1,
	              InputIterator2 first2, InputIterator2 last2,
	              Compare comp)
	{
		for (; first2 != last2; ++first1)
		{
			if (first1 == last1 || comp(*first2, *first1)) return false;
			if (!(comp(*first1, *first2))) ++first2;
		}
		return true;
	}

	// Constructs a sorted range beginning at d_first consisting of all elements
	// present in one or both sorted ranges [first1, last1) and [first2, last2).
	template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
	                         InputIterator2 first2, InputIterator2 last2,
	                         OutputIterator d_first)
	{

	}




	template <typename ForwardIterator, typename T>
	ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T &value)
	{
		for (ForwardIterator it = first; it != last; ++it)
		{
			if (!(*it == value))
			{
				*first = *it;
				++first;
			}
		}
		return first;
	}



	template <typename InputIterator, typename T, typename OutputIterator>
	OutputIterator remove_copy(InputIterator first, InputIterator last,
	                           const T &value,
	                           OutputIterator d_first)
	{
		for (; first != last; ++first)
		{
			if (!(*first == value))
			{
				*d_first = *first;
				++d_first;
			}
		}
		return d_first;
	}


	template <typename ForwardIterator, typename UnaryPredicate>
	ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, UnaryPredicate p)
	{
		for (ForwardIterator it = first; it != last; ++it)
		{
			if (!p(*it))
			{
				*first = *it;
				++first;
			}
		}
		return first;
	}


	template <typename InputIterator, typename OutputIterator, typename UnaryPredicate>
	OutputIterator remove_copy_if(InputIterator first, InputIterator last,
	                              OutputIterator d_first,
	                              UnaryPredicate p)
	{
		for (; first != last; ++first)
		{
			if (!p(*first))
			{
				*d_first = *first;
				++d_first;
			}
		}
		return d_first;
	}


	template <typename ForwardIterator, typename T>
	void replace(ForwardIterator first, ForwardIterator last,
	             const T &old_value,
	             const T &new_value)
	{
		for (; first != last; ++first)
		{
			if (*first == old_value)
				*first = new_value;
		}
	}

	template <typename InputIterator, typename OutputIterator, typename T>
	OutputIterator replace_copy(InputIterator first, InputIterator last,
	                            OutputIterator d_first,
	                            const T &old_value,
	                            const T &new_value)
	{
		for (; first != last; ++first, ++d_first)
			*d_first = (*first == old_value) ? new_value : *first;
		return d_first;
	}


	template <typename ForwardIterator, typename T, typename UnaryPredicate>
	void replace_if(ForwardIterator first, ForwardIterator last,
	                UnaryPredicate p,
	                const T &new_value)
	{
		for (; first != last; ++first)
		{
			if (p(*first))
				*first = new_value;
		}
	}


	template <typename InputIterator, typename OutputIterator, typename T, typename UnaryPredicate>
	OutputIterator replace_copy_if(InputIterator first, InputIterator last,
	                               OutputIterator d_first,
	                               const T &new_value,
	                               UnaryPredicate p)
	{
		for (; first != last; ++first, ++d_first)
			*d_first = p(*first) ? new_value : *first;
		return d_first;
	}


	template <typename BidirectionalIterator>
	void reverse(BidirectionalIterator first, BidirectionalIterator last)
	{
		while (first != last && first != --last)
		{
			ministl::iter_swap(first, last);
			++first;
		}
	}


	template <typename BidirectionalIterator, typename OutputIterator>
	OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator d_first)
	{
		for (; first != last; ++d_first)
		{
			--last;
			*d_first = *last;
		}
		return d_first;
	}

	template <typename ForwardIterator>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last)
	{
		if (first == last)
			return first;

		ForwardIterator it = first;
		for (++it; it != last; ++it)
		{
			if (!(*first == *it))
				*++first = *it;
		}
		return ++first;
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator d_first)
	{
		if (first == last)
			return d_first;

		typename iterator_traits<InputIterator>::value_type value = *first;
		*d_first = value;

		for (++first; first != last; ++first)
		{
			if (!(*first == value))
			{
				value = *first;
				*++d_first = value;
			}
		}

		return ++d_first;
	}


	template <typename RandomAccessIterator, typename T>
	RandomAccessIterator lower_bound(RandomAccessIterator first, RandomAccessIterator last,
	                                 const T &value,
	                                 random_access_iterator_tag)
	{
		typename iterator_traits<RandomAccessIterator>::difference_type len = last - first, step;
		RandomAccessIterator it;

		while (len > 0)
		{
			step = len >> 1;
			it = first + step;

			if (*it < value)
			{
				first = ++it;
				len -= step + 1;
			}
			else
				len = step;
		}

		return first;
	}


	template <typename ForwardIterator, typename T>
	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
	                            const T &value,
	                            forward_iterator_tag)
	{
		// do a linear search
		for (; first != last; ++first)
		{
			if (!(*first < value))
				return first;
		}
		return last;
	}


	template <typename ForwardIterator, typename T>
	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T &value)
	{
		return lower_bound(first, last, value,
		                   typename iterator_traits<ForwardIterator>::iterator_category());
	}


	template <typename RandomAccessIterator, typename T>
	RandomAccessIterator upper_bound(RandomAccessIterator first, RandomAccessIterator last,
	                                 const T &value,
	                                 random_access_iterator_tag)
	{
		typename iterator_traits<RandomAccessIterator>::difference_type len = last - first, step;
		RandomAccessIterator it;

		while (len > 0)
		{
			step = len >> 1;
			it = first + step;

			if (value < *it)
				len = step;
			else
			{
				first = ++it;
				len -= step + 1;
			}
		}

		return first;
	}


	template <typename ForwardIterator, typename T>
	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
	                            const T & value,
	                            forward_iterator_tag)
	{
		// do a linear search
		for (; first != last; ++first)
		{
			if (value < *first)
				return first;
		}
		return last;
	}



	template <typename ForwardIterator, typename T>
	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T & value)
	{
		return upper_bound(first, last, value,
		                   typename iterator_traits<ForwardIterator>::iterator_category());
	}


	template <typename ForwardIterator, typename T>
	bool binary_search(ForwardIterator first, ForwardIterator last, const T & value)
	{
		ForwardIterator it = lower_bound(first, last, value);
		return it != last && !(value < *it);
	}


	template <typename BidirectionalIterator>
	bool next_permutation(BidirectionalIterator first, BidirectionalIterator last)
	{

	}


	template <typename ForwardIterator, typename T>
	std::pair<ForwardIterator, ForwardIterator>
	equal_range(ForwardIterator first, ForwardIterator last, const T &value, forward_iterator_tag)
	{
		std::pair<ForwardIterator, ForwardIterator> result;

		for (; first != last && *first < value; ++first)
			;

		result.first = first;

		for (; first != last && !(value < *first); ++first)
			;

		result.second = first;

		return result;
	}


	template <typename RandomAccessIterator, typename T>
	std::pair<RandomAccessIterator, RandomAccessIterator>
	equal_range(RandomAccessIterator first, RandomAccessIterator last, const T &value, random_access_iterator_tag)
	{
		typename iterator_traits<RandomAccessIterator>::difference_type len = last - first, step;
		RandomAccessIterator it, left, right;

		while (len > 0)
		{
			step = len >> 1;
			it = first + step;

			if (*it < value)
			{
				first = ++it;
				len -= step + 1;
			}
			else if (value < *it)
				len = step;
			else
			{
				left = lower_bound(first, it, value);
				right = upper_bound(it, last, value);
				return std::make_pair(left, right);
			}
		}
		return std::make_pair(first, first);
	}


	template <typename ForwardIterator, typename T>
	std::pair<ForwardIterator, ForwardIterator>
	equal_range(ForwardIterator first, ForwardIterator last, const T &value)
	{
		return equal_range(first, last, value,
		                   typename iterator_traits<ForwardIterator>::iterator_category());
	}


	template <typename InputIterator, typename OutputIterator, typename UnaryFunction>
	OutputIterator transform(InputIterator first, InputIterator last,
	                         OutputIterator d_first,
	                         UnaryFunction f)
	{
		for (; first != last; ++first, ++d_first)
			*d_first = f(*first);
		return d_first;
	}


	template <typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
	                             ForwardIterator2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
			iter_swap(first1, first2);
		return first2;
	}
}

#endif