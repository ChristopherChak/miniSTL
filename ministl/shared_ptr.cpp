#include <functional>
#include <iostream>

namespace ministl
{
	template <typename T>
	class shared_ptr;

	template <typename T, typename U>
	bool operator==(const shared_ptr<T> &lhs, const shared_ptr<U> &rhs)
	{
		return lhs.get() == rhs.get();
	}

	template <typename T, typename U>
	bool operator!=(const shared_ptr<T> &lhs, const shared_ptr<T> &rhs)
	{
		return !(lhs == rhs);
	}

	/*template <typename T, typename U>
	bool operator<(const shared_ptr<T> &lhs, const shared_ptr<U> &rhs)
	{
		return std::less<>
	}*/

	template <typename T, typename U>
	bool operator>(const shared_ptr<T> &lhs, const shared_ptr<U> &rhs)
	{
		return rhs < lhs;
	}

	template <typename T, typename U>
	bool operator<=(const shared_ptr<T> &lhs, const shared_ptr<U> &rhs)
	{
		return !(lhs > rhs);
	}

	template <typename T, typename U>
	bool operator>=(const shared_ptr<T> &lhs, const shared_ptr<U> &rhs)
	{
		return !(lhs < rhs);
	}

	template <typename T>
	class shared_ptr
	{
	public:
		shared_ptr();
		explicit shared_ptr(T *p);
		shared_ptr(const shared_ptr &other);
		shared_ptr &operator=(const shared_ptr &rhs);
		~shared_ptr();

		// modifiers
		void        swap(shared_ptr &rhs);

		// observers
		T*          get()const;
		T           &operator*()const;
		T           *operator->()const;
		long        use_count()const;
		bool        unqiue()const;
		explicit    operator bool()const;

	private:
		void        remove_from_this();

	private:
		T      *_value;
		long   *_use_count;
	};

	//
	// constructors, destructors, operator=
	//
	template <typename T>
	shared_ptr<T>::shared_ptr()
		: _value(nullptr), _use_count(new long(0))
	{}

	template <typename T>
	shared_ptr<T>::shared_ptr(const shared_ptr &other)
		: _value(other._value), _use_count(other._use_count)
	{
		if (_value)
		{
			++*_use_count;
		}
	}

	template <typename T>
	shared_ptr<T> &shared_ptr<T>::operator=(const shared_ptr &rhs)
	{
		if (rhs._value)
		{
			++*rhs._use_count;
		}
		remove_from_this();
		_value     = rhs._value;
		_use_count = rhs._use_count;
		return *this;
	}

	template <typename T>
	shared_ptr<T>::~shared_ptr()
	{
		remove_from_this();
	}

	// modifiers
	template <typename T>
	void shared_ptr<T>::swap(shared_ptr &rhs)
	{
		using std::swap;
		swap(_value, rhs._value);
		swap(_use_count, rhs._use_count);
	}

	// observers
	template <typename T>
	T *shared_ptr<T>::get()const
	{
		return _value;
	}

	template <typename T>
	inline T &shared_ptr<T>::operator*()const
	{
		return *_value;
	}

	template <typename T>
	T* shared_ptr<T>::operator->()const
	{
		return &operator*();
	}

	template <typename T>
	inline long shared_ptr<T>::use_count()const
	{
		return *_use_count;
	}

	template <typename T>
	inline bool shared_ptr<T>::unqiue()const
	{
		return use_count() == 1;
	}

	template <typename T>
	inline shared_ptr<T>::operator bool ()const
	{
		return _value != nullptr ? true : false;
	}

	// helper functions
	template <typename T>
	void shared_ptr<T>::remove_from_this()
	{
		if (!_value && *_use_count)
		{
			
		}
		else if (--*_use_count == 0)
		{
			
		}
	}
}


int main(int argc, char const *argv[])
{
	ministl::shared_ptr<int>p, q;
	if (p == q)
	{
		std::cout << sizeof(p);
	}

	return 0;
}


