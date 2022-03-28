/**
 *  Please see the original licenses / copyright information in individual shader files.
 *
 *  Copyright (C) 2022  kevinlekiller
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *  https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

// AKA Texture or Source in libretro
uniform sampler2D g_Texture;
// Random number generated with drand48() casted to float.
// Only updates when a screen change occurs.
uniform float g_Random;
// AKA TEX0 or vTexCoord in libretro
// The coordinates of the current texture.
in vec2 g_oTexcoord;
// This is the final color of the pixel.
out vec4 g_FragColor;
// Initially set to the current pixel color, at the end of the program g_FragColor is set to this.
vec4 g_Color;
vec4 g_SourceSize;
vec2 g_TextureSize;

void shader_adaptive_sharpen();
void shader_amd_cas();
void shader_deband();
void shader_dpx();
void shader_fake_hdr();
void shader_fast_sharpen();
void shader_fxaa3();
void shader_film_noise();
void shader_gauss_blur_h();
void shader_gauss_blur_v();
void shader_levels();
void shader_lift_gamma_gain();
void shader_natural_vision();
void shader_nvidia_dls();
void shader_sepia();
void shader_technicolor1();
void shader_technicolor2();
void shader_tonemap();
void shader_vibrance();

void main() {
    g_Color = texture(g_Texture, g_oTexcoord).rgba;
    g_TextureSize = textureSize(g_Texture, 0);
    g_SourceSize = vec4(g_TextureSize, 1.0 / g_TextureSize);

    for (int shader = 0; shader <= SHADERS; shader++) {
        switch(SHADER_ORDER[shader]) {
            #if ADAPTIVE_SHARPEN_ENABLED == 1
            case SHADER_FAST_SHARPEN:
                shader_adaptive_sharpen();
                break;
            #endif
            #if CAS_ENABLED == 1
            case SHADER_AMD_CAS:
                shader_amd_cas();
                break;
            #endif
            #if DEBAND_ENABLED == 1
            case SHADER_DEBAND:
                shader_deband();
                break;
            #endif
            #if DPX_ENABLED == 1
            case SHADER_DPX:
                shader_dpx();
                break;
            #endif
            #if FAKEHDR_ENABLED == 1
            case SHADER_FAKE_HDR:
                shader_fake_hdr();
                break;
            #endif
            #if FAST_SHARPEN_ENABLED == 1
            case SHADER_ADAPTIVE_SHARPEN:
                shader_fast_sharpen();
                break;
            #endif
            #if FILM_NOISE_ENABLED == 1
            case SHADER_FILM_NOISE:
                shader_film_noise();
            #endif
            #if FXAA3_ENABLED == 1
            case SHADER_FXAA3:
                shader_fxaa3();
                break;
            #endif
            #if GAUSSBLURH_ENABLED == 1
            case SHADER_GAUSS_BLUR_H:
                shader_gauss_blur_h();
                break;
            #endif
            #if GAUSSBLURV_ENABLED == 1
            case SHADER_GAUSS_BLUR_V:
                shader_gauss_blur_v();
                break;
            #endif
            #if LEVELS_ENABLED == 1
            case SHADER_LEVELS:
                shader_levels();
                break;
            #endif
            #if LIFT_GAMMA_GAIN_ENABLED == 1
            case SHADER_LIFT_GAMMA_GAIN:
                shader_lift_gamma_gain();
                break;
            #endif
            #if NATURAL_VISION_ENABLED == 1
            case SHADER_NATURAL_VISION:
                shader_natural_vision();
                break;
            #endif
            #if NVIDIA_DLS_ENABLED == 1
            case SHADER_NVIDIA_DLS:
                shader_nvidia_dls();
                break;
            #endif
            #if SEPIA_ENABLED == 1
            case SHADER_SEPIA:
                shader_sepia();
                break;
            #endif
            #if TECHNICOLOR1_ENABLED == 1
            case SHADER_TECHNICOLOR1:
                shader_technicolor1();
                break;
            #endif
            #if TECHNICOLOR2_ENABLED == 1
            case SHADER_TECHNICOLOR2:
                shader_technicolor2();
                break;
            #endif
            #if TONEMAP_ENABLED == 1
            case SHADER_TONEMAP:
                shader_tonemap();
                break;
            #endif
            #if VIBRANCE_ENABLED == 1
            case SHADER_VIBRANCE:
                shader_vibrance();
                break;
            #endif
            default:
                break;
        }
    }
    g_FragColor = g_Color;
}
