#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>

namespace ministl
{
	template <typename T,
	          typename Container = std::vector<T>,
	          typename Compare = std::less<typename Container::value_type>>
	class priority_queue
	{
	public:
		using container_type   = Container;
		using value_compare    = Compare;
		using value_type       = typename Container::value_type;
		using size_type        = typename Container::size_type;
		using reference        = typename Container::reference;
		using const_reference  = typename Container::const_reference;

		priority_queue(const Compare &comp, const Container &cont);
		explicit priority_queue(const Compare &comp = Compare(), Container && cont = Container());
		priority_queue(const priority_queue &other);
		priority_queue(priority_queue &&other);

		const_reference top()const;

		bool            empty()const;
		size_type       size()const;

		void            push(const value_type &x);
		void            push(value_type &&x);
		void            emplace();
		void            pop();
		void            swap(priority_queue &other);

	protected:
		Compare   comp;
		Container c;
	};

	template <typename T, typename Container, typename Compare>
	priority_queue<T, Container, Compare>::priority_queue(const Compare &comp, const Container &cont)
		: comp{comp}, c{cont}
	{}

	template <typename T, typename Container, typename Compare>
	priority_queue<T, Container, Compare>::priority_queue(const Compare &comp, Container &&cont)
		: comp{comp}, c{std::move(cont)}
	{}

	template <typename T, typename Container, typename Compare>
	priority_queue<T, Container, Compare>::priority_queue(const priority_queue &other)
		: comp{other.c}, c{other.c}
	{}

	template <typename T, typename Container, typename Compare>
	priority_queue<T, Container, Compare>::priority_queue(priority_queue &&other)
		: comp{std::move(other.c)}, c{std::move(other.c)}
	{}

	template <typename T, typename Container, typename Compare>
	inline typename priority_queue<T, Container, Compare>::const_reference
	priority_queue<T, Container, Compare>::top()const
	{
		return c.front();
	}

	template <typename T, typename Container, typename Compare>
	inline bool
	priority_queue<T, Container, Compare>::empty()const
	{
		return c.empty();
	}

	template <typename T, typename Container, typename Compare>
	inline typename priority_queue<T, Container, Compare>::size_type
	priority_queue<T, Container, Compare>::size()const
	{
		return c.size();
	}

	template <typename T, typename Container, typename Compare>
	inline void
	priority_queue<T, Container, Compare>::push(const value_type &x)
	{
		c.push_back(x);
		std::push_heap(c.begin(), c.end(), comp);
	}

	template <typename T, typename Container, typename Compare>
	inline void
	priority_queue<T, Container, Compare>::push(value_type &&x)
	{
		c.push_back(std::move(x));
		std::push_heap(c.begin(), c.end(), comp);
	}

	template <typename T, typename Container, typename Compare>
	inline void
	priority_queue<T, Container, Compare>::pop()
	{
		std::pop_heap(c.begin(), c.end(), comp);
		c.pop_back();
	}

	template <typename T, typename Container, typename Compare>
	inline void priority_queue<T, Container, Compare>::swap(priority_queue &other)
	{
		using std::swap;
		swap(c, other.c);
		swap(comp, other.comp);
	}

}


int main(int argc, char const *argv[])
{
	return 0;
}