cmake_minimum_required(VERSION 3.26)

# Set the name of the project (the exec file)
SET(MY_PROJECT_NAME WindowsClientTest)

project(${MY_PROJECT_NAME})

# Set the C++ version
set(CMAKE_CXX_STANDARD 23)

# Set the path to for the local software installation
set(LOCAL_SOFTWARE_DIR /usr/local)
# Include directories for the locally installed header files
include_directories(${LOCAL_SOFTWARE_DIR}/include ${Boost_INCLUDE_DIRS})
# Include link directories for libraries
link_directories(${LOCAL_SOFTWARE_DIR}/lib)

include(FetchContent)

# Fetch httplib
FetchContent_Declare(httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG master
)
FetchContent_MakeAvailable(httplib)

# Fetch nlohmann/json
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG master
)
FetchContent_MakeAvailable(json)

# Source files
set(SOURCES
        ClipboardManager.cpp
        ClipboardManager.h
        main.cpp
        ClipboardContent.cpp
        ClipboardContent.h
        NetworkConnection.cpp
        NetworkConnection.h
)

# Create the executable
add_executable(${MY_PROJECT_NAME} ${SOURCES})

# Link against the libraries
target_link_libraries(${PROJECT_NAME}
        httplib::httplib

        nlohmann_json::nlohmann_json
)

# Set the RPATH to include the library directory
set_target_properties(${MY_PROJECT_NAME} PROPERTIES
        INSTALL_RPATH ${LOCAL_SOFTWARE_DIR}/lib
        INSTALL_RPATH_USE_LINK_PATH TRUE
)
