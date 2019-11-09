add_library(llmpx_lock MODULE llmpx_lock.cpp)
add_library(llmpx_me MODULE llmpx_me.cpp)
add_library(llmpx MODULE llmpx.cpp)

target_compile_features(llmpx PRIVATE cxx_range_for cxx_auto_type)
target_compile_features(llmpx_me PRIVATE cxx_range_for cxx_auto_type)


set_target_properties(llmpx PROPERTIES
	COMPILE_FLAGS "-fno-rtti"
)

set_target_properties(llmpx_me PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)

set_target_properties(llmpx_lock PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)