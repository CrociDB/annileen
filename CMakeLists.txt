cmake_minimum_required (VERSION 2.6)
project (clonecraft)
set(PROJECT_NAME clonecraft)
set(DIR "build/${CMAKE_SYSTEM_NAME}")
set(EXECUTABLE_OUTPUT_PATH ${DIR})

list(APPEND CMAKE_MODULE_PATH "cmake-scripts")

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
include_directories(libs/glm/)
include_directories(libs/PerlinNoise/)
include_directories(/usr/include/SDL2)
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# adding SDL
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# adding GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})

# adding OpenGL
cmake_policy(SET CMP0072 OLD)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})