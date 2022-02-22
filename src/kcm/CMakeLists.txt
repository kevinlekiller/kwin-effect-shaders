#######################################
# Config

set(kcm_SRCS
    ShadersEffectKCM.cc
    plugin.cc
)

kconfig_add_kcfg_files(kcm_SRCS
    ../ShadersConfig.kcfgc
)

qt5_wrap_ui(kcm_SRCS ShadersEffectKCM.ui)

qt5_add_dbus_interface(kcm_SRCS ${KWIN_EFFECTS_INTERFACE} kwineffects_interface)

add_library(kwin_shaders_config MODULE ${kcm_SRCS})

target_link_libraries(kwin_shaders_config
    KF5::ConfigWidgets
)

install(
    TARGETS
        kwin_shaders_config

    DESTINATION
        ${PLUGIN_INSTALL_DIR}/kwin/effects/configs
)
