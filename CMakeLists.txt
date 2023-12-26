cmake_minimum_required(VERSION 3.5)

project(Ev3RemoteControl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

list(APPEND CMAKE_PREFIX_PATH "/opt/ev3dds")
link_directories(AFTER "/opt/ev3dds/lib")

find_package(fastcdr REQUIRED)
find_package(foonathan_memory REQUIRED)
find_package(fastrtps REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set (generated_sources ${CMAKE_CURRENT_BINARY_DIR}/ev3data.cxx ${CMAKE_CURRENT_BINARY_DIR}/ev3dataPubSubTypes.cxx)

add_custom_command(
    OUTPUT  ${generated_sources}
    COMMAND fastddsgen -cs -d ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/ev3data.idl
    DEPENDS ev3data.idl   # Treated as relative to CMAKE_CURRENT_SOURCE_DIR
)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        rccommandpublisher.h
        rccommandpublisher.cpp
        ${generated_sources}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Ev3RemoteControl
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Ev3RemoteControl APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Ev3RemoteControl SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Ev3RemoteControl
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Ev3RemoteControl PRIVATE Qt${QT_VERSION_MAJOR}::Widgets fastrtps fastcdr foonathan_memory)

set_target_properties(Ev3RemoteControl PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Ev3RemoteControl)
endif()
