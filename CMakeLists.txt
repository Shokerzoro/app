# Sources
include(cmake/sources.cmake)

# Target
if(APP_COMPILE_SOURCES)
    add_executable(app
        ${APP_SOURCES}
    )
    set(APP_TARGET_SCOPE PUBLIC)
else()
    add_library(app INTERFACE)
    set(APP_TARGET_SCOPE INTERFACE)
endif()

target_compile_features(app ${APP_TARGET_SCOPE} cxx_std_17)

# Angle Includes
include(cmake/include.cmake)

target_include_directories(app
    ${APP_TARGET_SCOPE}
        ${APP_INCLUDE}
)

# Linkage
target_link_libraries(app
    ${APP_TARGET_SCOPE}
        clientcommon
        ui
        blogic
        data
        database
        control
        messaging
        network
        contract
        contract_verbose_include
        xmlcontract
        xmlcontract_verbose_include
)

# DevKit Usage
if(APP_COMPILE_SOURCES)
    if(DEBUG_BUILD AND COMMAND devkit_use_qt_shared)
        devkit_use_qt_shared(app Widgets Network)
    elseif(RELEASE_BUILD AND COMMAND devkit_use_qt_static)
        devkit_use_qt_static(app Widgets Network)
    endif()

    if(COMMAND devkit_finalize)
        devkit_finalize(app)
    endif()
endif()

# Tests
if(UNITER_BUILD_TESTS)
    include(cmake/test.cmake)
endif()


