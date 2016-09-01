project(CMakeEx)
cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(Qt4 REQUIRED COMPONENTS QtCore)
INCLUDE(${QT_USE_FILE})

set (DESTDIR ${PROJECT_SOURCE_DIR}/install)

ADD_DEFINITIONS(
    ${QT_DEFINITIONS}
    -DTURBOFLOAT_STATIC
    -DTURBOACTIVATE_STATIC
    -DCMAKE_INSTALL_PREFIX=${DESTDIR}
)

include_directories(
    ${QT_INCLUDES}
    .
    ${PROJECT_SOURCE_DIR}/include/TurboActivate
    ${PROJECT_SOURCE_DIR}/include/TurboFloat
)

if (WIN32)
    if (MSVC10)
        foreach(flag_var CMAKE_CXX_FLAGS_RELEASE)
           if(${flag_var} MATCHES "/MD")
              string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
           endif(${flag_var} MATCHES "/MD")
           if(${flag_var} MATCHES "/MTd")
              string(REGEX REPLACE "/MTd" "/MT" ${flag_var} "${${flag_var}}")
           endif(${flag_var} MATCHES "/MTd")
        endforeach(flag_var)

        set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"atls.lib\")
    endif(MSVC10)
endif(WIN32)

QT4_WRAP_CPP(MOC_HEADERS turboactivateex.h turbofloatex.h)

add_library(TurboFloat STATIC IMPORTED)
set_property(TARGET TurboFloat PROPERTY
    IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.4.7/TurboFloat/win32/shared/TurboFloat.lib)

add_library(TurboActivate STATIC IMPORTED)
add_library(cryptlib STATIC IMPORTED)
add_library(curllib STATIC IMPORTED)

set_property(TARGET TurboActivate PROPERTY
    IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.4.7/TurboActivate/win32/static/TurboActivate-MT.lib)
set_property(TARGET cryptlib PROPERTY
    IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.4.7/TurboActivate/win32/static/cryptlib.lib)
set_property(TARGET curllib PROPERTY
    IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.4.7/TurboActivate/win32/static/curllib.lib)

LINK_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.4.7/TurboFloat/win32/static
    ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.4.7/TurboActivate/win32/static
)

set (_LINK_LIBRARIES
        ${QT_LIBRARIES}
        TurboFloat
        TurboActivate
        cryptlib
        curllib
)

set (_INSTALL_FILES
        ${PROJECT_SOURCE_DIR}/include/TurboActivate/dcs.info
        # TurboFloat cannot be statically compiled yet
        ${PROJECT_SOURCE_DIR}/lib/LimeLM/v3.4.7/TurboFloat/win32/shared/TurboFloat.dll
)
install(FILES ${_INSTALL_FILES} DESTINATION ${DESTDIR})

add_executable(${PROJECT_NAME} main.cpp turboactivateex.cpp turbofloatex.cpp ${MOC_HEADERS})
target_link_libraries(${PROJECT_NAME} ${_LINK_LIBRARIES})
