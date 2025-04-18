cmake_minimum_required(VERSION 3.5)

project(QImageViewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Svg LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Svg LinguistTools REQUIRED)

include_directories(SYSTEM libs/tinyxml2 libs/tinyexif)

set(TS_FILES QImageViewer_zh_CN.ts)

set(TINY_XML2_FILES
    libs/tinyxml2/tinyxml2.h
    libs/tinyxml2/tinyxml2.cpp
    )

set(TINY_EXIF_FILES
    libs/tinyexif/TinyEXIF.h
    libs/tinyexif/TinyEXIF.cpp
    )

set(PROJECT_SOURCES
        filenavigator.cpp
        filenavigator.h
        imagemetainfo.h
        imagemetainfo.cpp
        imagemetainfomodel.h
        imagemetainfomodel.cpp
        imagewidget.cpp
        imagewidget.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TINY_EXIF_FILES}
        ${TINY_XML2_FILES}
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QImageViewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(QImageViewer SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(QImageViewer
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(QImageViewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg)

set_target_properties(QImageViewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QImageViewer)
endif()
