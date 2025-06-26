add_executable(test_add test_add.cpp)
target_link_libraries(test_add PRIVATE shm)
add_test(NAME add_test COMMAND test_add)
