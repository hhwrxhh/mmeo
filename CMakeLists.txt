cmake_minimum_required(VERSION 3.12)

project(investment)
set(PROJECT_NAME investment)

add_executable(${PROJECT_NAME}
	src/main.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
