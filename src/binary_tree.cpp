#include "binary_tree.h"


binary_tree::binary_tree()
	: head(nullptr)
{
}

binary_tree::~binary_tree()
{
	clear();
}

void binary_tree::insert_node(std::pair<int, int> pair_first, std::pair<int, int> pair_second)
{
	int min_value = std::min(pair_first.first, pair_second.first);

	if (head == nullptr)
	{
		head = new binary_tree_node(pair_first, pair_second);
	}
	else
	{
		binary_tree_node* current = head;
		while (true)
		{
			if (min_value < current->value)
			{
				if (current->left == nullptr)
				{
					current->left = new binary_tree_node(pair_first, pair_second);
					break;
				}
				else
				{
					current = current->left;
				}
			}
			else if (min_value > current->value)
			{
				current->value = std::max(current->data[0].first, current->data[1].first);
				current->right = new binary_tree_node(pair_first, pair_second);
				break;
			}
			else
				break;
		}
	}
}

bool binary_tree::find_node(int value) // find node and if this node has descendants
{
	binary_tree_node* current = head;
	while (current != nullptr)
	{
		if (current->value == value && current->right == nullptr && current->left == nullptr)
			return true;
		else if (current->value > value)
			current = current->left;
		else
			current = current->right;	
	}	
	return false;
}

void binary_tree::clear()
{
	delete_node(head);
	head = nullptr;
}

void binary_tree::printBT(const std::string& prefix, const binary_tree_node* node, bool isLeft)
{
	if (node != nullptr)
	{
		if (node != nullptr)
		{
			std::cout << prefix;
			std::cout << (isLeft ? "|--" : "L--");
			// print the value of the node
			std::cout << node->value << std::endl;
			// enter the next tree level - left and right branch
			printBT(prefix + (isLeft ? "|   " : "    "), node->right, true);
			printBT(prefix + (isLeft ? "|   " : "    "), node->left, false);
		}
	}
}

void binary_tree::print()
{
	printBT("", head, false);
}

void binary_tree::delete_node(binary_tree_node* node)
{
	if (node != nullptr)
	{
		delete_node(node->left);
		delete_node(node->right);
		delete node;
	}
}

