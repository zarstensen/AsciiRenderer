function(ConanInstall install_dir)
	# create the directory where the installed packages will be placed
	message(STATUS "Creating conan package directory")
	file(MAKE_DIRECTORY "${install_dir}")
	message(STATUS "Installing conan packages")
	execute_process(COMMAND conan install "${CMAKE_CURRENT_SOURCE_DIR}" --build missing WORKING_DIRECTORY "${install_dir}" RESULT_VARIABLE INSTALL_RESULT)



	if(${INSTALL_RESULT})
		if(${INSTALL_RESULT} EQUAL "The system cannot find the file specified")
			message(SEND_ERROR "Conan could not be found. Please install Conan by running the 'pip install conan' command.")
		else()
			message(SEND_ERROR "Conan install failed with exit code / message: ${INSTALL_RESULT}")
		endif()
	else()
		message(STATUS "Done installing conan packages")
	endif()
endfunction()
