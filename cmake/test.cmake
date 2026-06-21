file(GLOB_RECURSE APP_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)

if(APP_TEST_SOURCES)
    add_executable(app_tests
        ${APP_TEST_SOURCES}
    )

    target_link_libraries(app_tests
        PRIVATE
            GTest::gtest_main
            app
    )

    uniter_register_gtest(app_tests)
endif()
