add_library(shm STATIC
    add.cpp
    mapped_file.cpp
)

if(WIN32)
    target_sources(shm PRIVATE windows_mapped_file.cpp)
else()
    target_sources(shm PRIVATE posix_mapped_file.cpp)
endif()

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

# On Windows, set the suffix to .pyd for better Python compatibility
if(WIN32)
    set_target_properties(shm_py PROPERTIES SUFFIX ".pyd")
endif()
