cmake_minimum_required(VERSION 3.15)
project(OCR)

option(INSTALL_PYTHON_LIBRARIES "Distribute required python site-packages" ON)
# cmake INSTALL should be executed after build
# /python should contain embedded interpreter with all requirements (recognition/requirements.txt) installed


# CMAKE
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Qt
set(QT_VERSION 0)
find_package(Qt6 COMPONENTS Widgets QUIET)
if (Qt6_FOUND)
    set(QT_VERSION 6)
else ()
    find_package(Qt5 COMPONENTS Widgets QUIET)
    if (Qt5_FOUND)
        set(QT_VERSION 5)
    endif ()
endif ()

if (QT_VERSION MATCHES 0)
    find_package(Qt6)
    find_package(Qt5)
    message(FATAL_ERROR "Satisfying Qt version (6 or 5) is not found")
endif ()

set(QT_LIB Qt${QT_VERSION})
set(QT_COMPONENTS Widgets Multimedia MultimediaWidgets)
set(QT_LIBS ${QT_COMPONENTS})
LIST(TRANSFORM QT_LIBS PREPEND ${QT_LIB}::)

find_package(${QT_LIB} COMPONENTS ${QT_COMPONENTS} REQUIRED)
message(STATUS "Found ${QT_LIB}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS UI)
set(CMAKE_AUTORCC ON)


# Python
find_package(Python COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python_INCLUDE_DIRS})


# Targeting
set(SOURCES
        main.cpp
        App.cpp App.h
        MainWindow.cpp MainWindow.h

        Panels/PanelContainers.cpp Panels/PanelContainers.h

        Panels/InfoPanel.cpp Panels/InfoPanel.h
        Panels/DataListPanel.cpp Panels/DataListPanel.h
        Panels/DataViewPanel.cpp Panels/DataViewPanel.h

        Tabs/Tab.h

        Tabs/HomeTab.h

        Tabs/ImageView.cpp Tabs/ImageView.h
        Tabs/ImageViewerPanels.cpp Tabs/ImageViewerPanels.h
        Tabs/ImageViewer.cpp Tabs/ImageViewer.h

        Tabs/CameraTab.cpp Tabs/CameraTab.h

        TextData.cpp TextData.h
        recognition.cpp recognition.h
        )


if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_executable(${PROJECT_NAME} ${SOURCES})
else ()
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
endif ()
target_link_libraries(${PROJECT_NAME} ${QT_LIBS} ${Python_LIBRARIES})

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Version.txt "Compiler:
${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID} ${CMAKE_CXX_COMPILER_VERSION}

Project library versions
Qt:\t${${QT_LIB}_VERSION}
Python:\t${Python_VERSION}
")


# Installation
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

file(TO_CMAKE_PATH "${CMAKE_PREFIX_PATH}" CMAKE_PREFIX_PATH)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES README.md ${CMAKE_CURRENT_BINARY_DIR}/Version.txt recognition/requirements.txt DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY python/ DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN Lib EXCLUDE PATTERN Scripts EXCLUDE)
if (INSTALL_PYTHON_LIBRARIES)
    install(DIRECTORY python/Lib python/Scripts DESTINATION ${CMAKE_INSTALL_PREFIX})
endif ()
install(DIRECTORY recognition DESTINATION ${CMAKE_INSTALL_PREFIX})

find_file(WINDEPLOYQT windeployqt.exe PATHS ${CMAKE_INSTALL_PREFIX} ${QT${QT_VERSION}_INSTALL_PREFIX} PATH_SUFFIXES bin)
if (NOT DEFINED WINDEPLOYQT OR WINDEPLOYQT MATCHES "WINDEPLOYQT-NOTFOUND" OR NOT EXISTS ${WINDEPLOYQT})
    message(WARNING "windeployqt is not found: \"${WINDEPLOYQT}\"")
endif ()

install(CODE "execute_process(COMMAND ${WINDEPLOYQT} ${CMAKE_INSTALL_PREFIX})")

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message("${_variableName}=${${_variableName}}")
#endforeach()
