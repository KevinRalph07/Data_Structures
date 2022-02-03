#pragma once
#include <ostream>
#include <iostream>
#include <sstream>
#include <array_list.h>
#include <stack>

namespace ssuds
{

	enum class SetTraversalType { PRE_ORDER, IN_ORDER, POST_ORDER };

	template <class T>
	class Set
	{

	protected:
		// Node class
		class Node
		{
		public:
			Set& mOwner;
			T mData;
			Node* mParent;
			Node* mLeft;
			Node* mRight;

			Node(Set& the_set, const T& val) : mOwner(the_set), mData(val)
			{
				mParent = NULL;
				mLeft = NULL;
				mRight = NULL;
			}

			Node(Set& the_set, const T& val, Node* parent) : mOwner(the_set), mData(val)
			{
				mParent = parent;
				mLeft = NULL;
				mRight = NULL;
			}

			~Node()
			{

			}

			void add_recursive(const T& val)
			{
				if (val < mData)
				{
					// go to the left
					if (mLeft == NULL)
					{
						// no left child -- add new one
						mLeft = new Node(mOwner, val, this);
						mOwner.mSize++;
					}
					else
					{
						// have left child -- tell left node to add val
						mLeft->add_recursive(val);
					}
				}

				else if (val > mData)
				{
					// go to the right
					if (mRight == NULL)
					{
						// no right child -- add new one
						mRight = new Node(mOwner, val, this);
						mOwner.mSize++;
					}
					else
					{
						// have right child -- tell right node to add val
						mRight->add_recursive(val);
					}
				}
			}

			int get_height_recursive()
			{
				int left_height = 0;
				int right_height = 0;
				if (mLeft)
					left_height = mLeft->get_height_recursive();
				if (mRight)
					right_height = mRight->get_height_recursive();
				if (left_height > right_height)
					return left_height + 1;
				return right_height + 1;
			}

			void generate_tree_string_recursive(std::stringstream& sstr, int cur_depth, std::string prefix)
			{
				for (int i = 0; i < cur_depth; i++)
					sstr << "   ";
				sstr << prefix << mData << std::endl;
				if (mLeft)
					mLeft->generate_tree_string_recursive(sstr, cur_depth + 1, "<");
				if (mRight)
					mRight->generate_tree_string_recursive(sstr, cur_depth + 1, ">");
			}

			bool contains_recursive(T val)
			{
				bool left = false;
				bool right = false;
				if (mData == val)
					return true;
				if (mLeft)
					left = mLeft->contains_recursive(val);
				if (mRight)
					right = mRight->contains_recursive(val);
				return (left || right);
			}

			ArrayList<T> traverse_all_recursive(SetTraversalType stt, ArrayList<T>& AL)
			{
				if (stt == SetTraversalType::PRE_ORDER)
					AL.push_back(mData);
				if (mLeft)
					mLeft->traverse_all_recursive(stt, AL);
				if (stt == SetTraversalType::IN_ORDER)
					AL.push_back(mData);
				if (mRight)
					mRight->traverse_all_recursive(stt, AL);
				if (stt == SetTraversalType::POST_ORDER)
					AL.push_back(mData);
				return AL;
			}

			Node* successor_node(Node* node)
			{
				Node* current = node;
				while (current && current->mLeft)
					current = current->mLeft;
				return current;
			}

			Node* remove_recursive(Node* cur_node, T val)
			{
				// base case
				if (cur_node == NULL)
					return cur_node;

				// go left
				if (val < cur_node->mData)
					cur_node->mLeft = remove_recursive(cur_node->mLeft, val);

				// go right
				else if (val > cur_node->mData)
					cur_node->mRight = remove_recursive(cur_node->mRight, val);

				else
				{
					// no child
					if (cur_node->mLeft == NULL && cur_node->mRight == NULL)
						return NULL;

					// only 1 child
					else if (cur_node->mLeft == NULL)
					{
						Node* temp = cur_node->mRight;
						delete cur_node;
						return temp;
					}
					else if (cur_node->mRight == NULL)
					{
						Node* temp = cur_node->mLeft;
						delete cur_node;
						return temp;
					}

					// 2 children
					Node* temp = successor_node(cur_node->mRight);
					cur_node->mData = temp->mData;
					cur_node->mRight = remove_recursive(cur_node->mRight, temp->mData);


				}
				return cur_node;

				//bool left = false;
				//bool right = false;
				//if (cur_node->mData == val)
				//	// remove
				//	remove_node(this);
				//	return mData;
				//if (cur_node->mLeft)
				//	cur_node->mLeft->remove_recursive(cur_node->mLeft, val);
				//if (cur_node->mRight)
				//	cur_node->mRight->remove_recursive(cur_node->mRight, val);
			}

			void clear_recursive()
			{
				if (mLeft)
					mLeft->clear_recursive();
				if (mRight)
					mRight->clear_recursive();
				delete this;
			}

			T get_total_recursive(int layer=0)
			{
				T child_sum = 0;
				if (mLeft)
				{
					T left = mLeft->get_total_recursive(layer + 1);
					child_sum += left;
				}
				if (mRight)
				{
					T right = mRight->get_total_recursive(layer + 1);
					child_sum += right;
				}
				if (layer % 2 == 0)
				{
					// even layer
					return child_sum + mData;
				}
				else if (layer % 2 == 1)
				{
					// odd layer
					return child_sum - mData;
				}
			}
		};

