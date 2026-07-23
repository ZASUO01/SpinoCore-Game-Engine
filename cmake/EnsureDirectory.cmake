function(ensure_directory DIR_PATH README_CONTENT)
    get_filename_component(ABS_DIR "${DIR_PATH}" ABSOLUTE)

    if(NOT EXISTS "${ABS_DIR}")
        file(MAKE_DIRECTORY "${ABS_DIR}")

        if(NOT "${README_CONTENT}" STREQUAL "")
            file(WRITE "${ABS_DIR}/_README.md" "${README_CONTENT}")
        endif()
    endif()
endfunction()