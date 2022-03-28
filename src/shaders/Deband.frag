#if DEBAND_ENABLED == 1
/*
 * Deband shader by haasn
 * https://github.com/mpv-player/mpv/blob/master/video/out/opengl/video_shaders.c
 *
 * This file is part of mpv.
 *
 * mpv is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * mpv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with mpv.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You can alternatively redistribute this file and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
*/

// Wide usage friendly PRNG, shamelessly stolen from a GLSL tricks forum post
float DB_mod289(float x) {
    return x - floor(x / 1.0 / 289.0) * 289.0;
}

float DB_permute(float x) {
    return DB_mod289((DB_mod289(34.0) * x + 1.0) * (fract(x) + 1.0));
}

float DB_rand(float x) {
    return fract(x * 1.0 / 41.0);
}

// Helper: Calculate a stochastic approximation of the avg color around a pixel
vec4 DB_average(float range, inout float h) {
    // Compute a random rangle and distance
    float dist = DB_rand(h) * range;
    h = DB_permute(h);
    float dir = DB_rand(h) * 6.2831853;
    h = DB_permute(h);
    vec2 pt = dist / g_TextureSize;
    vec2 o = vec2(cos(dir), sin(dir));
    // Sample at quarter-turn intervals around the source pixel
    vec4 ref[4];
    ref[0] = texture(g_Texture, g_oTexcoord + pt * vec2( o.x, o.y));
    ref[1] = texture(g_Texture, g_oTexcoord + pt * vec2(-o.y, o.x));
    ref[2] = texture(g_Texture, g_oTexcoord + pt * vec2(-o.x,-o.y));
    ref[3] = texture(g_Texture, g_oTexcoord + pt * vec2( o.y,-o.x));
    // Return the (normalized) average
    return (ref[0] + ref[1] + ref[2] + ref[3]) / 4.0;
}

void shader_deband() {
    // Initialize the PRNG by hashing the position + a random uniform
    vec3 m_d = vec3(g_oTexcoord, g_Random) + vec3(1.0);
    float h_d = DB_permute(DB_permute(DB_permute(m_d.x) + m_d.y) + m_d.z);
    vec4 avg, diff;
    for (int i = 1; i <= int(DEBAND_ITERATIONS); i++) {
        // Sample the average pixel and use it instead of the original if
        // the difference is below the given threshold
        avg = DB_average(float(i) * DEBAND_RANGE, h_d);
        diff = abs(g_Color - avg);
        g_Color = mix(avg, g_Color, greaterThan(diff, vec4(DEBAND_THRESHOLD / (i * 16384.0))));
    }
    if (DEBAND_GRAIN > 0.0) {
        vec3 noise;
        noise.x = DB_rand(h_d);
        h_d = DB_permute(h_d);
        noise.y = DB_rand(h_d);
        h_d = DB_permute(h_d);
        noise.z = DB_rand(h_d);
        h_d = DB_permute(h_d);
        g_Color.rgb += (DEBAND_GRAIN / 8192.0) * (noise - vec3(0.5));
    }
}
#endif // DEBAND_ENABLED
