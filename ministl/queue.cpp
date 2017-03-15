#include <deque>
#include <iostream>

namespace ministl
{
	template <typename T, typename Container = std::deque<T>>
	class queue
	{
	public:
		using container_type   = Container;
		using value_type       = typename Container::value_type;
		using size_type        = typename Container::size_type;
		using reference        = typename Container::reference;
		using const_reference  = typename Container::const_reference;

		explicit queue(Container&& cont = Container());
		explicit queue(const Container &cont);
		queue(const queue &other);
		queue(queue &&other);

		reference             front();
		const_reference       front()const;
		reference             back();
		const_reference       back()const;

		bool                  empty()const;
		size_type             size()const;

		void                  push(const value_type &val);
		void                  push(value_type &&val);
		void                  pop();
		void                  swap(queue &other);

	protected:
		Container c;
	};

	template <typename T, typename Container>
	queue<T, Container>::queue(Container &&cont)
		: c(std::move(cont))
	{}

	template <typename T, typename Container>
	queue<T, Container>::queue(const Container &cont)
		: c(cont)
	{}

	template <typename T, typename Container>
	queue<T, Container>::queue(const queue &other)
		: c(other.c)
	{}

	template <typename T, typename Container>
	queue<T, Container>::queue(queue &&other)
		: c(std::remove(other.c))
	{}

	template <typename T, typename Container>
	inline typename queue<T, Container>::reference
	queue<T, Container>::front()
	{
		return c.front();
	}

	template <typename T, typename Container>
	inline typename queue<T, Container>::const_reference
	queue<T, Container>::front()const
	{
		return c.front();
	}

	template <typename T, typename Container>
	inline typename queue<T, Container>::reference
	queue<T, Container>::back()
	{
		return c.back();
	}

	template <typename T, typename Container>
	inline typename queue<T, Container>::const_reference
	queue<T, Container>::back()const
	{
		return c.back();
	}

	template <typename T, typename Container>
	inline bool
	queue<T, Container>::empty()const
	{
		return c.empty();
	}

	template <typename T, typename Container>
	inline typename queue<T, Container>::size_type
	queue<T, Container>::size()const
	{
		return c.size();
	}

	template <typename T, typename Container>
	inline void
	queue<T, Container>::push(const value_type &val)
	{
		c.push_back(val);
	}

	template <typename T, typename Container>
	inline void
	queue<T, Container>::push(value_type &&val)
	{
		c.push_back(std::move(val));
	}

	template <typename T, typename Container>
	inline void
	queue<T, Container>::pop()
	{
		c.pop_front();
	}

	template <typename T, typename Container>
	inline void
	queue<T, Container>::swap(queue &other)
	{
		using std::swap;
		swap(c, other.c);
	}
}

int main(int argc, char const *argv[])
{
	std::deque<int>dq{1, 2, 3};
	ministl::queue<int>q{dq};
	q.push(3);
	q.push(4);
	q.pop();
	std::cout << q.back() << q.front() << q.empty() << q.size();
	return 0;
}