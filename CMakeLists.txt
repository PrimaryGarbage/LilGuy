cmake_minimum_required(VERSION 3.22)
project(LilGuy VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)

### Put the names of your source files here ###
set(SRC_FILES 
    main.c
	logging.c logging.h
	result.h 
	short_types.h
	random.h
	math_helpers.c math_helpers.h
	vector2.c vector2.h
	timer.c timer.h
	screen_capture.c screen_capture.h
)

set(WINDOW_FILES
	window_context.h
	window.c window.h
)

set(GRAPHICS_FILES
	graphics.c graphics.h
	image.c image.h
	color.h
	stb_implementation.c
)

list(TRANSFORM SRC_FILES PREPEND "src/")
list(TRANSFORM WINDOW_FILES PREPEND "src/window/")
list(TRANSFORM GRAPHICS_FILES PREPEND "src/window/graphics/")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${WINDOW_FILES} ${GRAPHICS_FILES})

add_subdirectory(external/minifb)


set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)

### Add include directories here ###
target_include_directories(${PROJECT_NAME} PRIVATE external src external/minifb/include external/stb)

### Uncomment this line if you want the standard libraries to be linked statically ###
# target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
# target_link_options(${PROJECT_NAME} PRIVATE -static)

### Use this line to link external libraries
target_link_libraries(${PROJECT_NAME} minifb)

### Add link directories here ###
# target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/lib)
