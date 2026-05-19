cmake_minimum_required(VERSION 3.15)
project(TimerZone VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    main.cpp
    src/network.cpp
    src/app.cpp
    src/ui.cpp
)

# Create executable
# The WIN32 flag prevents a console window from popping up on Windows
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    "C:/raylib/raylib/src"
)

# Library directories
target_link_directories(${PROJECT_NAME} PRIVATE
    "C:/raylib/raylib/src"
)

# Link necessary libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    opengl32
    gdi32
    winmm
    curl
    ws2_32
    wldap32
    crypt32
    
)

# Copy the Res folder to the build directory so assets load correctly
file(COPY ${CMAKE_SOURCE_DIR}/Res DESTINATION ${CMAKE_BINARY_DIR})

