cmake_minimum_required(VERSION 3.16)
project(network_analyzer VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt configuration
if(NOT CMAKE_PREFIX_PATH)
    if(EXISTS $ENV{HOME}/Qt)
        file(GLOB QT_VERSIONS "$ENV{HOME}/Qt/[0-9]*.[0-9]*.[0-9]*")
        if(QT_VERSIONS)
            list(GET QT_VERSIONS -1 LATEST_QT)
            set(CMAKE_PREFIX_PATH "${LATEST_QT}/gcc_64")
        endif()
    endif()
endif()

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

# Set automatic MOC, UIC, and RCC processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/ui/view
    ${CMAKE_SOURCE_DIR}/src/ui/views
    ${CMAKE_SOURCE_DIR}/src/utils
)

# Add source files
set(SOURCES
    src/main.cpp
    src/ui/windows/main_window.cpp
    src/ui/views/ScanResultView.cpp
    src/ui/views/ButtonPanelView.cpp
    src/ui/views/InfoBarView.cpp
    src/core/network_device.cpp
    src/core/network_scanner.cpp
    src/core/models/device_list_model.cpp
)

# Add resource files
set(RESOURCES
    src/ui/windows/main_window.h
    src/ui/views/AView.h
    src/ui/views/ScanResultView.h
    src/ui/views/ButtonPanelView.h
    src/ui/views/InfoBarView.h
    src/core/network_device.h
    src/core/network_scanner.h
    src/core/models/device_list_model.h
)

# Add executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${RESOURCES}
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)

# Enable testing
enable_testing()