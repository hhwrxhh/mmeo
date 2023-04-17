#include <iostream>
#include<windows.h>

#include "binary_tree.h"
	
int main()
{
	binary_tree a;
	a.insert_node(1);
	a.insert_node(41);
	a.insert_node(29);
	a.insert_node(34);
	a.insert_node(29);
	a.insert_node(38);
	a.insert_node(29);
	
	a.print();
	a.clear();
}