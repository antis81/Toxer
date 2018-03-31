project (Toxer)
cmake_minimum_required (VERSION 3.5)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set (PROJECT_VERSION_MAJOR 1)
set (PROJECT_VERSION_MINOR 0)
set (PROJECT_VERSION_PATCH 0)
set (PROJECT_VERSION_TWEAK "pre-alpha")
set (PROJECT_VERSION
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    )
if (NOT PROJECT_VERSION_TWEAK STREQUAL "")
    set (PROJECT_VERSION "${PROJECT_VERSION}-${PROJECT_VERSION_TWEAK}")
endif()
add_definitions (-DTOXER_VERSION="${PROJECT_VERSION}")

set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Qt5 COMPONENTS Core Network Qml Gui Quick REQUIRED)
find_package (sodium REQUIRED)

file (GLOB_RECURSE SCRIPTS "scripts/*.*")
add_custom_target(scripts SOURCES ${SCRIPTS})

qt5_add_resources (RESOURCES res/res.qrc qml/qml.qrc)
set (QML_IMPORT_PATH "${PROJECT_SOURCE_DIR}/qml" CACHE STRING "Qt Creator extra qml import paths")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

add_subdirectory (lib)

find_library(TOX_CORE toxcore)

set (SRC_FILES ${SRC_FILES}
    src/IToxNotify.cpp
    src/main.cpp
    src/Private/ToxBootstrap.cpp
    src/Private/ToxerPrivate.cpp
    src/Private/ToxProfile.cpp
    src/Settings.cpp
    src/Toxer.cpp
    src/ToxTypes.cpp
    )

file (GLOB_RECURSE JS_FILES "qml/*.js")
add_custom_target (js_files SOURCES ${JS_FILES})

file (GLOB_RECURSE QML_FILES "qml/*.qml")
add_custom_target (qml_files SOURCES ${QML_FILES})

file (GLOB_RECURSE QMLDIR_FILES "qml/*qmldir")
add_custom_target (qmldir_files SOURCES ${QMLDIR_FILES})

add_executable (toxer
    ${SRC_FILES}
    ${RESOURCES}
    )

set_property (TARGET toxer APPEND PROPERTY COMPILE_DEFINITIONS
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    QT_NO_CAST_FROM_BYTEARRAY
    )

target_include_directories (toxer PRIVATE
    $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    >)

target_link_libraries (toxer
    Qt5::Quick

    ${TOX_CORE}
    sodium
    )

install (TARGETS
    toxer
    RUNTIME DESTINATION bin COMPONENT Runtime
    LIBRARY DESTINATION lib COMPONENT Runtime
    )
