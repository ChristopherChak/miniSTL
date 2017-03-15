/************************************* Heap operations *********************************/
#include <iostream>
#include <vector>
// Examines the range [first, last) and finds the largest range beginning
// at first which is a max heap.

// Elements are compared using operator<.
template <typename RandomIt>
inline RandomIt
is_heap_until(RandomIt first, RandomIt last)
{
	using difference_t = typename std::iterator_traits<RandomIt>::difference_type;
	difference_t N = std::distance(first, last);
	for (difference_t i = 1; i < N; ++i)
	{
		if (*(first + (i - 1) / 2) < * (first + i)) return first + i;
	}
	return last;
}

// Elements are compared using the given binary comparison function comp.
template <typename RandomIt, typename Compare>
inline RandomIt
is_heap_until(RandomIt first, RandomIt last, Compare comp)
{
	using difference_t = typename std::iterator_traits<RandomIt>::difference_type;
	difference_t N = std::distance(first, last);
	for (difference_t i = 1; i < N; ++i)
	{
		if (comp(*(first + (i - 1) / 2), *(first + i))) return first + i;
	}
	return last;
}

// Checks if the elements in range [first, last) are a max heap.
// Elements are compared using operator<.
template <typename RandomIt>
inline bool
is_heap(RandomIt first, RandomIt last)
{
	return is_heap_until(first, last) == last;
}

// Elements are compared using the given binary comparison function comp.
template <typename RandomIt, typename Compare>
inline bool
is_heap(RandomIt first, RandomIt last, Compare comp)
{
	return is_heap_until(first, last, comp) == last;
}

template <typename RandomIt, typename Distance, typename T>
inline void
sift_up(RandomIt first, Distance heap_size, Distance position, const T &value)
{
	Distance child_position = 2 * position + 2;

	for (; child_position < heap_size; child_position = 2 * position + 2)
	{
		if (*(first + child_position) < * (first + child_position - 1)) --child_position;

		if (value < * (first + child_position))
		{
			*(first + position) = * (first + child_position);
			position = child_position;
		}
		else break;
	}

	if (child_position == heap_size)
	{
		if (value < * (first + child_position - 1))
		{
			*(first + position) = *(first + child_position - 1);
			*(first + child_position - 1) = value;
		}
		else *(first + position) = value;
	}
	else *(first + position) = value;
}

template <typename RandomIt, typename Distance, typename T>
inline void
sift_down(RandomIt first, Distance top_position, Distance position, const T &value)
{
	for (Distance parent_postion = (position - 1) >> 1;
	        (position > top_position) && (*(first + parent_postion) < value);
	        parent_postion = (position - 1) >> 1)
	{
		*(first + position) = *(first + parent_postion);
		position = parent_postion;
	}
	*(first + position) = value;
}

template <typename RandomIt>
inline void
_push_heap(RandomIt first, RandomIt last)
{
	using difference_type = typename std::iterator_traits<RandomIt>::difference_type;
	using value_type      = typename std::iterator_traits<RandomIt>::value_type;

	const value_type tmp_bottom = *(last - 1);
	sift_down(first, (difference_type)0, (difference_type)(last - first - 1), tmp_bottom);
}

template <typename RandomIt>
inline void
_pop_heap(RandomIt first, RandomIt last)
{
	using difference_type = typename std::iterator_traits<RandomIt>::difference_type;
	using value_type      = typename std::iterator_traits<RandomIt>::value_type;

	const value_type tmp_bottom = *(last - 1);
	*(last - 1) = *first;
	sift_up(first, (difference_type)(last - first - 1), (difference_type)0, tmp_bottom);
}

template <typename RandomIt>
void make_heap(RandomIt first, RandomIt last)
{

}

///
template <typename RandomIt>
inline void sort_heap(RandomIt first, RandomIt last)
{
	for (; first < last - 1; --last)
	{
		pop_heap(first, last);
	}
}