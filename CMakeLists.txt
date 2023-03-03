cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(TreeBasedGameEngine VERSION 0.8)

set(ENGINE_SOURCE
  main.cpp
  deps/glad.c
  deps/stb_image.cpp
  src/Leaf.cpp
  src/LeafTree.cpp
  src/Seed.cpp
  src/Settings.cpp
  src/Utility.cpp
  src/leaves/AudioPlayer.cpp
  src/leaves/BGLayer.cpp
  src/leaves/Leaf2D.cpp
  src/leaves/Timer.cpp
  src/leaves/Tween.cpp
  src/leaves/UILayer.cpp
  src/leaves/leaves2D/Area2D.cpp
  src/leaves/leaves2D/Camera2D.cpp
  src/leaves/leaves2D/CollisionObject2D.cpp
  src/leaves/leaves2D/CollisionShape2D.cpp
  src/leaves/leaves2D/ColorRect.cpp
  src/leaves/leaves2D/KinematicBody2D.cpp
  src/leaves/leaves2D/Model3D.cpp
  src/leaves/leaves2D/ParticleEmitter.cpp
  src/leaves/leaves2D/PhysicsBody2D.cpp
  src/leaves/leaves2D/Sprite.cpp
  src/leaves/leaves2D/StaticBody2D.cpp
  src/leaves/leaves2D/TextLabel.cpp
  src/leaves/leaves2D/TileMap.cpp
  src/resources/AudioStream.cpp
  src/resources/Font.cpp
  src/resources/Model.cpp
  src/resources/Particle.cpp
  src/resources/Shader.cpp
  src/resources/Texture.cpp
  src/resources/TileSet.cpp
  src/resources/shapes2D/CircleShape2D.cpp
  src/resources/shapes2D/ConvexPolygonShape2D.cpp
  src/resources/shapes2D/LineShape2D.cpp
  src/resources/shapes2D/PolygonShape2D.cpp
  src/resources/shapes2D/RectangleShape2D.cpp
  src/servers/FileReader.cpp
  src/servers/GrassLoader.cpp
  src/servers/Input.cpp
  src/servers/Physics2D.cpp
  src/servers/Renderer.cpp
  src/servers/ResourceManager.cpp
)

#------ADD-PROJECT-SOURCE-FILES-HERE------
set(PROJECT_SOURCE
  project/prj_src/LEAVES.CPP
  project/prj_src/DemoGrass.cpp
  project/prj_src/MainCharacter.cpp
  project/prj_src/Enemy.cpp
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set (WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/project/res.rc)
endif()

add_executable(TreeBasedGameEngine ${ENGINE_SOURCE} ${PROJECT_SOURCE} ${WIN32_RESOURCES})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wl,-subsystem,windows")

target_include_directories(TreeBasedGameEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/include)
target_include_directories(TreeBasedGameEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(TreeBasedGameEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/project/prj_src)

find_library(GLFW glfw3 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/)
target_link_libraries(TreeBasedGameEngine PUBLIC ${GLFW})
find_library(GDI32 gdi32)
target_link_libraries(TreeBasedGameEngine PUBLIC ${GDI32})
find_library(OPENGL32 opengl32)
target_link_libraries(TreeBasedGameEngine PUBLIC ${OPENGL32})
find_library(FREETYPE freetype PATHS ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/)
target_link_libraries(TreeBasedGameEngine PUBLIC ${FREETYPE})