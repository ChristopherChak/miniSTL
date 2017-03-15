namespace ministl
{
	enum
	{
		__ALIGN = 8
	};

	enum
	{
		__MAX_ALLOCATE_SIZE = 128
	};

	enum
	{
		__N_FREE_LIST = __MAX_ALLOCATE_SIZE / __ALIGN
	};

	/// The default allocator follows the style of how the 2nd level allocator
	/// in SGI STL manipulates memory. It only allocates memory less than
	/// __MAX_ALLOCATE_SIZE bytes which is defined above. For larger memmory
	/// requirments, the first level allocator allocator_malloc will be called.
	/// Note that this allocator ignores different types of allocated objects,
	/// and offers a uniform interface like malloc/free in C.
	template <bool threads>
	class __default_allocator
	{
	public:
		static void *allocate(size_t n);
		static void deallocate(void *p, size_t n);

	private:
		static size_t round_up(size_t n);
		static size_t freelist_index(size_t n);

		static void *refill(size_t n);
		static char *chunk_alloc(size_t n, int &n_node);

	private:

		union free_list_node
		{
			free_list_node *next;

		};

		static free_list_node *volatile free_list[__N_FREE_LIST];
		static char *free_begin;
		static char *free_end;
	};

	template <typename threads>
	__default_allocator<threads>::free_list_node *volatile __default_allocator<threads>::free_list[__N_FREE_LIST] = {0};

	template <typename threads>
	char *__default_allocator<threads>::free_begin = nullptr;

	template <typename threads>
	char *__default_allocator<threads>::free_end = nullptr;

	template <bool threads>
	void *__default_allocator<threads>::allocate(size_t n)
	{
		if (n > __MAX_ALLOCATE_SIZE)
			return allocator_malloc::allocate(n);

		free_list_node *volatile *index = free_list + freelist_index(n);
		free_list_node *result = *index;

		if (!result)
			return refill(round_up(n));

		*index = result->next;
		return result;
	}

	template <bool threads>
	void __default_allocator<threads>::deallocate(void *p, size_t n)
	{
		if (n > __MAX_ALLOCATE_SIZE)
			return allocator_malloc.deallocate(p, n);

		free_list_node *volatile *index = free_list + freelist_index(n);
		free_list_node *q = static_cast<free_list_node*>(p);
		q->next = *index;
		*index = q;
	}

	template <bool threads>
	void *__default_allocator<threads>::refill(size_t n)
	{
		// allocate 20 memory chunks by default.
		int n_node = 20;
		char *chunk = chunk_alloc(n, n_node);

		// when there is only one memeory chunk successfully allocated,
		// return directly the newly allocated chunk, note that there are
		// no spare chunks in free list.
		if (n_node == 1)
			return chunk;

		free_list_node *volatile *index = free_list + freelist_index(n);
		*index = static_cast<free_list_node*>(chunk + n);
		free_list_node *current, *next = *index;

		for (int i = 1;; ++i)
		{
			current = next;
			if (i + 1 == n_node)
			{
				current->next = 0;
				break;
			}
			else
			{
				next = static_cast<free_list_node*>(static_cast<char*>(next + n));
				current->next = next;
			}
		}

		return chunk;
	}

	template <bool threads>
	char *__default_allocator<threads>::chunk_alloc(size_t n, int &n_node)
	{
		size_t total_bytes = n * n_node;
		size_t bytes_left = free_end - free_begin;

		char *result;

		if (bytes_left >= total_bytes)
		{
			result = free_begin;
			free_begin += total_bytes;
			return result;
		}
		else if (bytes_left >= n)
		{
			n_node = bytes_left / n;
			result = free_begin;
			free_begin += n_node * n;
			return result;
		}

		if (bytes_left >= __ALIGN)
		{
			free_list *volatile *index = free_list + freelist_index(bytes_left);
			free_list *begin = static_cast<free_list_node*>(free_begin);
			begin->next = *index;
			*index = begin;
		}

		size_t bytes_to_alloc = total_bytes << 1 + round_up(heap_size >> 4);

		free_begin = static_cast<char*>(std::malloc(bytes_to_alloc));
		if (!free_begin)
		{
			free_list *volatile *index
			for (int i = n; i <= __MAX_ALLOCATE_SIZE; i += __ALIGN)
			{
				index = free_list + freelist_index(i);
				if (*index)
				{
					*index = (*index)->next;
					free_begin = static_cast<char*>(*index);
					free_end = free_begin + i;
					return chunk_alloc(n, n_node);
				}
			}

			free_end = nullptr;
			free_begin = static_cast<char*>(allocator_malloc::allocate(bytes_to_alloc));
		}

		heap_size += bytes_to_alloc;
		free_end = free_end + bytes_to_alloc;
		return chunk_alloc(n, n_node);
	}

	template <bool threads>
	size_t __default_allocator<threads>::freelist_index(size_t n)
	{
		return (n + __ALIGN - 1) / __ALIGN - 1;
	}
}