cmake_minimum_required(VERSION 3.31)
project(shopfront CXX)

include(CheckIncludeFileCXX)
include(FetchContent)

# C/C++ Settings
set(CMAKE_CXX_STANDARD 23) # Using C++23 instead of C++26 due to GitHub Actions
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build Shared Libs
if (BUILD_SHARED_LIBS)
	message(STATUS "Build Shared Libs ON")
else()
	message(STATUS "Build Shared Libs OFF")
endif()

# Release Type
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	message(STATUS "Standard Debug Build")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	message(STATUS "Standard Release Build")
endif()

# Compile Commands for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Miscellaneous Settings
set(YAML_BUILD_SHARED_LIBS BUILD_SHARED_LIBS CACHE BOOL "Yaml Build Shared Libs" FORCE)
set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "disable building examples" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable dependency examples" FORCE)
set(USE_MYSQL ON CACHE BOOL "Enable Mysql" FORCE)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Disable Yaml tests" FORCE)
set(JSONCPP_WITH_TESTS OFF CACHE BOOL "Disable Json tests" FORCE)
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "Disable Json tests" FORCE)

# Debug Flag for Development
if (BUILD_TYPE EQUAL Debug AND ENABLE_DEBUG_SHOPFRONT)
	message(WARNING "Shopfront Debug Mode Enabled")
	add_compile_definitions(-DENABLE_DEBUG_SHOPFRONT)	
endif ()

# Project Executable
add_executable(${PROJECT_NAME} main.cc)

# Fetch and Add jwt-cpp
fetchcontent_declare(jwt-cpp 
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG b0ea29a58fc852a67d4e896d266880c2c63b0c4c # v0.7.2 release
)
fetchcontent_makeavailable(jwt-cpp) 
target_link_libraries(${PROJECT_NAME} PRIVATE jwt-cpp::jwt-cpp)

# Fetch and Add jsoncpp
fetchcontent_declare(jsoncpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_TAG 8519b8381f3c741ad1421f88237b1deda0b11412 # v1.9.8 release
)
fetchcontent_makeavailable(jsoncpp)
target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp_lib)
set(JSONCPP_INCLUDE_DIRS 
    "${jsoncpp_SOURCE_DIR}/include" 
    "${jsoncpp_BINARY_DIR}/include" 
    CACHE INTERNAL "Jsoncpp include dirs for Drogon"
)
set(JSONCPP_LIBRARIES jsoncpp_lib CACHE INTERNAL "Jsoncpp libraries for Drogon")
# Fetch and Add yaml-cpp
fetchcontent_declare(yaml-cpp 
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG 56e3bb550c91fd7005566f19c079cb7a503223cf # v0.9.0 release
)
fetchcontent_makeavailable(yaml-cpp)
install(TARGETS yaml-cpp EXPORT DrogonTargets)
target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

# Fetch and Add Drogon
fetchcontent_declare(drogon 
    GIT_REPOSITORY https://github.com/drogonframework/drogon.git
    GIT_TAG 4c5430757ea5451a7c38fbbef4b4bef7dbb47f2f # v1.9.13 release
)
fetchcontent_makeavailable(drogon)
target_link_libraries(${PROJECT_NAME} PRIVATE drogon)

# Additional Source Directories 
aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)

# Generate Drogon Views
drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
                    ${CMAKE_CURRENT_BINARY_DIR})

# Include Database Models
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                   ${CMAKE_CURRENT_SOURCE_DIR}/models)

# Add Source Directories to Project
target_sources(${PROJECT_NAME}
               PRIVATE
               ${SRC_DIR}
               ${CTL_SRC}
               ${FILTER_SRC}
               ${PLUGIN_SRC}
               ${MODEL_SRC})

# Enable Unit Testing
if (BUILD_TESTING)
	message(STATUS "unit testing enabled")
	add_subdirectory(test)
endif()
