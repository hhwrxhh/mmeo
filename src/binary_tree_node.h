#pragma once 

class binary_tree_node
{
public:
	binary_tree_node(int data, // do not forget about matrix
		binary_tree_node* Left = nullptr,
		binary_tree_node* Right = nullptr);

	int value;
	binary_tree_node* left;
	binary_tree_node* right;
};