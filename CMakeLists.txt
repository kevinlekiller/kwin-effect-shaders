cmake_minimum_required(VERSION 3.16)

set(SHADERS kwin4_effect_shaders)

project(${SHADERS})

set(QT_MIN_VERSION "5.4.0")
set(KF5_MIN_VERSION "5.9.0")

cmake_policy(SET CMP0054 NEW)

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

include(KDEInstallDirs)
include(KDECMakeSettings)

# required frameworks by Core
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    GuiAddons
    KIO
)

find_package(KWinDBusInterface CONFIG REQUIRED)

find_path(EFFECTS_H kwineffects.h PATH_SUFFIXES kf5)

add_subdirectory(src)
