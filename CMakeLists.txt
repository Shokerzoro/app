# Sources
include(cmake/sources.cmake)

# Target
add_executable(app
    ${APP_SOURCES}
)

target_compile_features(app PUBLIC cxx_std_17)

# Angle Includes
include(cmake/include.cmake)

target_include_directories(app
    PUBLIC
        ${APP_INCLUDE}
)

# Linkage
target_link_libraries(app
    PUBLIC
        clientcommon
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

# DevKit Usage
if(DEBUG_BUILD AND COMMAND devkit_use_qt_shared)
    devkit_use_qt_shared(app Widgets Network)
elseif(RELEASE_BUILD AND COMMAND devkit_use_qt_static)
    devkit_use_qt_static(app Widgets Network)
endif()

if(COMMAND devkit_finalize)
    devkit_finalize(app)
endif()

# Tests
if(UNITER_BUILD_TESTS)
    include(cmake/test.cmake)
endif()
