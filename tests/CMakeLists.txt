find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_executable(test_add test_add.cpp)
target_link_libraries(test_add PRIVATE shm)
add_test(NAME add_test COMMAND test_add)

add_test(
    NAME python_add_test
    COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/tests/test_python_add.py
)
