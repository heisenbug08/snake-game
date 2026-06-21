cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 99)

project(snakeR C)

# Source files
file(GLOB SRCS "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(snakeR WIN32 ${SRCS})

# Include directories
target_include_directories(snakeR PRIVATE
	"${CMAKE_SOURCE_DIR}/raylib-6.0/include"
)

# Platform-specific raylib locations
if(WIN32)
	target_link_directories(snakeR PRIVATE
		"${CMAKE_SOURCE_DIR}/raylib-6.0/lib/win64_mingw-w64"
	)
	target_link_libraries(snakeR PRIVATE
		raylib
		m
		winmm
	)
elseif(UNIX AND NOT APPLE)
	target_link_directories(snakeR PRIVATE
		"${CMAKE_SOURCE_DIR}/raylib-6.0/lib/linux_amd64"
	)
	target_link_libraries(snakeR PRIVATE
		raylib
		m
		GL
		X11
	)
endif()
