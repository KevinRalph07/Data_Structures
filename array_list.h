#pragma once
#include <ostream>
#include <iostream>
#include <exception>

namespace ssuds
{
	template <class T>
	class ArrayList
	{

	// Nested Iterator Class
	public:
		class ArrayListIterator
		{
		protected:
			// attributes
			ArrayList& mOwner;
			int mCurPos;
			bool mForward;
		public:
			// constructors
			ArrayListIterator(ArrayList& the_list, int start_index=0, bool forward = true) : mOwner(the_list)
			{
				mCurPos = start_index;
				mForward = forward;
			}

			// overloading operators
			bool operator!=(const ArrayListIterator& other)
			{
				return &mOwner != &other.mOwner || mCurPos != other.mCurPos;
			}

			bool operator==(const ArrayListIterator& other)
			{
				return &mOwner == &other.mOwner && mCurPos == other.mCurPos;
			}

			T operator*() // return T& if you want them to be able to change the value
			{
				return mOwner.mData[mCurPos];
			}

			void operator++()
			{
				if (mForward)
					mCurPos++;
				else
					mCurPos--;
			}

			void operator++(int dummy)
			{
				if (mForward)
					mCurPos++;
				else
					mCurPos--;
			}
		};

	protected:

		// Attributes
		T* mData;
		int mSize;
		int mCapacity;

	private: 
		void resize()
		{
			mCapacity *= 2;
			T* temp_array = new T[mCapacity];
			for (int i = 0; i < mSize; i++)
			{
				temp_array[i] = mData[i];
			}
			delete[] mData;
			mData = temp_array;
		}

	public:

		// constructors
		ArrayList()
		{
			mSize = 0;
			mCapacity = 5;
			mData = new T[mCapacity];
		}

		ArrayList(size_t initial_cap)
		{
			mSize = 0;
			mCapacity = initial_cap;
			mData = new T[mCapacity];
		}

		ArrayList(const ArrayList<T>& other)
		{
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mData = new T[mCapacity];
			for (int i = 0; i < mSize; i++)
			{
				mData[i] = other.mData[i];
			}
		}

		ArrayList(const std::initializer_list<T>& ilist)
		{
			mSize = 0;
			mCapacity = ilist.size();
			mData = new T[mCapacity];
			for (T item : ilist)
			{
				push_back(item);
			}
		}

		ArrayList(ArrayList&& other)
		{
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mData = other.mData;
			other.mData = NULL;
		}


		// destructor
		~ArrayList()
		{
			delete[] mData;
		}


		// adding to array
		void push_back(T new_val)
		{
			if (mSize == mCapacity)
			{
				// resize
				resize();
			}

			// add new data
			mData[mSize] = new_val;
			mSize += 1;
		}

		void push_front(T new_val)
		{
			if (mSize == mCapacity)
			{
				// resize
				resize();
			}

			// shift data
			for (int i = mSize; i > 0; i--)
			{
				mData[i] = mData[i - 1];
			}

			// add new data
			mData[0] = new_val;
			mSize += 1;
		}

		void insert(T new_val, int index)
		{

			if (index < 0 || index > mSize)
			{
				//std::cout << "ERROR!: Index is out of range!" << std::endl;
				// throw an exception
				throw std::out_of_range("Index is out of range!");
			}
			else if (index == mSize)
			{
				push_back(new_val);
			}
			else
			{
				if (mSize == mCapacity)
				{
					// resize
					resize();
				}

				// shift data
				for (int i = mSize; i > index; i--)
				{
					mData[i] = mData[i - 1];
				}

				// add new data
				mData[index] = new_val;
				mSize += 1;
			}
		}


		// overloading operators
		T& operator[](int index) const
		{
			// First make sure index is valid (i.e. refers to a spot in our array ( 0 <= index < array_size ))
			if (index < 0 || index >= mSize)
			{
				//std::cout << "ERROR!: Index is out of range!" << std::endl;
				// throw an exception
				throw std::out_of_range("Index is out of range!");
			}
			else
			{
				return mData[index];
			}
			

			// If your return type is T, the user gets a COPY of the value at that index
			// std::string s = alist[15];
			// s = "xyz";				// only changes s
			// alist[15] = "xyz";		// error
			// If your return type is T& (a reference to a T object), the user can modify that value
			// alist[15] = "xyz";		// changes the value in the array
		}

		friend std::ostream& operator<<(std::ostream& os, const ArrayList& AL)
		{
			// [A, B, C, D, ..., Z]
			if (AL.size() == 0)
			{
				os << "[]";
				return os;
			}
			int limit = AL.size() - 1;
			os << "[";
			for (int i = 0; i < limit; i++)
			{
				os << AL[i] << ", ";
			}
			os << AL[limit] << "]";
			return os;
		}

		ArrayList& operator=(const ArrayList& other)
		{
			delete[] mData;
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mData = new T[mCapacity];
			for (int i = 0; i < mSize; i++)
			{
				mData[i] = other.mData[i];
			}
			return *this;
		}


		// other array methods
		int find(T value, int start_index=0) const 
		{
			if (start_index < 0 || start_index > mSize)
			{
				//std::cout << "ERROR!: Index is out of range!" << std::endl;
				// throw exception
				throw std::out_of_range("Index is out of range!");
			}
			else
			{
				for (int i = start_index; i < mSize; i++)
				{
					if (mData[i] == value)
						return i;
				}
			}
			return -1;
		}

		T remove(int index)
		{
			if (index < 0 || index >= mSize)
			{
				//std::cout << "ERROR!: Index is out of range!" << std::endl;
				// throw and exception
				throw std::out_of_range("Index is out of range!");
			}
			else
			{
				// save removed value for return statement
				T removed = mData[index];

				// shift values
				for (int i = index; i < mSize - 1; i++)
				{
					mData[i] = mData[i + 1];
				}

				mSize--;

				if (mSize == mCapacity / 2)
				{
					// shrink mData
					mCapacity /= 2;
					T* temp_array = new T[mCapacity];
					for (int i = 0; i < mSize; i++)
					{
						temp_array[i] = mData[i];
					}
					delete[] mData;
					mData = temp_array;

				}
				return removed;

			}
		}

		void clear()
		{
			delete[] mData;
			mSize = 0;
			mCapacity = 5;
			mData = new T[mCapacity];
		}


		// getters
		int size() const
		{
			return mSize;
		}

		int capacity() const
		{
			return mCapacity;
		}

		// iterator methods
		ArrayListIterator begin()
		{
			return ArrayListIterator(*this);
		}

		ArrayListIterator end()
		{
			return ArrayListIterator(*this, mSize);
		}

		ArrayListIterator rbegin()
		{
			return ArrayListIterator(*this, mSize-1, false);
		}

		ArrayListIterator rend()
		{
			return ArrayListIterator(*this, -1, false);
		}
	};
}