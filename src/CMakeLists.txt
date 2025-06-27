add_library(shm STATIC
    add.cpp
    mapped_file.cpp
)

target_include_directories(shm PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

find_package(Python3 COMPONENTS Development REQUIRED)
add_library(shm_py MODULE
    python_module.cpp
)
set_target_properties(shm_py PROPERTIES PREFIX "")
target_link_libraries(shm_py PRIVATE shm Python3::Python)
