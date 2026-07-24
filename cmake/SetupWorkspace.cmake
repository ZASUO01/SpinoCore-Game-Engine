function(setup_workspace TARGET_NAME DIR_PATH README_CONTENT)
    get_filename_component(ABS_SRC "${DIR_PATH}" ABSOLUTE BASE_DIR "${CMAKE_SOURCE_DIR}")
    get_filename_component(ABS_DST "${DIR_PATH}" ABSOLUTE BASE_DIR "${CMAKE_BINARY_DIR}")

    string(MAKE_C_IDENTIFIER "${TARGET_NAME}_${ABS_SRC}" TARGET_ID)
    set(WORKER_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/SyncWorker_${TARGET_ID}.cmake")
    set(SYNC_TARGET "SyncWorkspace_${TARGET_ID}")

    file(WRITE "${WORKER_SCRIPT}" "
        if(NOT EXISTS \"${ABS_SRC}\")
            file(MAKE_DIRECTORY \"${ABS_SRC}\")
            file(WRITE \"${ABS_SRC}/README.txt\" [[${README_CONTENT}]])
        endif()

        file(REMOVE_RECURSE \"${ABS_DST}\")
        file(COPY \"${ABS_SRC}/\" DESTINATION \"${ABS_DST}\")
    ")

    execute_process(COMMAND ${CMAKE_COMMAND} -P "${WORKER_SCRIPT}")

    add_custom_target(
            ${SYNC_TARGET}
            COMMAND ${CMAKE_COMMAND} -P "${WORKER_SCRIPT}"
    )

    add_dependencies(${TARGET_NAME} ${SYNC_TARGET})
endfunction()