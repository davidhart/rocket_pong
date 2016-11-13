cmake_minimum_required(VERSION 3.4)
project(rocket_pong)
include ("rocket/common.cmake")

add_subdirectory(rocket)

file(GLOB_RECURSE ROCKET_EXAMPLE_GAME_SOURCES
	RELATIVE ${PROJECT_SOURCE_DIR}
	GROUP
	"src/*.cpp"
	"include/*.h")

add_executable("rocket_pong" ${ROCKET_EXAMPLE_GAME_SOURCES})
target_link_libraries("rocket_pong" "rocket")
target_include_directories("rocket_pong" PRIVATE "include/")
target_include_directories("rocket_pong" PRIVATE "rocket/include/rocket")
define_source_root_groups("src" "src")
define_source_root_groups("include" "include")
