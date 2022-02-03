#pragma once
#include <functional>
#include <iostream>

namespace ssuds
{

	template <class K, class V>
	class Map
	{
	protected:
		// attributes
		std::pair<K, V>** mTable;
		std::hash<K> mKeyHasher;
		int mSize;
		int mCapacity;

	private:
		int resize(const K& key)
		{
			/*std::pair<K, V>** temp = new std::pair<K, V>*[mCapacity];
			for (int i = 0; i < mCapacity; i++)
				temp[i] = mTable[i];
			delete[] mTable;
			mCapacity *= 2;
			mTable = new std::pair<K, V>*[mCapacity];
			for (int i = 0; i < mCapacity; i++)
			{
				mTable[i] = NULL;
			}
			for (int i = 0; i < mCapacity / 2; i++)
			{
				if (temp[i] != NULL)
					*this->[temp[i]->first] = temp[i]->second;
			}*/
			std::pair<K, V>** temp = new std::pair<K, V>*[mCapacity * 2];
			for (int i = 0; i < mCapacity * 2; i++)
				temp[i] = NULL;
			for (int i = 0; i < mCapacity; i++)
			{
				if (mTable[i] != NULL)
				{
					// add to temp
					unsigned long long hash = mKeyHasher(mTable[i]->first);
					int index = hash % mCapacity * 2;
					int j = index;
					while (true)
					{
						if (temp[j] == NULL)
						{
							// adds new pair
							temp[j] = new std::pair<K, V>;
							temp[j]->first = mTable[i]->first;
							if (mTable[i]->first != key)
								temp[j]->second = mTable[i]->second;
							break;
						}
						else
						{
						// iterates through the table (loops to the beginning)
						j++;
						if (j == mCapacity * 2)
							j = 0;
						}
					}
				}
			}
			delete[] mTable;
			mTable = temp;
			mCapacity *= 2;
			// returning index of new val
			int j = 0;
			while (mTable[j]->first != key)
			{
				j++;
			}
			return j;


		}

	public:
		// nested iterator class
		class MapIterator
		{
		protected:
			Map& mOwner;
			int mCurPos;

		public:

			MapIterator(Map& the_map, int start_index=0) : mOwner(the_map)
			{
				mCurPos = start_index;
			}

			void operator++()
			{
				mCurPos++;
				while (mOwner.mTable[mCurPos] == NULL)
					mCurPos++;
			}

			bool operator!=(const MapIterator& other)
			{
				return &mOwner != &other.mOwner || mCurPos != other.mCurPos;
			}

			bool operator==(const MapIterator& other)
			{
				return &mOwner == &other.mOwner && mCurPos == other.mCurPos;
			}

			std::pair<K, V>& operator*()
			{
				return *(mOwner.mTable[mCurPos]);
			}
		};

		// constructor
		Map()
		{
			mTable = new std::pair<K, V>*[5];
			for (int i = 0; i < 5; i++)
				mTable[i] = NULL;
			mSize = 0;
			mCapacity = 5;
		}

		~Map()
		{
			delete[] mTable;
		}

		// getters
		int size()
		{
			return mSize;
		}

		// editing Map
		V& operator[](const K& key)
		{
			unsigned long long hash = mKeyHasher(key);
			int index = hash % mCapacity;
			int i = index;
			while (true)
			{
				if (mTable[i] == NULL)
				{
					// adds new pair
					mTable[i] = new std::pair<K, V>;
					mTable[i]->first = key;
					mSize++;
					if ((float)mSize / mCapacity > 0.7f)
						i = resize(key);
					return mTable[i]->second;
				}
				else if (mTable[i]->first == key)
				{
					// changes value of existing key
					return mTable[i]->second;
				}
				else
				{
					// iterates through the table (loops to the beginning)
					i++;
					if (i == mCapacity)
						i = 0;
				}
			}


		}

		V remove(const K& key)
		{
			for (int i = 0; i < mCapacity; i++)
			{
				if (mTable[i] == NULL)
					break;
				else if (mTable[i]->first == key)
				{
					V val = mTable[i]->second;
					delete mTable[i];
					mTable[i] = NULL;
					mSize--;
					return val;
				}
			}
			throw std::out_of_range("Key not in Map!");
		}

		bool contains(const K& key)
		{
			for (int i = 0; i < mCapacity; i++)
			{
				while (mTable[i] == NULL)
					i++;
				if (mTable[i]->first == key)
					return true;
			}
			return false;
		}

		// output
		std::ostream& debug_output(std::ostream& os)
		{
			// {Key:Value, NULL, NULL, Key:Value, NULL}
			os << "{";
			for (int i = 0; i < mCapacity-1; i++)
			{
				if (mTable[i] == NULL)
					os << "NULL";
				else
					os << mTable[i]->first << ":" << mTable[i]->second;
				os << ", ";
			}
			if (mTable[mCapacity - 1] == NULL)
				os << "NULL}";
			else
				os << mTable[mCapacity - 1]->first << ":" << mTable[mCapacity - 1]->second << "}";
			return os;

		}

		friend std::ostream& operator<<(std::ostream& os, Map& M)
		{
			// {Key:Value, NULL, NULL, Key:Value, NULL}
			os << "{";
			for (int i = 0; i < M.mCapacity - 1; i++)
			{
				if (M.mTable[i] != NULL)
				{
					os << M.mTable[i]->first << ":" << M.mTable[i]->second;
					os << ", ";
				}
			}
			if (M.mTable[M.mCapacity - 1] != NULL)
				os << M.mTable[M.mCapacity - 1]->first << ":" << M.mTable[M.mCapacity - 1]->second << "}";
			return os;
		}

		// iterator methods
		MapIterator begin()
		{
			int i = 0;
			while (mTable[i] == NULL)
				i++;
			return MapIterator(*this, i);
		}

		MapIterator end()
		{
			return MapIterator(*this, mCapacity);
		}

	};

}