if (FETCHED_YAML)

	set(yaml-cpp_FOUND TRUE)
	set(YAML_CPP_LIBRARIES yaml-cpp)
	set(yaml-cpp_LIBRARIES yaml-cpp)

	if(NOT TARGET yaml-cpp::yaml-cpp AND TARGET yaml-cpp)
	    add_library(yaml-cpp::yaml-cpp ALIAS yaml-cpp)
	endif()

endif (FETCHED_YAML)
