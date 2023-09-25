cmake_minimum_required(VERSION 3.25)
project(rb_tree_c C)

set(CMAKE_C_STANDARD 11)

add_executable(rb_tree_c main.c rb_tree_pointer.h)
