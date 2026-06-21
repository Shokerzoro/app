include(cmake/sources.cmake)
include(cmake/include.cmake)

add_executable(app
    ${APP_SOURCES}
)

target_compile_features(app PUBLIC cxx_std_17)

target_include_directories(app
    PUBLIC
        ${APP_INCLUDE}
)

target_link_libraries(app
    PUBLIC
        clientcommon
        pdfparser
        ui
        blogic
        data
        database
        control
        messaging
        network
        contract
        xmlcontract
)

if(DEBUG_BUILD AND COMMAND devkit_use_qt_shared)
    devkit_use_qt_shared(app Widgets Network)
elseif(RELEASE_BUILD AND COMMAND devkit_use_qt_static)
    devkit_use_qt_static(app Widgets Network)
endif()

if(COMMAND devkit_finalize)
    devkit_finalize(app)
endif()

if(UNITER_BUILD_TESTS)
    include(cmake/test.cmake)
endif()
