file(GLOB LIB_DIRS LIST_DIRECTORIES true "*")

foreach(LIB_DIR ${LIB_DIRS})
  if(IS_DIRECTORY ${LIB_DIR})
	  get_filename_component(LIB_NAME ${LIB_DIR} NAME)
	  file(GLOB_RECURSE LIB_SRC_FILES ${LIB_NAME}/*.cpp)
    add_library(${LIB_NAME} SHARED ${LIB_SRC_FILES})
    target_link_libraries(${EXE} ${LIB_NAME})
	endif()
endforeach()