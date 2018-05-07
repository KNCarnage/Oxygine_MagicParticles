cmake_minimum_required (VERSION 2.6)
project (OXYGINEMAGICPARTICLES)

if (EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "-Wno-warn-absolute-paths")
	message(status emscripten)
	set(OXYGINEMAGICPARTICLES_INCLUDE_DIRS 
		${CMAKE_CURRENT_SOURCE_DIR}/src
		${CMAKE_CURRENT_SOURCE_DIR}/magic_particles/emscripten)

	set(OXYGINEMAGICPARTICLES_LIBS
		${CMAKE_CURRENT_SOURCE_DIR}/magic_particles/emscripten/libmagic.a)

else(EMSCRIPTEN)
	set(OXYGINEMAGICPARTICLES_INCLUDE_DIRS 
		${CMAKE_CURRENT_SOURCE_DIR}/src
		${CMAKE_CURRENT_SOURCE_DIR}/magic_particles/win32)

	set(OXYGINEMAGICPARTICLES_LIBS
		${CMAKE_CURRENT_SOURCE_DIR}/magic_particles/win32/magic.lib)
endif(EMSCRIPTEN)

include_directories(${OXYGINEMAGICPARTICLES_INCLUDE_DIRS})
add_library(oxygine-magicparticles STATIC 
	src/MagicEmitter.cpp 
	src/MagicEmitter.h
	src/ResMagicParticles.cpp 
	src/ResMagicParticles.h
)

#target_link_libraries(oxyginemagicparticles)

set(OXYGINEMAGICPARTICLES_INCLUDE_DIRS ${OXYGINEMAGICPARTICLES_INCLUDE_DIRS} PARENT_SCOPE)
set(OXYGINEMAGICPARTICLES_LIBS ${OXYGINEMAGICPARTICLES_LIBS} PARENT_SCOPE)