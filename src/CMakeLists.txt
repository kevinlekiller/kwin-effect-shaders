#######################################
# Effect

find_package (KF5 REQUIRED COMPONENTS
    GlobalAccel
    I18n
)

set(effect_SRCS
    plugin.cc
    Shaders.cpp
)

kconfig_add_kcfg_files(effect_SRCS ShadersConfig.kcfgc)

add_library(${SHADERS} SHARED ${effect_SRCS})

target_link_libraries(${SHADERS}
    PUBLIC
        ${KWIN_EFFECTS}
        ${KWIN_GLUTILS}

    PRIVATE
        KF5::GlobalAccel
        KF5::I18n
        KF5::XmlGui
        KF5::WindowSystem
)

execute_process(COMMAND kf5-config --install data OUTPUT_VARIABLE DATAPATH OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND kf5-config --install module OUTPUT_VARIABLE MODULEPATH OUTPUT_STRIP_TRAILING_WHITESPACE)

install(
    TARGETS
        ${SHADERS}
    DESTINATION
        ${MODULEPATH}/kwin/effects/plugins/
)
install(DIRECTORY shaders DESTINATION ${DATAPATH}/kwin)

add_subdirectory(kcm)
