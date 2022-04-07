cmake_minimum_required(VERSION 3.16)

set(SHADERS kwin_effect_shaders)

project(${SHADERS})

find_package(ECM REQUIRED NO_MODULE)

add_definitions (-Wall -Werror)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${ECM_MODULE_PATH}
    ${ECM_KDE_MODULE_DIR}
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_SOURCE_DIR}/cmake/Modules
)

include (FeatureSummary)
feature_summary (WHAT ALL)

add_subdirectory(src)
