#pragma once 

#include <iostream>
#include <string>

#include "binary_tree_node.h"

class binary_tree
{
public:
	binary_tree();
	~binary_tree();

	void insert_node(int value);
	bool find_node(int value);
	void clear();

	void printBT(const std::string& prefix, const binary_tree_node* node, bool isLeft);
	void print();

private:
	binary_tree_node* head;
	void delete_node(binary_tree_node* node);
};