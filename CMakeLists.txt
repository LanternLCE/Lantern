set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

cmake_minimum_required(VERSION 3.30.5)
project(Lantern)
set (CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/lantern/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/lantern/*.h)

set(SRC_FILES
	resource.h 
	Resource.rc

	${SOURCE_FILES}
	#${HEADER_FILES}
)

include_directories(lantern ${CMAKE_SOURCE_DIR}/lib/detours/include)

include(GenerateExportHeader)

add_subdirectory(lib/detours/)

add_library(Lantern SHARED ${SRC_FILES})

generate_export_header(Lantern)

set_target_properties(Lantern
  PROPERTIES
  DEFINE_SYMBOL "LANTERN_EXPORTS"
  )

target_link_libraries(Lantern PUBLIC lib_detours)