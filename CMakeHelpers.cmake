cmake_minimum_required(VERSION 3.3)


macro(list_sources OUTPUT DIRECTORY)
    file(GLOB_RECURSE ALL_SOURCES "${DIRECTORY}/*.c*")
    list(APPEND ${OUTPUT} ${ALL_SOURCES})
	list(FILTER ${OUTPUT} EXCLUDE REGEX "_platform")
    list(FILTER ${OUTPUT} EXCLUDE REGEX "windows")
    list(FILTER ${OUTPUT} EXCLUDE REGEX "linux")
	list(FILTER ${OUTPUT} EXCLUDE REGEX ".ignore")

    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
		list(FILTER ALL_SOURCES INCLUDE REGEX "windows")
	elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		list(FILTER ALL_SOURCES INCLUDE REGEX "linux")
	endif()

	list(APPEND ${OUTPUT} ${ALL_SOURCES})
endmacro()


macro(list_subdirs OUTPUT DIRECTORY)
    file(GLOB children RELATIVE "${DIRECTORY}" "${DIRECTORY}/*")
    set(subdirs "")
    foreach(child ${children})
        if(IS_DIRECTORY "${DIRECTORY}/${child}")
            list(APPEND subdirs ${child})
        endif()
    endforeach()
    set(${OUTPUT} ${subdirs})
endmacro()


macro(add_submodule NAME INCLUDE_DIR LIBNAME)
    set("${NAME}_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${NAME}")
    add_subDIRECTORY(${${NAME}_DIR})
    list(APPEND INCLUDES "${${NAME}_DIR}/${INCLUDE_DIR}")
    list(APPEND LINK_LIBRARIES "${LIBNAME}")
endmacro()
