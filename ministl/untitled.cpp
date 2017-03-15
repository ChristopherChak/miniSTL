#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>
#include <string>
#include <forward_list>
#include <array>
// #include "vector.h"

using std::array;
using std::cout;

/*template <typename T>
void ClockWisePrintArray(const vector<vector<T>> &a, int n, int m)
{
	vector<T> result;

	for (size_t row = 0; row <= (N - 1) >> 1; ++row)
	{
		size_t len = N - (row << 1);

		for (size_t j = row; j < row + len - 1; ++j)
			cout << a[row][j] << ' ';

		for (size_t i = row; i < row + len - 1; ++i)
			cout << a[i][row + len - 1] << ' ';

		for (size_t j = row + len - 1; j > row; --j)
			cout << a[row + len - 1][j] << ' ';

		for (size_t i = row + len - 1; i > row; --i)
			cout << a[i][row] << ' ';
	}

	if (N & 1)
		cout << a[N>>1][N >> 1];
}

template <typename RandomAccessIterator>
typename std::iterator_traits<RandomAccessIterator>::difference_type
Merge(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last)
{
	typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;

	typename std::iterator_traits<RandomAccessIterator>::difference_type n = 0;
	std::vector<value_type> v(first, last);

	auto it1 = v.begin();
	auto it2 = v.begin() + (middle - first);
	auto it3 = first;

	while (it2 != v.end())
	{
		if (it1 == (v.begin() + (middle - first)))
		{
			std::copy(it2, v.end(), it3);
			return n;
		}

		if (*it2 < *it1)
		{
			*it3++ = *it2++;
			++n;
		}
		else
			*it3++ = *it1++;
	}

	std::copy(it1, v.begin() + (middle - first), it3);
	n += (v.begin() + (middle - first) - it1);
	return n;
}

template <typename RandomAccessIterator>
typename std::iterator_traits<RandomAccessIterator>::difference_type
CountInversedPair(RandomAccessIterator first, RandomAccessIterator last)
{
	if (first == last || last - first == 1)
		return 0;

	RandomAccessIterator middle = first + ((last - first) >> 1);

	auto l = CountInversedPair(first, middle);
	auto r = CountInversedPair(middle, last);
	auto t = Merge(first, middle, last);

	return l + r + t;
}*/

/*class Solution
{
public:
	int MoreThanHalfNum_Solution(vector<int> numbers)
	{
		nth_element(numbers.begin(), numbers.begin() + numbers.size() / 2, numbers.end());
		return *(numbers.begin() + numbers.size() / 2);
	}
};*/

class Solution {
public:
	string LeftRotateString(string str, int n)
	{
		if (str.empty())
			return "";
		n %= str.size();
		reverse(str.begin(), str.begin() + n);
		reverse(str.begin() + n, str.end());
		reverse(str.begin(), str.end());
		return str;
	}
};

template <typename RandomAccessIterator, typename UnaryPredicate>
void partition(ForwardIterator first, ForwardIterator last, UnaryPredicate p)
{

}





int main(int argc, char const *argv[])
{
	array<int, 3> a = {1, 2, 3};
	cout << CountInversedPair(a.begin(), a.end()) << std::endl;
	for (auto i : a)
	{
		cout << i << ' ';
	}

}
