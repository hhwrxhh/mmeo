cmake_minimum_required(VERSION 3.12)

project(branch_and_bound)
set(PROJECT_NAME branch_and_bound)

add_executable(${PROJECT_NAME}
	src/main.cpp

	src/binary_tree.cpp
	src/binary_tree.h

	src/binary_tree_node.cpp
	src/binary_tree_node.h 
)

