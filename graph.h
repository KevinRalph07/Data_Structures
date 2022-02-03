#pragma once
#include <map>
#include <string>
#include <iostream>


namespace ssuds
{

	template <class N, class E>
	class Graph
	{

	protected:
		// attributes
		std::map<N, std::map<N, E>> mData;

	public:

		// adding
		void add_node(const N& new_node)
		{
			// haven't seen new node before
			if (mData.find(new_node) == mData.end())
				mData[new_node];
		}

		void add_edge(const N& start, const N& end, const E& edge_val)
		{
			// if start or end doesn't exist, throw exception
			if (mData.find(start) == mData.end() || mData.find(end) == mData.end())
				throw std::out_of_range("start and/or end do not exist");
			mData[start][end] = edge_val;
		}

		// removing
		void remove_node(const N& node)
		{
			if (has_node(node))
			{
				mData[node].clear();
				typename std::map<N, std::map<N, E>>::iterator it = mData.begin();
				while (it != mData.end())
				{
					remove_edge(it->first, node);
					it++;
				}
				mData.erase(node);
			}
			//throw std::out_of_range("node does note exist");
		}

		void remove_edge(const N& start, const N& end)
		{
			if (are_connected(start, end))
			{
				mData[start].erase(end);
			}
			//throw std::out_of_range("start and end are not connected");
		}

		// getters
		int get_num_nodes()
		{
			return mData.size();
		}

		int get_num_neighbors(const N& node)
		{
			return mData[node].size();
		}

		int total_edges()
		{
			int total = 0;
			for (int i = 0; i < get_num_nodes(); i++)
			{
				total += get_num_neighbors(get_node(i));
			}
			return total;
		}

		N get_node(int index)
		{
			if (index < 0 || index >= mData.size())
				throw std::out_of_range("index out of range");
			typename std::map<N, std::map<N, E>>::iterator it = mData.begin();
			for (int i = 0; i < index; i++)
				it++;
			return it->first;
		}

		E get_edge(const N& start, const N& end)
		{
			if (are_connected(start, end))
			{
				return mData[start][end];
			}
			throw std::out_of_range("start and end are not connected");
		}

		N get_neighbor(const N& node, int index)
		{
			if (index < 0 || index >= mData[node].size())
				throw std::out_of_range("index out of range");
			typename std::map<N, E>::iterator it = mData[node].begin();
			for (int i = 0; i < index; i++)
				it++;
			return it->first;
		}

		E get_neighbor_edge(const N& node, int index)
		{
			if (index < 0 || index >= mData[node].size())
				throw std::out_of_range("index out of range");
			return mData[node][get_neighbor(node, index)];
		}

		// checks
		bool has_node(const N& node_val)
		{
			return mData.find(node_val) != mData.end();
		}

		bool are_connected(const N& start, const N& end)
		{
			if (mData.find(start) != mData.end())
			{
				if (mData[start].find(end) != mData[start].end())
					return true;
			}
			return false;
		}

		// overloading operators
		friend std::ostream& operator<<(std::ostream& os, Graph& G)
		{
			int longest_string = 0;
			int num_nodes = G.get_num_nodes();
			for (int i = 0; i < num_nodes; i++)
			{
				N node = G.get_node(i);
				std::string str = "" + node;
				if (str.length() > longest_string)
				{
					longest_string = str.length();
				}
			}

			int gap = longest_string + 5;
			for (int i = 0; i < num_nodes; i++)
			{
				N node = G.get_node(i);
				std::string temp = "" + node;
				int num_spaces = gap - temp.length();
				os << node;
				for (int i = 0; i < num_spaces; i++)
					os << " ";
				os << "| ";
				int num_neighbors = G.get_num_neighbors(node);
				for (int i = 0; i < num_neighbors; i++)
				{
					N neighbor = G.get_neighbor(node, i);
					E edge = G.get_edge(node, neighbor);
					os << "{" << neighbor << ":" << edge << "}" << " ";
				}
				os << std::endl;
			}

			return os;

		}
	};




}