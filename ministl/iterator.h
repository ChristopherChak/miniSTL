#ifndef ITERATOR_H
#define ITERATOR_H

#include <cstddef>

namespace ministl
{
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	template< class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
		typedef typename Iterator::iterator_category iterator_category;
	};

	template <typename T>
	struct iterator_traits<T*>
	{
		typedef ptrdiff_t difference_type;
		typedef  T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template <typename T>
	struct iterator_traits<const T*>
	{
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef const T* pointer;
		typedef const T& reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template <typename InputIterator, typename Distance>
	void advance(InputIterator &it, Distance distance, input_iterator_tag)
	{
		for (; distance > 0; --distance)
			++it;
	}

	template <typename BidirectionalIterator, typename Distance>
	void advance(BidirectionalIterator &it, Distance distance, bidirectional_iterator_tag)
	{
		if (distance > 0)
		{
			for (; distance > 0; --distance)
				++it;
		}
		else
		{
			for (; distance < 0; ++distance)
				--it;
		}
	}

	template <typename RandomAccessIterator, typename Distance>
	void advance(RandomAccessIterator &it, Distance distance, random_access_iterator_tag)
	{
		it += distance;
	}

	template <typename InputIterator, typename Distance>
	void advance(InputIterator &it, Distance distance)
	{
		return advance(it, distance, typename iterator_traits<InputIterator>::iterator_category());
	}

	template <typename InputIterator>
	typename iterator_traits<InputIterator>::difference_type
	distance_impl(InputIterator first, InputIterator last, input_iterator_tag)
	{
		typename iterator_traits<InputIterator>::difference_type distance = 0;
		for (; first != last; ++first)
			++distance;
		return distance;
	}

	template <typename RandomAccessIterator>
	typename iterator_traits<RandomAccessIterator>::difference_type
	distance_impl(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
	{
		return last - first;
	}

	template <typename InputIterator>
	typename iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last)
	{
		return distance_impl(first, last, typename iterator_traits<InputIterator>::iterator_category());
	}

	template <typename ForwardIterator>
	ForwardIterator next(ForwardIterator it, typename iterator_traits<ForwardIterator>::difference_type n = 1)
	{
		for (; n > 0; --n)
			++it;
		return it;
	}

	template <typename Iterator>
	class reverse_iterator
	{
	public:
		// Note that before C++17, these are all inherited from std::iterator
		typedef typename iterator_traits<Iterator>::value_type value_type;
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::difference_type difference_type;
		typedef typename iterator_traits<Iterator>::reference reference;
		typedef typename iterator_traits<Iterator>::pointer pointer;
		typedef reverse_iterator this_type;

		reverse_iterator();
		explicit reverse_iterator(Iterator x);
		template <typename U>
		reverse_iterator(const reverse_iterator<U> &other);
		template <typename U>
		reverse_iterator &operator=(const reverse_iterator<U> &other);

		Iterator base()const;

		reference operator*()const;
		pointer operator->()const;
		reference operator[](difference_type n)const;

		this_type &operator++();
		this_type operator++(int);
		this_type &operator--();
		this_type operator--(int);
		this_type operator+(difference_type n)const;
		this_type &operator+=(difference_type n);
		this_type operator-(difference_type n)const;
		this_type &operator-=(difference_type n);

	protected:
		Iterator currrent;
	};

	template <typename Iterator>
	reverse_iterator<Iterator>::reverse_iterator()
	{
		// empty
	}


	template <typename Iterator>
	reverse_iterator<Iterator>::reverse_iterator(Iterator x)
		: currrent(x)
	{
		// empty
	}


	template <typename Iterator>
	template <typename U>
	reverse_iterator<Iterator>::reverse_iterator(const reverse_iterator<U> &other)
		: currrent(other.currrent)
	{
		// empty
	}


	template <typename Iterator>
	template <typename U>
	reverse_iterator<Iterator>&
	reverse_iterator<Iterator>::operator=(const reverse_iterator<U> &other)
	{
		currrent = other.base();
		return *this;
	}


	template <typename Iterator>
	Iterator reverse_iterator<Iterator>::base()const
	{
		return currrent;
	}


	template <typename Iterator>
	typename reverse_iterator<Iterator>::reference
	reverse_iterator<Iterator>::operator*()const
	{
		Iterator temp = currrent;
		--temp;
		return *temp;
	}


	template <typename Iterator>
	typename reverse_iterator<Iterator>::pointer
	reverse_iterator<Iterator>::operator->()const
	{
		return &(operator*());
	}


	template <typename Iterator>
	typename reverse_iterator<Iterator>::reference
	reverse_iterator<Iterator>::operator[](difference_type n)const
	{
		return *(*this + n);
	}


	template <typename Iterator>
	typename reverse_iterator<Iterator>::this_type&
	reverse_iterator<Iterator>::operator++()
	{
		--currrent;
		return *this;
	}


	template <typename Iterator>
	typename reverse_iterator<Iterator>::this_type
	reverse_iterator<Iterator>::operator++(int)
	{
		this_type temp = *this;
		--currrent;
		return temp;
	}


	template <typename Iterator>
	typename reverse_iterator<Iterator>::this_type&
	reverse_iterator<Iterator>::operator--()
	{
		++currrent;
		return *this;
	}


	template <typename Iterator>
	typename reverse_iterator<Iterator>::this_type
	reverse_iterator<Iterator>::operator--(int)
	{
		reverse_iterator temp = *this;
		++currrent;
		return temp;
	}


	template <typename Iterator>
	typename reverse_iterator<Iterator>::this_type
	reverse_iterator<Iterator>::operator-(difference_type n)const
	{
		this_type temp = *this;
		temp -= n;
		return temp;
	}


	template <typename Iterator>
	typename reverse_iterator<Iterator>::this_type&
	reverse_iterator<Iterator>::operator-=(difference_type n)
	{
		currrent += n;
		return *this;
	}


	template <typename Iterator>
	typename reverse_iterator<Iterator>::this_type
	reverse_iterator<Iterator>::operator+(difference_type n)const
	{
		this_type temp = *this;
		temp += n;
		return temp;
	}


	template <typename Iterator>
	typename reverse_iterator<Iterator>::this_type&
	reverse_iterator<Iterator>::operator+=(difference_type n)
	{
		currrent -= n;
		return *this;
	}


	template <typename IteratorA, typename IteratorB>
	bool operator==(const reverse_iterator<IteratorA> &lhs, const reverse_iterator<IteratorB> &rhs)
	{
		return lhs.base() == rhs.base();
	}


	template <typename IteratorA, typename IteratorB>
	bool operator!=(const reverse_iterator<IteratorA> &lhs, const reverse_iterator<IteratorB> &rhs)
	{
		return lhs.base() != rhs.base();
	}


	template <typename IteratorA, typename IteratorB>
	bool operator<(const reverse_iterator<IteratorA> &lhs, const reverse_iterator<IteratorB> &rhs)
	{
		return lhs.base() > rhs.base();
	}


	template <typename IteratorA, typename IteratorB>
	bool operator<=(const reverse_iterator<IteratorA> &lhs, const reverse_iterator<IteratorB> &rhs)
	{
		return lhs.base() >= rhs.base();
	}

	template <typename IteratorA, typename IteratorB>
	bool operator>(const reverse_iterator<IteratorA> &lhs, const reverse_iterator<IteratorB> &rhs)
	{
		return lhs.base() < rhs.base();
	}


	template <typename IteratorA, typename IteratorB>
	bool operator>=(const reverse_iterator<IteratorA> &lhs, const reverse_iterator<IteratorB> &rhs)
	{
		return lhs.base() <= rhs.base();
	}

	/*template <typename T, typename Distance>
	class istream_itertor
	{
	public:
		using iterator_category = input_iterator_tag;
		using value_type        = T;
		using difference_type   = ptrdiff_t;
		using pointer           = const T*;
		using reference         = const T&;

		istream_itertor();
		istream_itertor(std::istream &istream);

		reference operator*()const;
		pointer   operator->()const;

		istream_itertor &operator++();
		istream_itertor  operator++(int);

	private:
		void read();

		std::istream *istream;
		T value;
	};

	template <typename T>
	istream_itertor<T>::istream_itertor()
		: istream(nullptr)
	{
		// empty
	}

	template <typename T>
	istream_itertor<T>::istream_itertor(std::istream &istream)
		: istream(&istream)
	{
		// read immmediately from istream
		read();
	}

	template <typename T>
	typename istream_itertor<T>::reference
	istream_itertor<T>::operator*()const
	{
		return value;
	}

	template <typename T>
	typename istream_itertor<T>::pointer
	istream_itertor<T>::operator->()const
	{
		return &value;
	}

	template <typename T>
	istream_itertor<T>& istream_itertor<T>::operator++()
	{
		read();
		return *this;
	}

	template <typename T>
	istream_itertor<T> istream_itertor<T>::operator++(int)
	{
		istream_itertor temp = *this;
		read();
		return temp;
	}

	template <typename T>
	void istream_itertor<T>::read()
	{
		if (istream && *istream)
			*istream >> value;
	}

	template <typename T>
	bool operator==(const istream_itertor<T> &lhs, const istream_itertor<T> &rhs)
	{
		return lhs.istream == rhs.istream;
	}

	template <typename T>
	bool operator!=(const istream_itertor<T> &lhs, const istream_itertor<T> &rhs)
	{
		return !(lhs == rhs);
	}*/
}

#endif // iterator.h