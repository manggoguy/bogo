add_library(lock static wrapper_pthread.h)

set_target_properties(llmpx PROPERTIES
	COMPILE_FLAGS "-fno-rtti -lpthread"
	)
