#include <iterator>

namespace mini_stl
{
	// Fills the range [first, last) with sequentially increasing values, 
	// starting with value and repetitively evaluating ++value.
	template <typename ForwardItr, typename T>
	void iota(ForwardItr first, ForwardItr last, T value)
	{
		while (first != last) *first++ = value++;
	}

	// Computes the sum of the given value init and the elements in the range 
	// [first, last).

	// The first version uses operator+ to sum up the elements.
	template <typename InputItr, typename T>
	T accumulate(InputItr first, InputItr last, T init)
	{
		for (; first != last; ++first)
		{
			init = init + *first;
		}
		return init;
	}

	// The second version uses the given binary function op.
	template <typename InputItr, typename T, typename BinaryOperation>
	T accumulate(InputItr first, InputItr last, T init, BinaryOperation op)
	{
		for (; first != last; ++first)
		{
			init = op(init, *first);
		}
		return init;
	}

	// Computes inner product (i.e. sum of products) of the range [first1, last1)
	// and another range beginning at first2.

	// Products are calculated using operator* and sums are calculated using
	// operator+.
	template <typename InputItr1, typename InputItr2, typename T>
	T innner_product(InputItr1 first1, InputItr1 last1, InputItr2 first2, T init)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			init = init + (*first1) * (*first2);
		}
		return init;
	}

	// Products are calculated using op2 and sums are calculated using op1.
	template <typename InputItr1, typename InputItr2,
	          typename BinaryOperation1, typename BinaryOperation2,
	          typename T>
	T innner_product(InputItr1 first1, InputItr1 last1, InputItr2 first2,
	                 BinaryOperation1 op1, BinaryOperation2 op2,
	                 T init)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			init = op1(init, op2(*first1, *first2));
		}
		return init;
	}

	// Computes the differences between the second and the first of each 
	// adjacent pair of elements of the range [first, last) and writes 
	// them to the range beginning at d_first + 1. Unmodified copy of 
	// first is written to d_first.

	// Differences are calculated using operator-.
	template <typename InputItr, typename OutputIt>
	OutputIt adjacent_difference(InputItr first, InputItr last, OutputIt d_first)
	{
		if (first == last) return d_first;

		using value_t = typename std::iterator_traits<InputItr>::value_type;
		value_t acc = *first;
		*d_first = *first;

		while (++first != last)
		{
			value_t val = *first;
			*++d_first = val - acc;
			acc = std::move(val);
		}
		return ++d_first;
	}

	// Differences are calculated using the given binary function op.
	template <typename InputItr, typename OutputIt, typename BinaryOperation>
	OutputIt adjacent_difference(InputItr first, InputItr last, OutputIt d_first, BinaryOperation op)
	{
		if (first == last) return d_first;

		using value_t = typename std::iterator_traits<InputItr>::value_type;
		value_t acc = *first;
		*d_first = *first;

		while (++first != last)
		{
			value_t val = *first;
			*++d_first = op(val, acc);
			acc = std::move(val);
		}
		return ++d_first;
	}

	// Computes the partial sums of the elements in the subranges of
	// the range [first, last) and writes them to the range beginning
	// at d_first.

	// The first version uses operator+ to sum up the elements.
	template <typename InputItr, typename OutputIt>
	OutputIt partial_sum(InputItr first, InputItr last, OutputIt d_first)
	{
		if (first == last) return d_first;

		typename std::iterator_traits<InputItr>::value_type sum = *first;
		*d_first = sum;

		while (++first != last)
		{
			sum = *first + sum;
			*++d_first = sum;
		}
		return ++d_first;
	}

	// The second version uses the given binary function op.
	template <typename InputItr, typename OutputIt, typename BinaryOperation>
	OutputIt partial_sum(InputItr first, InputItr last, OutputIt d_first,
	                     BinaryOperation op)
	{
		if (first == last) return d_first;

		typename std::iterator_traits<InputItr>::value_type sum = *first;
		*d_first = sum;

		while (++first != last)
		{
			sum = op(*first, sum);
			*++d_first = sum;
		}
		return ++d_first;
	}

}