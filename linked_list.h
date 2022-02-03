#pragma once


namespace ssuds
{
	
	template <class T>
	class LinkedList
	{
	protected:
		// nested classes
		class Node
		{
		public:
			T mData;
			Node* mNext;
			Node* mPrev;

			Node(T val)
			{
				mData = val;
				mNext = mPrev = nullptr;
			}

			friend class LinkedList;

		};

		// attributes
		Node* mStart;
		Node* mEnd;
		int mSize;

	public:
		class LinkedListIterator
		{
		protected:
			// attributes
			LinkedList* mList;
			Node* mCurNode;
			bool mForward;

		public:
			// constructor
			LinkedListIterator()
			{

			}

			LinkedListIterator(LinkedList* the_list, Node* node, bool forward = true) : mList(the_list)
			{
				mCurNode = node;
				mForward = forward;
			}

			// overloading operators
			T& operator*()
			{
				return mCurNode->mData;
			}

			// overloading operators
			void operator++()
			{
				if (mForward)
					mCurNode = mCurNode->mNext;
				else
					mCurNode = mCurNode->mPrev;
			}

			void operator++(int dummy)
			{
				if (mForward)
					mCurNode = mCurNode->mNext;
				else
					mCurNode = mCurNode->mPrev;
			}

			bool operator!=(const LinkedListIterator& other)
			{
				return mList != other.mList || mCurNode != other.mCurNode;
			}

			LinkedListIterator operator+(int offset)
			{
				for (int i = 0; i < offset; i++)
				{
					mCurNode = mCurNode->mNext;
				}
				return *this;
			}

			// getters
			Node* cur_node()
			{
				return mCurNode;
			}

			int index()
			{
				int index = 0;
				Node* temp = mList->list_start();
				while (temp != this->mCurNode)
				{
					temp = temp->mNext;
					index++;
				}
				return index;
			}
		};

		// constructors
		LinkedList()
		{

		}

		LinkedList(const LinkedList<T>& other)
		{
			push_back(other.mStart->mData);
			Node* cur = other.list_start()->mNext;
			while (cur != other.list_end()->mNext)
			{
				push_back(cur->mData);
				cur = cur->mNext;
			}
		}

		LinkedList(const std::initializer_list<T>& ilist)
		{
			for (T item : ilist)
			{
				push_back(item);
			}
		}

		LinkedList(LinkedList&& other)
		{
			mStart = other.mStart;
			mEnd = other.mEnd;
			mSize = other.mSize;
			other.mSize = 0;
			other.mStart = other.mEnd = NULL;
		}

		// destructor
		~LinkedList()
		{
			clear();
		}

		// methods for editing linked list
		void push_back(const T& new_val)
		{
			insert(new_val, mSize);
		}

		void push_front(const T& new_val)
		{
			insert(new_val, 0);
		}

		void insert(const T& val, int index)
		{
			Node* new_node = new Node(val);
			if (mStart == NULL && index == 0)
			{
				// Case I: inserting into an empty list
				mStart = mEnd = new_node;
			}
			else if (mStart != NULL && index == 0)
			{
				// Case II: inserting at the beginning of a non-empty list
				new_node->mNext = mStart;
				mStart->mPrev = new_node;
				mStart = new_node;
			}
			else if (mStart != NULL && index == mSize)
			{
				// Case III: inserting at the END of a non-empty list
				new_node->mPrev = mEnd;
				mEnd->mNext = new_node;
				mEnd = new_node;
			}
			else if (mStart != NULL && index > 0 && index < mSize)
			{
				// Case IV: inserting in the middle of a non-empty lists
				// part1: get to the right spot
				Node* cur = mStart;
				for (int i = 0; i < index - 1; i++)
					cur = cur->mNext;

				// part2: actually insert
				new_node->mNext = cur->mNext;
				new_node->mPrev = cur;
				cur->mNext->mPrev = new_node;
				cur->mNext = new_node;
			}
			else
				throw std::out_of_range("Invalid index: " + std::to_string(index));

			mSize++;
		}

		void clear()
		{
			Node* cur = list_start();
			Node* next;
			while (cur != list_end())
			{
				next = cur->mNext;
				delete cur;
				cur = next;
			}
			mSize = 0;
			mStart = mEnd = NULL;
			
		}

		// getters
		Node* list_start() const
		{
			return mStart;
		}

		Node* list_end() const
		{
			return mEnd;
		}

		int size() const
		{
			return mSize;
		}

		T find_largest()
		{
			LinkedListIterator it = begin();
			T largest_val = *it;
			while (it != end())
			{
				T cur_val = *it;
				if (cur_val > largest_val)
					largest_val = cur_val;
				it++;
			}
			return largest_val;
		}

		// overloading operators
		T& operator[](int index) const
		{
			if (index < 0 || index >= mSize)
			{
				throw std::out_of_range("Index is out of range!");
			}
			else
			{
				// get to the correct spot
				Node* cur = mStart;
				for (int i = 0; i < index; i++)
					cur = cur->mNext;
				return cur->mData;
			}
		}

		friend std::ostream& operator<<(std::ostream& os, const LinkedList& LL)
		{
			if (LL.size() == 0)
			{
				os << "[]";
				return os;
			}
			os << "[";
			Node* cur = LL.list_start();
			while (cur != LL.list_end())
			{
				os << cur->mData << ", ";
				cur = cur->mNext;
			}
			os << LL.list_end()->mData;
			os << "]";
			return os;
		}

		LinkedList& operator=(const LinkedList& other)
		{
			clear();
			push_back(other.mStart->mData);
			Node* cur = other.list_start()->mNext;
			while (cur != other.list_end()->mNext)
			{
				push_back(cur->mData);
				cur = cur->mNext;
			}
			return *this;
		}

		// iterator methods
		LinkedListIterator begin()
		{
			return LinkedListIterator(this, mStart);
		}

		LinkedListIterator end()
		{
			return LinkedListIterator(this, mEnd->mNext);
		}

		LinkedListIterator rbegin()
		{
			return LinkedListIterator(this, mEnd, false);
		}

		LinkedListIterator rend()
		{
			return LinkedListIterator(this, mStart->mPrev, false);
		}

		LinkedListIterator find(T value)
		{
			LinkedListIterator iter = begin();
			while (iter != end())
			{
				if (iter.cur_node()->mData == value)
					return iter;
				++iter;
			}
			return end();
		}

		LinkedListIterator find(T value, LinkedListIterator start)
		{
			LinkedListIterator iter = start;
			while (iter != end())
			{
				if (iter.cur_node()->mData == value)
					return iter;
				++iter;
			}
			return end();
		}

		LinkedListIterator remove(LinkedListIterator iter)
		{
			LinkedListIterator next;
			if (iter.cur_node() == mStart)
			{
				iter.cur_node()->mNext->mPrev = NULL;
				mStart = iter.cur_node()->mNext;
				next = LinkedListIterator(this, iter.cur_node()->mNext);
			}
			else if (iter.cur_node() == mEnd)
			{
				iter.cur_node()->mPrev->mNext = NULL;
				mEnd = iter.cur_node()->mPrev;
				next = LinkedListIterator(this, iter.cur_node()->mPrev);
			}
			else
			{
				iter.cur_node()->mPrev->mNext = iter.cur_node()->mNext;
				iter.cur_node()->mNext->mPrev = iter.cur_node()->mPrev;
				next = LinkedListIterator(this, iter.cur_node()->mNext);
			}
			delete iter.cur_node();
			mSize--;
			return next;
		}

	};

}