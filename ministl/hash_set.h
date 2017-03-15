#ifndef UNORDERED_SET_H
#define UNORDERED_SET_H

namespace ministl
{
	/// HashSetNode
	///
	template <typename Key>
	struct HashSetNode
	{
		Key mKey;
		HashSetNode *mpNext;
	};

	/// HashSetIterator
	///
	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	struct HashSetIterator
	{
		typedef forward_iterator_tag                              iterator_category;
		typedef ptrdiff_t                                         difference_type;
		typedef Key                                               value_type;
		typedef Key*                                              pointer;
		typedef Key&                                              reference;
		typedef HashSetIterator<Key, Hash, KeyEqual, Allocator> iterator;
		typedef HashSetIterator<Key, Hash, KeyEqual>            const_iterator;
		typedef HashSetNode<Key>                                  node_type;
		typedef HashSetIterator<Key, Hash, KeyEqual>            this_type;

	public:
		node_type *mpNode;
		HashSet   *mpTable;

	public:
		HashSetIterator();
		HashSetIterator(node_type *mpNode, HashSet *mpTable);

		reference  operator*()const;
		pointer    operator->()const;
		this_type& operator++();
		this_type  operator++(int);
	};

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename HashSetIterator<Key, Hash, KeyEqual, Allocator>::HashSetIterator()
		: mpNode(NULL),
		  mpTable(NULL)
	{
		// empty
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename HashSetIterator<Key, Hash, KeyEqual, Allocator>::HashSetIterator(node_type *pNode, HashSet *pTable)
		: mpNode(pNode),
		  mpTable(pTable)
	{
		// empty
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename HashSetIterator<Key, Hash, KeyEqual, Allocator>::reference
	HashSetIterator<Key, Hash, KeyEqual, Allocator>::operator*()const
	{
		return mpNode->mKey;
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename HashSetIterator<Key, Hash, KeyEqual, Allocator>::pointer
	HashSetIterator<Key, Hash, KeyEqual, Allocator>::operator->()const
	{
		return &mpNode->mKey;
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename HashSetIterator<Key, Hash, KeyEqual, Allocator>::this_type&
	HashSetIterator<Key, Hash, KeyEqual, Allocator>::operator++()
	{
		node_type *pNode = mpNode;
		mpNode = mpNode->mpNext;
		if (!mpNode)
			mpNode = mpTable->GetNextBucket(pNode->mKey);
		return *this;
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename HashSetIterator<Key, Hash, KeyEqual, Allocator>::this_type
	HashSetIterator<Key, Hash, KeyEqual, Allocator>::operator++(int)
	{
		this_type temp(*this);
		++*this;
		return temp;
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	bool operator==(const HashSetIterator<Key, Hash, KeyEqual, Allocator> &lhs,
	                const HashSetIterator<Key, Hash, KeyEqual, Allocator> &rhs)
	{
		return lhs.mpNode == rhs.mpNode;
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	bool operator!=(const HashSetIterator<Key, Hash, KeyEqual, Allocator> &lhs,
	                const HashSetIterator<Key, Hash, KeyEqual, Allocator> &rhs)
	{
		return lhs.mpNode != rhs.mpNode;
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	class HashSetBase
	{
	public:
		typedef Allocator allocator_type;
		typedef HashSetNode<Key> node_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

	protected:
		std::vector<node_type*> mBucketTable;
		size_type               mSize;
		allocator_type          mAllocator;

	protected:
		node_type *AllocateNode();
		void       FreeNode(node_type *pNode);
		void       Insert(node_type *pBucket, node_type *pNode);
		void       RemoveAfter(node_type *pNode);
	};

	template <typename Key, typename Hash = std::hash<Key>, typename KeyEqual = std::equal<Key>, typename Allocator = alloc>
	class unordered_set
	{
		typedef HashSetBase<Key, Hash, KeyEqual, Allocator>           base_type;
		typedef unordered_set<Key, Hash, KeyEqual, Allocator>         this_type;

	public:
		typedef Key                                                   key_type;
		typedef Key                                                   value_type;
		typedef Hash                                                  hasher;
		typedef KeyEqual                                              key_equal;
		typedef Key&                                                  reference;
		typedef const Key&                                            const_reference;
		typedef Key*                                                  pointer;
		typedef const Key*                                            const_pointer;
		typedef HashSetIterator<const Key, Hash, KeyEqual, Allocator> iterator;
		typedef HashSetIterator<const Key, Hash, KeyEqual, Allocator> const_iterator;
		typedef iterator                                              local_iterator;
		typedef const_iterator                                        const_local_iterator;
		typedef HashSetNode<T>                                        node_type;
		typedef base_type::difference_type                            difference_type;
		typedef base_type::size_type                                  size_type;
		typedef base_type::allocator_type                             allocator_type;
		


	};


}


#endif // unordered_set.h