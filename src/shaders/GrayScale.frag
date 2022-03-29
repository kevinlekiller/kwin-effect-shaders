#if GRAYSCALE_ENABLED == 1
/**
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

uniform vec3 grayScaleMethods[4] = vec3[] (
    vec3(0.2100, 0.7200, 0.0700), // sRGB
    vec3(0.2990, 0.5870, 0.1140), // YUV
    vec3(0.2126, 0.7152, 0.0722), // ITU-R BT.709
    vec3(0.2627, 0.6780, 0.0593)  // ITU-R BT.2100
);

void shader_grayscale() {
    g_Color.rgb = vec3(
        (g_Color.r * grayScaleMethods[GRAYSCALE_METHOD].r) +
        (g_Color.g * grayScaleMethods[GRAYSCALE_METHOD].g) +
        (g_Color.b * grayScaleMethods[GRAYSCALE_METHOD].b)
    );
}
#endif
