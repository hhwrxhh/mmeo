#include "binary_tree_node.h"

binary_tree_node::binary_tree_node(std::pair<int, int> pair_first,
								   std::pair<int, int> pair_second,
								   binary_tree_node* Left ,
								   binary_tree_node* Right )
	:left(Left), right(Right)
{	
	data[0] = pair_first;
	data[1] = pair_second;

	value = std::min(pair_first.first, pair_second.first);
}