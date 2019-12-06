add_library(llmpx_lock MODULE llmpx_lock.cpp)
add_library(llmpx_me MODULE llmpx_me.cpp)
add_library(llmpx MODULE llmpx.cpp)
add_library(llmpx_lock_static llmpx_lock_static.cpp)

target_compile_features(llmpx PRIVATE cxx_range_for cxx_auto_type)
target_compile_features(llmpx_me PRIVATE cxx_range_for cxx_auto_type)

#llvm_map_components_to_libnames(llvm_libs bitwriter core ipo irreader instcombine instrumentation target linker analysis scalaropts support)

message(STATUS "LLVM_LIBS: ${llvm_libs}")
link_directories(/home/mok/project/SVF_/build/lib
    /home/mok/project/SVF_/build/lib/CUDD
    )
    
target_link_libraries( llmpx_lock_static -lm -ldl  LLVMSvf LLVMCudd)


