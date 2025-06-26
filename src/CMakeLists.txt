add_library(shm STATIC
    add.cpp
)

target_include_directories(shm PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
