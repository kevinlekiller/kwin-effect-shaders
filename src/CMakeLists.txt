set(QT_MIN_VERSION "5.4.0")
set(KF5_MIN_VERSION "5.9.0")

include(KDEInstallDirs)
include(KDECMakeSettings)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS KIO GlobalAccel I18n)

set(effect_SRCS
    plugin.cc
    Shaders.cpp
)

add_library(${SHADERS} SHARED ${effect_SRCS})

target_link_libraries(${SHADERS}
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

