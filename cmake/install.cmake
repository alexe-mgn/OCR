
message("Dependency check")
file(GET_RUNTIME_DEPENDENCIES
    EXECUTABLES ${TARGET_FILE}
    RESOLVED_DEPENDENCIES_VAR RESOLVED_DEPS
    UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPS
)
foreach(_i IN LISTS RESOLVED_DEPS UNRESOLVED_DEPS)
    get_filename_component(_i "${_i}" NAME)
    LIST(APPEND DEPENDENCIES_LIST "${_i}")
endforeach()

message("${DEPENDENCIES_LIST}")

message("Installing dependencies")
foreach(_f IN LISTS DEPENDENCIES_LIST)
    unset(_fp CACHE)
    find_file(_fp "${_f}"
     PATHS ${CMAKE_PREFIX_PATH}
     PATH_SUFFIXES bin
     NO_SYSTEM_ENVIRONMENT_PATH
    )
    if (NOT _fp MATCHES "_fp-NOTFOUND")
        file(INSTALL ${_fp} DESTINATION ${INSTALL_DESTINATION})
    endif()
endforeach()
