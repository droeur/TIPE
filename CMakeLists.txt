####################
#      Global      #
####################

cmake_minimum_required(VERSION 3.26)
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
    
)

# Project setup
project(${LOCAL_PROJECT_NAME}
        VERSION ${LOCAL_PROJECT_VERSION}
        DESCRIPTION ${LOCAL_PROJECT_DESCRIPTION}
        LANGUAGES CXX)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt.user)
    include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt.user)
endif()

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


#glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE OpenGL::GL)


#SDL3
set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
set( SDL_LIBC ON CACHE BOOL "" FORCE )
set( SDL_TEST OFF CACHE BOOL "" FORCE )

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${LOCAL_PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

find_package(SDL2_ttf CONFIG REQUIRED)
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)

#boost
find_package(Boost 1.81.0 COMPONENTS program_options log REQUIRED)
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})