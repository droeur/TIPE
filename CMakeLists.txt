####################
#      Global      #
####################

cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)


#####################
#      Project      #
#####################

# Project variables
set(LOCAL_PROJECT_NAME        "TIPE")
set(LOCAL_PROJECT_VERSION     "0.0.1")
set(LOCAL_PROJECT_DESCRIPTION "Description")

# Header files (relative to "include" directory)
file(GLOB HEADERS
    "include/*.hpp"
)

# Source files (relative to "src" directory)
file(GLOB SOURCES
    "src/*.cpp"
)

# Compiler definitions
set(DEFINES

)

# Compiler options
set(OPTIONS
    -Wall -Wpedantic
)

# Project setup
project(${LOCAL_PROJECT_NAME}
        VERSION ${LOCAL_PROJECT_VERSION}
        DESCRIPTION ${LOCAL_PROJECT_DESCRIPTION}
        LANGUAGES CXX)

add_executable(${LOCAL_PROJECT_NAME})

target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_sources(${LOCAL_PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})
target_compile_definitions(${LOCAL_PROJECT_NAME} PRIVATE ${DEFINES})
target_compile_options(${LOCAL_PROJECT_NAME} PRIVATE ${OPTIONS})

set_target_properties(${LOCAL_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin")



####################
#   Dependencies   #
####################
set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
#rapidcsv pour la map
add_subdirectory(external/rapidcsv)
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE external/rapidcsv/src)


#glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE external/glfw/include)
target_link_directories(${LOCAL_PROJECT_NAME} PRIVATE external/glfw/src)
target_link_libraries(${LOCAL_PROJECT_NAME} glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(${LOCAL_PROJECT_NAME} OpenGL::GL)


#SDL3
set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
set( SDL_LIBC ON CACHE BOOL "" FORCE )
set( SDL_TEST OFF CACHE BOOL "" FORCE )

add_subdirectory(external/sdl)

target_link_libraries(${LOCAL_PROJECT_NAME} SDL3::SDL3)

add_subdirectory(external/sdl-image)

target_link_libraries(${LOCAL_PROJECT_NAME} SDL3_image)

#YAML-CPP
add_subdirectory(external/yaml-cpp)

target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE external/yaml-cpp/include)
target_link_libraries(${LOCAL_PROJECT_NAME} yaml-cpp)