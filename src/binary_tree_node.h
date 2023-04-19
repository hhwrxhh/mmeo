#pragma once 

#include <utility>
#include <vector>
#include <array>
#include <algorithm>


class binary_tree_node
{
public:
	binary_tree_node(std::pair<int, int> pair_first, 
					 std::pair<int, int> pair_second, // do not forget about matrix,
					 binary_tree_node* Left = nullptr,
					 binary_tree_node* Right = nullptr);

	int value;
	binary_tree_node* left;
	binary_tree_node* right;

	std::array<std::pair<int, int>, 2> data; // value of nested node, matrix of nested node
};