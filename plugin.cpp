#include "shaders.h"

KWIN_EFFECT_FACTORY_ENABLED(
                            KWin::ShadersEffect,
                            "shaders.json",
                            return false;)

#include "plugin.moc"
