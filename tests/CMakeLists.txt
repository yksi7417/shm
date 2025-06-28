find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_executable(test_add test_add.cpp)
target_link_libraries(test_add PRIVATE shm)
add_test(NAME add_test COMMAND test_add)

add_executable(test_mapped_file test_mapped_file.cpp)
target_link_libraries(test_mapped_file PRIVATE shm)
add_test(NAME mapped_file_test COMMAND test_mapped_file)

add_executable(test_strategy test_strategy.cpp)
target_link_libraries(test_strategy PRIVATE shm)
add_test(NAME strategy_test COMMAND test_strategy)

add_test(
    NAME python_add_test
    COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/tests/test_python_add.py
)
set_tests_properties(python_add_test PROPERTIES
    # Point PYTHONPATH at the directory of the built Python module. This
    # automatically handles configuration subdirectories on Windows.
    ENVIRONMENT "PYTHONPATH=$<TARGET_FILE_DIR:shm_py>"
)
