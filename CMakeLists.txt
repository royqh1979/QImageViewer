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
        dirmodel.cpp
        dirmodel.h
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
        thumbnailview.cpp
        thumbnailview.h
        ${TINY_EXIF_FILES}
        ${TINY_XML2_FILES}
        ${TS_FILES}
)

set(RESOURCES
    icons.qrc
)

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32
        ${PROJECT_SOURCES}
        ${RESOURCES})
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${RESOURCES})
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(QImageViewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg)

target_link_options(QImageViewer PRIVATE
    $<$<CONFIG:MinSizeRel>:-s -static>
)

if(WIN32)
    set(APP_ICON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/imageviewer.ico")
    if(EXISTS ${APP_ICON_PATH})
        set(APP_RC "${CMAKE_CURRENT_BINARY_DIR}/app.rc")
        file(WRITE ${APP_RC} "IDI_ICON1 ICON \"${APP_ICON_PATH}\"\n")
        target_sources(${PROJECT_NAME} PRIVATE ${APP_RC})
    endif()
endif()

set_target_properties(QImageViewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QImageViewer)
endif()
