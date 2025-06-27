find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_executable(test_add test_add.cpp)
target_link_libraries(test_add PRIVATE shm)
add_test(NAME add_test COMMAND test_add)

add_executable(test_mapped_file test_mapped_file.cpp)
target_link_libraries(test_mapped_file PRIVATE shm)
add_test(NAME mapped_file_test COMMAND test_mapped_file)

add_test(
    NAME python_add_test
    COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/tests/test_python_add.py
)
set_tests_properties(python_add_test PROPERTIES
    ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/src"
)
