function(directory_sync TARGET_NAME SRC_DIR DST_DIR)
    get_filename_component(ABS_SRC "${SRC_DIR}" ABSOLUTE)
    get_filename_component(ABS_DST "${DST_DIR}" ABSOLUTE)

    string(MAKE_C_IDENTIFIER "${TARGET_NAME}_${ABS_SRC}_${ABS_DST}" TARGET_ID)

    set(SYNC_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/SyncDirectory_${TARGET_ID}.cmake")
    set(SYNC_TARGET "MirrorDirTarget_${TARGET_ID}")

    file(WRITE "${SYNC_SCRIPT}" "
    if(EXISTS \"${ABS_SRC}\")
        file(REMOVE_RECURSE \"${ABS_DST}\")
        file(COPY \"${ABS_SRC}/\" DESTINATION \"${ABS_DST}\")

        file(MAKE_DIRECTORY \"${CONFIG_DST_DIR}\")
        execute_process(COMMAND \${CMAKE_COMMAND} -E copy_if_different \"${CONFIG_SRC_FILE}\" \"${CONFIG_DST_FILE}\")
    else()
        if(EXISTS \"${ABS_DST}\")
            file(REMOVE_RECURSE \"${ABS_DST}\")
        endif()
    endif()
    ")

    add_custom_target(
            ${SYNC_TARGET}
            COMMAND ${CMAKE_COMMAND} -P "${SYNC_SCRIPT}"
            COMMENT "Sync directory ${TARGET_NAME}: ${ABS_SRC} | ${ABS_DST}"
    )
    
    add_dependencies(${TARGET_NAME} ${SYNC_TARGET})
endfunction()