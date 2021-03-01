cmake_minimum_required(VERSION 3.15)
project(OCR)

# CMAKE
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Qt
set(QT_VERSION 5)
set(QT_LIB Qt${QT_VERSION})
set(QT_COMPONENTS Widgets)
set(QT_LIBS ${QT_COMPONENTS})
LIST(TRANSFORM QT_LIBS PREPEND ${QT_LIB}::)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    string(TOLOWER "platforms/q${CMAKE_SYSTEM_NAME}d${CMAKE_SHARED_LIBRARY_SUFFIX}" QT_PLUGINS)
else()
    string(TOLOWER "platforms/q${CMAKE_SYSTEM_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}" QT_PLUGINS)
endif()

find_package(${QT_LIB} COMPONENTS ${QT_COMPONENTS} REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS UI)
set(CMAKE_AUTORCC ON)


# Targeting
set(SOURCES
        main.cpp
        MainWindow.cpp MainWindow.h

        PanelContainers.cpp PanelContainers.h
        Tab.h
        ToolPanels.cpp ToolPanels.cpp

        HomeTab.cpp HomeTab.h
        ImageViewer.cpp ImageViewer.h
        ImageView.cpp ImageView.h

        TextData.cpp TextData.h)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_executable(${PROJECT_NAME} Debug.h ${SOURCES})
else()
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
endif()
target_link_libraries(${PROJECT_NAME} ${QT_LIBS})


# Installation
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
set(INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX})

file(TO_CMAKE_PATH "${CMAKE_PREFIX_PATH}" CMAKE_PREFIX_PATH)
include(cmake/init_install.cmake)

install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_DESTINATION})

install(CODE "set(QT_PLUGINS ${QT_PLUGINS})")
install(CODE [[
foreach(_f IN LISTS QT_PLUGINS)
    unset(_fp CACHE)
    find_file(_fp "${_f}"
        PATHS ${CMAKE_PREFIX_PATH}
        PATH_SUFFIXES plugins
    )
    get_filename_component(_f ${_f} DIRECTORY)
    if (NOT _fp MATCHES "_fp-NOTFOUND")
        file(INSTALL ${_fp} DESTINATION ${INSTALL_DESTINATION}/${_f})
    endif()
endforeach()
]])
install(CODE [[
unset(_dp CACHE)
find_path(_dp imageformats
    PATHS ${CMAKE_PREFIX_PATH}
    PATH_SUFFIXES plugins
)
if (NOT _dp MATCHES "_dp-NOTFOUND")
    file(INSTALL ${_dp}/imageformats DESTINATION ${INSTALL_DESTINATION})
endif()
]])

install(SCRIPT cmake/install.cmake)


#add_custom_command(TARGET ${PROJECT_NAME}
#        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
#        copy_directory ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_autogen/include ${CMAKE_SOURCE_DIR})