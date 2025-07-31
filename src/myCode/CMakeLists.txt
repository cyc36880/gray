cmake_minimum_required(VERSION 3.22)

enable_language(C ASM)

set(Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(Application_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/myMain.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/light.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/myMath.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys_config_and_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/register.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/binary.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/color.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gray.c
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${Include_Dirs})
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${Application_Src})


set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)
