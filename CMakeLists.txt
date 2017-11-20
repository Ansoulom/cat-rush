cmake_minimum_required(VERSION 3.6.2 FATAL_ERROR)

set(PROJ_NAME "CatRush")
project(${PROJ_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
	file(COPY "${PROJ_NAME}.sln.DotSettings" DESTINATION ${CMAKE_BINARY_DIR})
endif(MSVC)

set(PROJ_PATH ${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJ_PATH}/cmake")
set(CMAKE_BINARY_DIR "${PROJ_PATH}/Build")
set(PROJ_OUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(PROJ_LIBRARY_PATH "${PROJ_PATH}/Libraries/mingw")
	set(Boost_COMPILER "-mgw71") # Boost was compiled with -mgw71 for Windows
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(PROJ_LIBRARY_PATH "${PROJ_PATH}/Libraries/msvc") # Boost directory for -vc140 for Windows
	set(Boost_COMPILER "-vc140")
endif()

set(GAME_HEADER_DIR "${PROJ_PATH}/Source code/Game/Header files")
set(GAME_SOURCE_DIR "${PROJ_PATH}/Source code/Game/Source files")

set(PROJ_INCLUDE_DIRS
"${GAME_HEADER_DIR}"
"${GAME_HEADER_DIR}/Audio"
"${GAME_HEADER_DIR}/Debug"
"${GAME_HEADER_DIR}/Geometry"
"${GAME_HEADER_DIR}/Graphics"
"${GAME_HEADER_DIR}/Input"
"${GAME_HEADER_DIR}/IO"
"${GAME_HEADER_DIR}/Libraries"
"${GAME_HEADER_DIR}/Main"
"${GAME_HEADER_DIR}/Math"
"${GAME_HEADER_DIR}/Objects"
"${GAME_HEADER_DIR}/Objects/Graphics components"
"${GAME_HEADER_DIR}/Objects/Input components"
"${GAME_HEADER_DIR}/Objects/Physics components"
"${GAME_HEADER_DIR}/Physics"
"${GAME_HEADER_DIR}/Text"
"${GAME_HEADER_DIR}/Time"
"${GAME_HEADER_DIR}/UI"
"${GAME_HEADER_DIR}/Utility"
"${GAME_HEADER_DIR}/Wrappers"
)

set(PROJ_HEADERS 
"${GAME_HEADER_DIR}/Audio/Audio_player.h"
"${GAME_HEADER_DIR}/Audio/Sound.h"
"${GAME_HEADER_DIR}/Debug/Logger.h"
"${GAME_HEADER_DIR}/Geometry/Shapes.h"
"${GAME_HEADER_DIR}/Geometry/Vector.h"
"${GAME_HEADER_DIR}/Graphics/Animation.h"
"${GAME_HEADER_DIR}/Graphics/Colors.h"
"${GAME_HEADER_DIR}/Graphics/Render_instance.h"
"${GAME_HEADER_DIR}/Graphics/Renderer.h"
"${GAME_HEADER_DIR}/Graphics/Texture.h"
"${GAME_HEADER_DIR}/Input/Input_context.h"
"${GAME_HEADER_DIR}/Input/Input_handler.h"
"${GAME_HEADER_DIR}/Input/Input_types.h"
"${GAME_HEADER_DIR}/IO/File_paths.h"
"${GAME_HEADER_DIR}/IO/INI.h"
"${GAME_HEADER_DIR}/IO/JSON_extensions.h"
"${GAME_HEADER_DIR}/Libraries/INIReader.h"
"${GAME_HEADER_DIR}/Libraries/json.hpp"
"${GAME_HEADER_DIR}/Libraries/tinyxml2.h"
"${GAME_HEADER_DIR}/Main/Camera.h"
"${GAME_HEADER_DIR}/Main/Game_core.h"
"${GAME_HEADER_DIR}/Main/Playing_state.h"
"${GAME_HEADER_DIR}/Main/Resource_managers.h"
"${GAME_HEADER_DIR}/Main/Service_locator.h"
"${GAME_HEADER_DIR}/Main/Settings.h"
"${GAME_HEADER_DIR}/Main/Text_state.h"
"${GAME_HEADER_DIR}/Main/Window.h"
"${GAME_HEADER_DIR}/Main/World.h"
"${GAME_HEADER_DIR}/Math/Math_functions.h"
"${GAME_HEADER_DIR}/Objects/Graphics components/Animated_graphics_component.h"
"${GAME_HEADER_DIR}/Objects/Graphics components/Graphics_component.h"
"${GAME_HEADER_DIR}/Objects/Graphics components/Static_graphics_component.h"
"${GAME_HEADER_DIR}/Objects/Input components/Combat_component.h"
"${GAME_HEADER_DIR}/Objects/Input components/Noodles_ai_component.h"
"${GAME_HEADER_DIR}/Objects/Input components/Player_input_component.h"
"${GAME_HEADER_DIR}/Objects/Physics components/Acceleration_component.h"
"${GAME_HEADER_DIR}/Objects/Physics components/Collider_component.h"
"${GAME_HEADER_DIR}/Objects/Physics components/Movement_component.h"
"${GAME_HEADER_DIR}/Objects/Component.h"
"${GAME_HEADER_DIR}/Objects/Game_object.h"
"${GAME_HEADER_DIR}/Physics/Collision_system.h"
"${GAME_HEADER_DIR}/Text/Fonts.h"
"${GAME_HEADER_DIR}/Text/Text_functions.h"
"${GAME_HEADER_DIR}/Time/Frame_timer.h"
"${GAME_HEADER_DIR}/Time/Timer.h"
"${GAME_HEADER_DIR}/UI/HUD.h"
"${GAME_HEADER_DIR}/Utility/Communication.h"
"${GAME_HEADER_DIR}/Utility/Randomness.h"
"${GAME_HEADER_DIR}/Wrappers/SDL_deleter.h"
"${GAME_HEADER_DIR}/Wrappers/SDL_wrapper.h"
)

set(PROJ_SOURCES 
"${GAME_SOURCE_DIR}/Audio/Audio_player.cpp"
"${GAME_SOURCE_DIR}/Audio/Sound.cpp"
"${GAME_SOURCE_DIR}/Debug/Logger.cpp"
"${GAME_SOURCE_DIR}/Graphics/Animation.cpp"
"${GAME_SOURCE_DIR}/Graphics/Colors.cpp"
"${GAME_SOURCE_DIR}/Graphics/Render_instance.cpp"
"${GAME_SOURCE_DIR}/Graphics/Renderer.cpp"
"${GAME_SOURCE_DIR}/Graphics/Texture.cpp"
"${GAME_SOURCE_DIR}/Input/Input_context.cpp"
"${GAME_SOURCE_DIR}/Input/Input_handler.cpp"
"${GAME_SOURCE_DIR}/Input/Input_types.cpp"
"${GAME_SOURCE_DIR}/IO/File_paths.cpp"
"${GAME_SOURCE_DIR}/IO/INI.cpp"
"${GAME_SOURCE_DIR}/IO/JSON_extensions.cpp"
"${GAME_SOURCE_DIR}/Libraries/tinyxml2.cpp"
"${GAME_SOURCE_DIR}/Main/Camera.cpp"
"${GAME_SOURCE_DIR}/Main/Game_core.cpp"
"${GAME_SOURCE_DIR}/Main/Main.cpp"
"${GAME_SOURCE_DIR}/Main/Playing_state.cpp"
"${GAME_SOURCE_DIR}/Main/Resource_managers.cpp"
"${GAME_SOURCE_DIR}/Main/Service_locator.cpp"
"${GAME_SOURCE_DIR}/Main/Settings.cpp"
"${GAME_SOURCE_DIR}/Main/Text_state.cpp"
"${GAME_SOURCE_DIR}/Main/Window.cpp"
"${GAME_SOURCE_DIR}/Main/World.cpp"
"${GAME_SOURCE_DIR}/Objects/Graphics components/Animated_graphics_component.cpp"
"${GAME_SOURCE_DIR}/Objects/Graphics components/Graphics_component.cpp"
"${GAME_SOURCE_DIR}/Objects/Graphics components/Static_graphics_component.cpp"
"${GAME_SOURCE_DIR}/Objects/Input components/Combat_component.cpp"
"${GAME_SOURCE_DIR}/Objects/Input components/Noodles_ai_component.cpp"
"${GAME_SOURCE_DIR}/Objects/Input components/Player_input_component.cpp"
"${GAME_SOURCE_DIR}/Objects/Physics components/Acceleration_component.cpp"
"${GAME_SOURCE_DIR}/Objects/Physics components/Collider_component.cpp"
"${GAME_SOURCE_DIR}/Objects/Physics components/Movement_component.cpp"
"${GAME_SOURCE_DIR}/Objects/Component.cpp"
"${GAME_SOURCE_DIR}/Objects/Game_object.cpp"
"${GAME_SOURCE_DIR}/Physics/Collision_system.cpp"
"${GAME_SOURCE_DIR}/Text/Fonts.cpp"
"${GAME_SOURCE_DIR}/Text/Text_functions.cpp"
"${GAME_SOURCE_DIR}/Time/Frame_timer.cpp"
"${GAME_SOURCE_DIR}/Time/Timer.cpp"
"${GAME_SOURCE_DIR}/UI/HUD.cpp"
"${GAME_SOURCE_DIR}/Utility/Randomness.cpp"
"${GAME_SOURCE_DIR}/Wrappers/SDL_deleter.cpp"
"${GAME_SOURCE_DIR}/Wrappers/SDL_wrapper.cpp"
)

add_executable(${PROJ_NAME} ${PROJ_SOURCES} ${PROJ_HEADERS})
include_directories("${PROJ_INCLUDE_DIRS}")


# Boost stuff
add_definitions(-DBOOST_ALL_NO_LIB)
set(BOOST_ROOT "${PROJ_LIBRARY_PATH}/boost")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.60.0 REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJ_NAME} ${Boost_LIBRARIES})

message("LIB PATH: ${PROJ_LIBRARY_PATH}")
# SDL stuff
set(SDL2_PATH "${PROJ_LIBRARY_PATH}/SDL2")
set(SDL2_IMAGE_PATH "${PROJ_LIBRARY_PATH}/SDL2_image")
set(SDL2_TTF_PATH "${PROJ_LIBRARY_PATH}/SDL2_ttf")
set(SDL2_MIXER_PATH "${PROJ_LIBRARY_PATH}/SDL2_mixer")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
message("SDL found: ${SDL2_FOUND}")
message(${SDL2_INCLUDE_DIR})
message(${SDL2_LIBRARY})
include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR}
					${SDL2_MIXER_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${SDL2_LIBRARY}
                             ${SDL2_IMAGE_LIBRARIES}
                             ${SDL2_TTF_LIBRARIES}
							 ${SDL2_MIXER_LIBRARIES})

							 
# Place dll files next to the executable
file(GLOB_RECURSE DLL_FILES "${PROJ_LIBRARY_PATH}/*.dll")
foreach(DLL_FILE ${DLL_FILES})
	add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
						${DLL_FILE}
						$<TARGET_FILE_DIR:${PROJ_NAME}>)
endforeach()

# Copy resources to the build directory
add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						"${PROJ_PATH}/Resources"
						$<TARGET_FILE_DIR:${PROJ_NAME}>/Resources)

# Copy preferences to the build directory
add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						"${PROJ_PATH}/Preferences"
						$<TARGET_FILE_DIR:${PROJ_NAME}>/Preferences)

# Copy world files to the build directory
add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						"${PROJ_PATH}/World"
						$<TARGET_FILE_DIR:${PROJ_NAME}>/World)
						
# Copy world files to the build directory
add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						"${PROJ_PATH}/Prefabs"
						$<TARGET_FILE_DIR:${PROJ_NAME}>/Prefabs)