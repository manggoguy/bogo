cmake_minimum_required(VERSION 3.1)

add_library(lock static wrapper_pthread.h)

set_target_properties(lock PROPERTIES
	COMPILE_FLAGS "-fno-rtti -lpthread"
	)
