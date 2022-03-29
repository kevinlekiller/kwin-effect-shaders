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

in vec4 g_Position;
in vec4 g_iTexcoord;
out vec2 g_oTexcoord;
uniform sampler2D g_Texture;
uniform mat4 modelViewProjectionMatrix;
uniform float g_Random;
vec2 g_TextureSize;
vec4 g_SourceSize;

#if ADVANCED_CARTOON_ENABLED == 1
void shader_advanced_cartoon();
#endif

void main() {
    g_oTexcoord = g_iTexcoord.st;
    gl_Position = modelViewProjectionMatrix * g_Position;
    g_TextureSize = textureSize(g_Texture, 0);
    g_SourceSize = vec4(g_TextureSize, 1.0 / g_TextureSize);

    for (int shader = 0; shader <= SHADERS; shader++) {
        switch(SHADER_ORDER[shader]) {
            #if ADVANCED_CARTOON_ENABLED == 1
            case SHADER_ADVANCED_CARTOON:
                shader_advanced_cartoon();
                break;
            #endif
            default:
                break;
        }
    }
}
