#include <deque>
#include <iostream>

namespace ministl
{
	template <typename T, typename Container = std::deque<T>>
	class stack
	{
	public:
		using container_type   = Container;
		using value_type       = typename Container::value_type;
		using size_type        = typename Container::size_type;
		using reference        = typename Container::reference;
		using const_reference  = typename Container::const_reference;

		explicit stack(Container&& cont = Container());
		explicit stack(const Container &cont);
		stack(const stack &other);
		stack(stack &&other);

		reference             top();
		const_reference       top()const;

		bool                  empty()const;
		size_type             size()const;

		void                  push(const value_type &val);
		void                  push(value_type &&val);
		void                  pop();
		void                  swap(stack &other);

	protected:
		Container c;
	};

	template <typename T, typename Container>
	stack<T, Container>::stack(Container &&cont)
		: c(std::move(cont))
	{}

	template <typename T, typename Container>
	stack<T, Container>::stack(const Container &cont)
		: c(cont)
	{}

	template <typename T, typename Container>
	stack<T, Container>::stack(const stack &other)
		: c(other.c)
	{}

	template <typename T, typename Container>
	stack<T, Container>::stack(stack &&other)
		: c(std::remove(other.c))
	{}

	template <typename T, typename Container>
	inline typename stack<T, Container>::reference
	stack<T, Container>::top()
	{
		return c.back();
	}

	template <typename T, typename Container>
	inline typename stack<T, Container>::const_reference
	stack<T, Container>::top()const
	{
		return c.back();
	}

	template <typename T, typename Container>
	inline bool
	stack<T, Container>::empty()const
	{
		return c.empty();
	}

	template <typename T, typename Container>
	inline typename stack<T, Container>::size_type
	stack<T, Container>::size()const
	{
		return c.size();
	}

	template <typename T, typename Container>
	inline void
	stack<T, Container>::push(const value_type &val)
	{
		c.push_back(val);
	}

	template <typename T, typename Container>
	inline void
	stack<T, Container>::push(value_type &&val)
	{
		c.push_back(std::move(val));
	}

	template <typename T, typename Container>
	inline void
	stack<T, Container>::pop()
	{
		c.pop_back();
	}

	template <typename T, typename Container>
	inline void
	stack<T, Container>::swap(stack &other)
	{
		using std::swap;
		swap(c, other.c);
	}
}

int main(int argc, char const *argv[])
{
	ministl::stack<int>s, s1;
	s.push(3);
	s.push(4);
	s.pop();
	std::cout << s.top() << s.size() << s.empty();
	s.swap(s1);
	std::cout << s1.top() << s1.size() << s1.empty();
	return 0;
}