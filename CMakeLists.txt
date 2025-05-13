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

# Find Boost packages
find_package(Boost REQUIRED COMPONENTS system)

# Set automatic MOC, UIC, and RCC processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

# Add source files
set(SOURCES
    src/main.cpp
    # Models
    src/models/current_network_device.cpp
    src/models/device_list_model.cpp
    src/models/network_device.cpp
    # Views
    src/views/main_window.cpp
    src/views/abstract/abstract_view.cpp
    src/views/components/button_panel.cpp
    src/views/components/info_bar.cpp
    src/views/components/scan_result.cpp
    # Controllers
    src/controllers/device_controller.cpp
    src/controllers/scan_controller.cpp
    # Services
    src/services/network_scanner.cpp
)

# Add resource files
set(HEADERS
    # Models
    src/models/current_network_device.h
    src/models/device_list_model.h
    src/models/network_device.h
    # Views
    src/views/abstract/abstract_view.h
    src/views/main_window.h
    src/views/components/button_panel.h
    src/views/components/info_bar.h
    src/views/components/scan_result.h
    # Controllers
    src/controllers/device_controller.h  
    src/controllers/scan_controller.h
    # Services
    src/services/network_scanner.h
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

# Link Boost libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::system pthread
)

# Enable testing
enable_testing()