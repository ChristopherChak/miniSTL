#include "iterator.h"
#include "type_traits.h"

namespace ministl
{
	template <typename T, typename U>
	void construct(T *p, const U &value)
	{
		new(p)U(value);
	}

	template <typename T>
	void destroy(T *p)
	{
		p->~T();
	}

	template <typename ForwardIterator>
	void destroy_impl(ForwardIterator first, ForwardIterator last, true_type)
	{
		// empty
	}

	template <typename ForwardIterator>
	void destroy_impl(ForwardIterator first, ForwardIterator last, false_type)
	{
		for (; first != last; ++first)
			destroy(&*first);
	}

	template <typename ForwardIterator>
	void destroy(ForwardIterator first, ForwardIterator last)
	{
		typedef typename iterator_traits<ForwardIterator>::value_type value_type;
		destroy_impl(first, last,
		             typename type_traits<value_type>::is_trivially_destructible());
	}
}