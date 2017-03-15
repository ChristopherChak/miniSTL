#include <cstddef>
#include <cstdlib>

namespace ministl
{
	class malloc_alloc
	{
	public:
		static void *allocate(size_t n);
		static void deallocate(void *p, size_t n);
	};

	void *malloc_alloc::allocate(size_t n)
	{
		void *p = std::malloc(n);
		if (!p)
			;
		return p;
	}

	void malloc_alloc::deallocate(void *p, size_t)
	{
		std::free(p);
	}

	template <typename T, typename Alloc>
	class simple_alloc
	{
	public:
		static T *allocate(size_t n);
		static T *allocate();
		static void deallocate(T *p, size_t n);
		static void deallocate(T *p);

	};

	template <typename T, typename Alloc>
	T *simple_alloc<T, Alloc>::allocate(size_t n)
	{
		return (T*)(Alloc::allocate(n * sizeof(T)));
	}

	template <typename T, typename Alloc>
	T *simple_alloc<T, Alloc>::allocate()
	{
		return (T*)Alloc::allocate(sizeof(T));
	}

	template <typename T, typename Alloc>
	void simple_alloc<T, Alloc>::deallocate(T *p, size_t n)
	{
		Alloc::deallocate(p, n * sizeof(T));
	}

	template <typename T, typename Alloc>
	void simple_alloc<T, Alloc>::deallocate(T *p)
	{
		Alloc::deallocate(p, sizeof(T));
	}

	#ifndef MINISTLFree
	   #define MINISTLFree(allocator, p, size) (allocator).deallocate((void*)(p), (size))
	#endif


	typedef malloc_alloc alloc;

	template <typename Alloc, typename Integer>
	void *allocate_memory(Alloc &alloc, Integer n)
	{
		return alloc.allocate((size_t)n);
	}

}