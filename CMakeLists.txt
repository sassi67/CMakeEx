project(CMakeEx)
cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(Qt4 REQUIRED COMPONENTS QtCore)
INCLUDE(${QT_USE_FILE})

ADD_DEFINITIONS(
    ${QT_DEFINITIONS}
    -DTURBOACTIVATE_STATIC
)

include_directories(
    ${QT_INCLUDES}
    .
    ${PROJECT_SOURCE_DIR}/include/TurboActivate
    ${PROJECT_SOURCE_DIR}/include/TurboFloat
)

QT4_WRAP_CPP(MOC_HEADERS turbofloatex.h)

if(WIN32)
    set(TurboActivateDir
        ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.5.6/TurboActivate/win32/static/
        )
    set(TurboActivateLib
        ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.5.6/TurboActivate/win32/static/libTurboActivate.lib
        )

    set(TurboFloatDir
        ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.5.6/TurboFloat/win32/static/
        )
    set(TurboFloatLib
        ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.5.6/TurboFloat/win32/static/libTurboFloat.lib
        )

else(WIN32)
    set(TurboActivateDir
        ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.5.6/TurboActivate/linux/static/amd64/
        )
    set(TurboActivateLib
        ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.5.6/TurboActivate/linux/static/amd64/libTurboActivate.a
        )
    add_library(TurboActivate_Lib STATIC IMPORTED IMPORTED_LOCATION ${TurboActivateLib})

    # TurboFloat for Linux is still shared
    set(TurboFloatDir
        ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.5.6/TurboFloat/linux/shared/amd64/
        )
    set(TurboFloatLib
        ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.5.6/TurboFloat/win32/shared/amd64/libTurboFloat.so
        )
endif(WIN32)

LINK_DIRECTORIES(
    ${TurboActivateDir}
    ${TurboFloatDir}
)

SET(_LINK_LIBRARIES
    ${QT_LIBRARIES}
    TurboActivate
    TurboFloat
    rt
)

add_executable(${PROJECT_NAME} main.cpp turbofloatex.cpp ${MOC_HEADERS})
target_link_libraries(${PROJECT_NAME} ${_LINK_LIBRARIES})
