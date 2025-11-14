target_include_directories(sml PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_sources(sml PRIVATE
    sml_algo.c
    sml_string.c
    sml_logger.c
    sml_filesystem.c
)