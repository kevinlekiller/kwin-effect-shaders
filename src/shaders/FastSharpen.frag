#if FAST_SHARPEN_ENABLED == 1
/*
   Fast Sharpen Shader

   Copyright (C) 2005 - 2019 guest(r) - guest.r@gmail.com
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
void shader_fast_sharpen() {
    vec2 g01 = vec2(-1.0, -0.3333) * g_SourceSize.zw;
    vec2 g10 = vec2(0.3333, -1.0) * g_SourceSize.zw;
    vec2 g12 = vec2(-0.3333, 1.0) * g_SourceSize.zw;
    vec2 g21 = vec2(1.0, 0.3333) * g_SourceSize.zw;
    vec3 c10 = texture(g_Texture, g_oTexcoord + g10).rgb;
    vec3 c01 = texture(g_Texture, g_oTexcoord + g01).rgb;
    vec3 c21 = texture(g_Texture, g_oTexcoord + g21).rgb;
    vec3 c12 = texture(g_Texture, g_oTexcoord + g12).rgb;
    vec3 c11 = clamp(g_Color.rgb, 0.0, 1.0);
    vec3 b11 = (c10 + c01 + c12 + c21) * 0.25;
    float contrast = max(max(c11.r, c11.g), c11.b);
    contrast = mix(2.0 * FS_CONTR, FS_CONTR, contrast);
    vec3 mn1 = min(min(c10, c01), min(c12, c21));
    mn1 = min(mn1, c11 * (1.0 - contrast));
    vec3 mx1 = max(max(c10, c01), max(c12, c21));
    mx1 = max(mx1, c11 * (1.0 + contrast));
    g_Color.rgb = clamp(
        mix(
            c11, b11,
            -mix(
                vec3(FS_SHARPEN * FS_DETAILS),
                vec3(FS_SHARPEN),
                pow(mx1-mn1+0.0001, vec3(0.75, 0.75, 0.75))
            )
        ), mn1, mx1
    );
}
#endif // FAST_SHARPEN_ENABLED
