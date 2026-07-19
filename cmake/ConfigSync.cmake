function(setup_config_sync TARGET_NAME)
    set(CONFIG_SRC_FILE "${CMAKE_SOURCE_DIR}/config/config.json")
    set(CONFIG_DST_DIR "${CMAKE_BINARY_DIR}/config")
    set(CONFIG_DST_FILE "${CONFIG_DST_DIR}/config.json")

    set(SYNC_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/SyncConfig_${TARGET_NAME}.cmake")

    file(WRITE "${SYNC_SCRIPT}" "
    if(EXISTS \"${CONFIG_SRC_FILE}\")
        file(MAKE_DIRECTORY \"${CONFIG_DST_DIR}\")
        execute_process(COMMAND \${CMAKE_COMMAND} -E copy_if_different \"${CONFIG_SRC_FILE}\" \"${CONFIG_DST_FILE}\")
    else()
        if(EXISTS \"${CONFIG_DST_FILE}\")
            file(REMOVE \"${CONFIG_DST_FILE}\")
        endif()
    endif()
    ")

    set(SYNC_TARGET "UpdateEngineConfig_${TARGET_NAME}")

    add_custom_target(
            ${SYNC_TARGET}
            COMMAND ${CMAKE_COMMAND} -P "${SYNC_SCRIPT}"
    )
    
    add_dependencies(${TARGET_NAME} ${SYNC_TARGET})
endfunction()