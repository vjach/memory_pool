macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} memory_pool gtest gtest_main)
    target_compile_features(${TESTNAME} PUBLIC cxx_std_14)
    add_test(${TESTNAME} ${TESTNAME})
endmacro()

package_add_test(run_pool_tests mempool-tests.cc)
