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
// Only pixels which have changed since the last draw are processed,
// which means if you're doing noise for example, the noise will only apply to the pixels that have changed.
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

#if ADAPTIVE_SHARPEN_ENABLED == 1
void shader_adaptive_sharpen();
#endif
#if ADVANCED_CARTOON_ENABLED == 1
void shader_advanced_cartoon();
#endif
#if CAS_ENABLED == 1
void shader_amd_cas();
#endif
#if DEBAND_ENABLED == 1
void shader_deband();
#endif
#if DPX_ENABLED == 1
void shader_dpx();
#endif
#if FAKEHDR_ENABLED == 1
void shader_fake_hdr();
#endif
#if FAST_SHARPEN_ENABLED == 1
void shader_fast_sharpen();
#endif
#if FXAA3_ENABLED == 1
void shader_fxaa3();
#endif
#if FILM_NOISE_ENABLED == 1
void shader_film_noise();
#endif
#if FAST_GAUSS_BLURV_ENABLED == 1
void shader_fast_gaussian_blurV();
#endif
#if GRAYSCALE_ENABLED == 1
void shader_grayscale();
#endif
#if LEVELS_ENABLED == 1
void shader_levels();
#endif
#if LIFT_GAMMA_GAIN_ENABLED == 1
void shader_lift_gamma_gain();
#endif
#if NATURAL_VISION_ENABLED == 1
void shader_natural_vision();
#endif
#if NVIDIA_DLS_ENABLED == 1
void shader_nvidia_dls();
#endif
#if SEPIA_ENABLED == 1
void shader_sepia();
#endif
#if TECHNICOLOR1_ENABLED == 1
void shader_technicolor1();
#endif
#if TECHNICOLOR2_ENABLED == 1
void shader_technicolor2();
#endif
#if TONEMAP_ENABLED == 1
void shader_tonemap();
#endif
#if VIBRANCE_ENABLED == 1
void shader_vibrance();
#endif

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
            #if ADVANCED_CARTOON_ENABLED == 1
            case SHADER_ADVANCED_CARTOON:
                shader_advanced_cartoon();
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
            #if FAST_GAUSS_BLURV_ENABLED == 1
            case SHADER_FAST_GAUSS_BLURV:
                shader_fast_gaussian_blurV();
                break;
            #endif
            #if GRAYSCALE_ENABLED == 1
            case SHADER_GRAYSCALE:
                shader_grayscale();
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
