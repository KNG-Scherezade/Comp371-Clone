# Based on examples from https://github.com/ruslo/hunter/tree/master/examples

cmake_minimum_required(VERSION 3.8)

# Set HUNTER_ROOT environment variable to local directory if not set already
string(COMPARE EQUAL "$ENV{HUNTER_ROOT}" "" hunter_root_is_empty)
if(hunter_root_is_empty)
    get_filename_component(HUNTER_ROOT
        "${CMAKE_CURRENT_LIST_DIR}/hunter_installs"
        ABSOLUTE
    )
endif()

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.104.tar.gz"
    SHA1 "34a6ae7bc9d6a6599d21c239a60ebc05abe8abb4"
)

project(ProceduralWorld)

add_definitions(-Wno-deprecated-declarations -DCMAKE_BUILD_TYPE=Release -DGLM_FORCE_RADIANS)

#Set the correct output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

## Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s")  ## Strip binary
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Download dependencies
if(NOT APPLE)
    hunter_add_package(glew)
endif()
hunter_add_package(glfw)
hunter_add_package(glm)

# Find dependencies
if(APPLE)
    find_library(OPEN_GL OpenGL)
else()
    find_package(glew CONFIG REQUIRED)
endif()
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)

# Add executables
add_executable(proceduralworld
    src/main.cpp
    src/glsetup.hpp
    src/glsetup.cpp
    src/shaderprogram.hpp
    src/shaderprogram.cpp
    src/objloader.hpp
    src/objloader.cpp
    src/loadTexture.hpp
    src/loadTexture.cpp
    src/utils.hpp
    src/constants.hpp
    src/TreeRandom.cpp
    src/TreeRandom.hpp
    src/HitBox2d.hpp
    src/HitBox2d.cpp
    src/entities/Entity.hpp
    src/entities/Entity.cpp
    src/entities/DrawableEntity.hpp
    src/entities/DrawableEntity.cpp
    src/entities/WorldOrigin.hpp
    src/entities/WorldOrigin.cpp
    src/entities/Player.hpp
    src/entities/Player.cpp
    src/entities/WorldTile.hpp
    src/entities/WorldTile.cpp
    src/entities/World.hpp
    src/entities/World.cpp
    src/entities/Light.h
    src/entities/Rock.hpp
    src/entities/Rock.cpp
    src/entities/RockB.hpp
    src/entities/RockB.cpp
    src/entities/Tentacle.hpp
    src/entities/Tentacle.cpp
    src/entities/Trees/Tree.hpp
    src/entities/Trees/Tree.cpp
    src/entities/Trees/TreeA.hpp
    src/entities/Trees/TreeA.cpp
    src/entities/Trees/TreeA_Autumn.hpp
    src/entities/Trees/TreeA_Autumn.cpp
    src/entities/Trees/TrunkAB.hpp
    src/entities/Trees/TrunkAB.cpp
    src/entities/Trees/LeafAB.hpp
    src/entities/Trees/LeafAB.cpp
    src/entities/Trees/LeafBranchAB.hpp
    src/entities/Trees/LeafBranchAB.cpp
    src/entities/Trees/LeafContainerAB.hpp
    src/entities/Trees/LeafContainerAB.cpp
    src/entities/Trees/TreeB.hpp
    src/entities/Trees/TreeB.cpp
    src/entities/Trees/TreeC.hpp
    src/entities/Trees/TreeC.cpp
    src/entities/Trees/TreeClusterItem.hpp
    src/entities/Trees/TreeClusterItem.cpp
    src/entities/Trees/TrunkC.hpp
    src/entities/Trees/TrunkC.cpp
    src/entities/Trees/LeafContainerC.cpp
    src/entities/Trees/LeafContainerC.hpp
    src/entities/Skybox.cpp
    src/entities/Skybox.hpp
    src/entities/Text.hpp
    src/entities/Text.cpp
)

# Link librarssies
if(APPLE)
    target_link_libraries(proceduralworld ${OPEN_GL})
else()
    target_link_libraries(proceduralworld glew::glew)
endif()
target_link_libraries(proceduralworld glfw)
target_link_libraries(proceduralworld glm)
