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
	vector3.c vector3.h
	vector4.h
	timer.c timer.h
	raylib_wrapper.c raylib_wrapper.h
	raymath_wrapper.c raymath_wrapper.h
	tween.c tween.h
	list.c list.h
	debug.c debug.h
	cleanup.c cleanup.h
	edit_mode.c edit_mode.h
	game_manager.c game_manager.h
)

set(WINDOW_FILES
	window.c window.h
)

set(GRAPHICS_FILES
	graphics.c graphics.h
	image.c image.h
	screen_capture.c screen_capture.h
	color.h
	deferred_rendering.h
	draw_order.h
	texture_atlas.c texture_atlas.h
	shader.c shader.h
)

set(INPUT_FILES
	input_button.h
	input.c input.h
)

set(SCENE_FILES
	scene_type.h
	scene.c scene.h
	main_char_scene.c main_char_scene.h
	main_char_foot_scene.c main_char_foot_scene.h
	main_char_eyes_scene.c main_char_eyes_scene.h
	empty_scene.c empty_scene.h
	block_scene.c block_scene.h
	collider_scene.c collider_scene.h
	root_scene.c root_scene.h
	raycast_scene.c raycast_scene.h
	main_char_gun_scene.c main_char_gun_scene.h
	main_char_jetpack_fire_scene.c main_char_jetpack_fire_scene.h
	main_char_bullet_scene.c main_char_bullet_scene.h
	sprite_scene.c sprite_scene.h
	animated_sprite_scene.c animated_sprite_scene.h
	spring_arm_scene.c spring_arm_scene.h
	point_light_scene.c point_light_scene.h
	tumor_scene.c tumor_scene.h
	splash_scene.c splash_scene.h
	spark_scene.c spark_scene.h
)

set (SCENE_UI_FILES
	button_scene.c button_scene.h
	stack_container_scene.c stack_container_scene.h
	label_scene.c label_scene.h
	exit_menu_scene.c exit_menu_scene.h
)

set(PHYSICS_FILES
	collider.c collider.h
	transform.h
	raycast.c raycast.h
	matrix.c matrix.h
)

list(TRANSFORM SRC_FILES PREPEND "src/")
list(TRANSFORM WINDOW_FILES PREPEND "src/window/")
list(TRANSFORM GRAPHICS_FILES PREPEND "src/graphics/")
list(TRANSFORM INPUT_FILES PREPEND "src/input/")
list(TRANSFORM SCENE_FILES PREPEND "src/scene/")
list(TRANSFORM SCENE_UI_FILES PREPEND "src/scene/ui/")
list(TRANSFORM PHYSICS_FILES PREPEND "src/physics/")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${WINDOW_FILES} ${GRAPHICS_FILES} ${INPUT_FILES} ${SCENE_FILES} ${SCENE_UI_FILES} ${PHYSICS_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror=return-type)

### Add include directories here ###
target_include_directories(${PROJECT_NAME} PRIVATE external src external/minifb/include external/stb external/raylib/include)

### Uncomment this line if you want the standard libraries to be linked statically ###
# target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
# target_link_options(${PROJECT_NAME} PRIVATE -static)

### Use this line to link external libraries
target_link_libraries(${PROJECT_NAME} raylib)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} winmm)
elseif(LINUX)
	target_link_libraries(${PROJECT_NAME} m)
endif()

### Add link directories here ###
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/raylib/lib)
