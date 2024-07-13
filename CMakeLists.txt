cmake_minimum_required(VERSION 3.28)
project(PDFGlimpseManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 打印系统类型
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")


# using vcpkg
message(STATUS "VCPKG_ROOT_DIR: ${VCPKG_ROOT_DIR}")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT_DIR}/scripts/buildsystems/vcpkg.cmake")

# 源码获取变量
ADD_DEFINITIONS(-DPROJECT_DIR="${PROJECT_SOURCE_DIR}")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(CMAKE_PREFIX_PATH "D:/software/Qt/6.7.2/msvc2019_64")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    # set(CMAKE_PREFIX_PATH "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    # set(CMAKE_PREFIX_PATH "/Users//Qt/6.5.3/macos")
endif()

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
)

message(STATUS "Qt6 Core: ${Qt6Core_DIR}")
message(STATUS "Qt6 Gui: ${Qt6Gui_DIR}")
message(STATUS "Qt6 Widgets: ${Qt6Widgets_DIR}")


set(QRC_SOURCE_FILE resources/application.qrc)
qt_add_resources(QRC_FILES ${QRC_SOURCE_FILE})

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(SOURCES
        main.cpp
        mainwindow.cpp
        ${QRC_FILES}
)
if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        # Windows的Release模式下使用WIN32选项生成窗口应用程序
        add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
    else()
        add_executable(${PROJECT_NAME} ${SOURCES})
    endif()
elseif(APPLE)
    message(STATUS "APPLE")
else()
    message(STATUS "LINUX")
endif()

# 链接静态库
target_link_libraries(PDFGlimpseManager
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        fmt::fmt
        spdlog::spdlog
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    message(STATUS "QT_INSTALL_PATH: ${QT_INSTALL_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}OpenGL${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

    # 复制vcpkg安装的DLLs
    foreach (VCPKG_LIB fmt spdlog)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${VCPKG_ROOT_DIR}/installed/x64-windows/bin/${VCPKG_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach ()
endif ()
