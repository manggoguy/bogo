include_directories("/usr/local/include")

add_subdirectory(tsan-compiler-rt)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fno-rtti -Wall -Wstrict-prototypes -DUSE_TSD_DATA_HACK -D_REENTRANT -DTHREAD_STATS=1  -DMSPACES=1 -DUSE_DL_PREFIX=1 -mrtm")
set(CMAKE_C_FLAGS "-fno-rtti -Wall -Wstrict-prototypes -DUSE_TSD_DATA_HACK -D_REENTRANT -DTHREAD_STATS=1  -DMSPACES=1 -DUSE_DL_PREFIX=1 -mrtm")

set(TXHOOK_SRC
    TxHooks.cpp
)
set(TXHOOK_HEADER
    IDAssigner.h
    LogRecord.h
)

include_directories(
    tsan-compiler-rt/rtl
    tsan-compiler-rt/rtl/interception
    tsan-compiler-rt/rtl/sanitizer_common
)

add_library(TxHooks STATIC
    ${TXHOOK_SRC}
)


target_include_directories(TxHooks PRIVATE ${CMAKE_SOURCE_DIR})
include_directories(../)