		// Attributes
		Node* mRoot;
		int mSize;

	public:

		class SetIterator
		{

		protected:
			Set& mOwner;
			std::stack<Node*> mNodeStack;
			Node* mNode;
			bool mEnd;
			T curData;

		public:
			SetIterator(Set& the_set) : mOwner(the_set)
			{
				mNode = NULL;
				mEnd = true;
			}

			SetIterator(Set& the_set, Node* root_node) : mOwner(the_set)
			{
				mNode = root_node;
				mEnd = false;
			}

			bool operator!=(const SetIterator& other)
			{
				return &mOwner != &other.mOwner || mNode != other.mNode || mEnd != other.mEnd;
			}

			T operator*()
			{
				return curData;
			}

			void operator++()
			{
				while (mNode != NULL)
				{
					mNodeStack.push(mNode);
					mNode = mNode->mLeft;
				}
				
				if (mNodeStack.empty())
				{
					mEnd = true;
					return;
				}

				mNode = mNodeStack.top();
				mNodeStack.pop();
				curData = mNode->mData;
				mNode = mNode->mRight;
			}

		};

		// Constructors
		Set()
		{
			mRoot = NULL;
			mSize = NULL;
		}

		Set(const Set<T>& other)
		{
			ArrayList<T> list = other.traverse_all(SetTraversalType::PRE_ORDER);
			for (T item : list)
				add(item);
		}

		Set(const std::initializer_list<T>& ilist)
		{
			int mSize = 0;
			for (T item : ilist)
			{
				add(item);
			}
		}

		Set(Set&& other) 
		{
			mRoot = other.mRoot;
			mSize = other.mSize;
			other.mSize = 0;
			other.mRoot = NULL;
		}

		// deconstructor
		~Set()
		{
			clear();
		}

		// starting methods for recursion
		void add(const T& val)
		{
			if (mRoot == NULL)
			{
				// first item -- make new root
				mRoot = new Node(*this, val);
				mSize = 1;
			}
			mRoot->add_recursive(val);
		}

		int get_height()
		{
			if (mRoot == NULL)
				return 0;
			return mRoot->get_height_recursive();
		}
		
		std::string generate_tree_string()
		{
			std::stringstream ss;
			if (mRoot)
				mRoot->generate_tree_string_recursive(ss, 0, "");
			return ss.str();
		}

		bool contains(T val)
		{
			if (mRoot == NULL)
				return false;
			return mRoot->contains_recursive(val);
		}

		ArrayList<T> traverse_all(SetTraversalType stt) const
		{
			ArrayList<T> AL;
			if (mRoot)
				mRoot->traverse_all_recursive(stt, AL);
			return AL;
		}

		void remove(T val)
		{
			if (contains(val))
			{
				mRoot->remove_recursive(mRoot, val);
				return;
			}
			throw std::out_of_range("Value Not In Set");

		}

		void clear()
		{
			if (mRoot)
			{
				mRoot->clear_recursive();
				mRoot = NULL;
				mSize = 0;
			}
		}

		T get_total()
		{
			if (mRoot == NULL)
				return NULL;
			return mRoot->get_total_recursive();

		}
		
		// getters
		int size()
		{
			return mSize;
		}

		// overloading operators
		friend std::ostream& operator<<(std::ostream& os, const Set& S)
		{
			ArrayList<T> AL = S.traverse_all(SetTraversalType::IN_ORDER);
			os << AL;
			return os;
		}

		Set& operator=(const Set& other)
		{
			clear();
			ArrayList<T> list = other.traverse_all(SetTraversalType::PRE_ORDER);
			for (T item : list)
				add(item);
			return *this;
		}

		Set<T> operator|(const Set& other)
		{
			ArrayList<T> A = traverse_all(SetTraversalType::IN_ORDER);
			ArrayList<T> B = other.traverse_all(SetTraversalType::IN_ORDER);
			Set<T> newSet;
			for (T a : A)
			{
				newSet.add(a);
			}
			for (T b : B)
			{
				newSet.add(b);
			}
			return newSet;
		}

		Set<T> operator&(const Set& other)
		{
			ArrayList<T> A = traverse_all(SetTraversalType::IN_ORDER);
			ArrayList<T> B = other.traverse_all(SetTraversalType::IN_ORDER);
			ArrayList<T> list;
			for (T a : A)
			{
				for (T b : B)
				{
					if (a == b)
					{
						list.push_back(a);
					}
				}
			}
			Set<T> newSet;
			for (T item : list)
			{
				newSet.add(item);
			}
			return newSet;
		}

		Set<T> operator-(const Set& other)
		{
			ArrayList<T> A = traverse_all(SetTraversalType::IN_ORDER);
			ArrayList<T> B = other.traverse_all(SetTraversalType::IN_ORDER);
			ArrayList<T> list;
			for (T a : A)
			{
				int counter = 0;
				for (T b : B)
				{
					if (a == b)
					{
						counter++;
					}
				}
				if (counter == 0)
					list.push_back(a);

			}
			Set<T> newSet;
			for (T item : list)
			{
				newSet.add(item);
			}
			return newSet;
		}

		// iterator methods
		SetIterator begin()
		{
			return SetIterator(*this, mRoot);
		}

		SetIterator end()
		{
			return SetIterator(*this);
		}
	};

};