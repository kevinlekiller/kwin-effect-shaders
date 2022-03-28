#if FAKEHDR_ENABLED == 1
/**
 * HDR
 * by Christian Cann Schuldt Jensen ~ CeeJay.dk
 *
 * Not actual HDR - It just tries to mimic an HDR look (relatively high performance cost)
 *
 * Ported to glsl by kevinlekiller - 2022
 */
 /*
    The MIT License (MIT)

    Copyright (c) 2014 CeeJayDK

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

float FHDR_DIST = FHDR_RADIUS2 - FHDR_RADIUS1;

void shader_fake_hdr() {
    vec3 bloom_sum1 = texture(g_Texture, g_oTexcoord + vec2(1.5, -1.5) * FHDR_RADIUS1 * g_TextureSize).rgb;
    bloom_sum1 += texture(g_Texture, g_oTexcoord + vec2(-1.5, -1.5) * FHDR_RADIUS1 * g_TextureSize).rgb;
    bloom_sum1 += texture(g_Texture, g_oTexcoord + vec2( 1.5,  1.5) * FHDR_RADIUS1 * g_TextureSize).rgb;
    bloom_sum1 += texture(g_Texture, g_oTexcoord + vec2(-1.5,  1.5) * FHDR_RADIUS1 * g_TextureSize).rgb;
    bloom_sum1 += texture(g_Texture, g_oTexcoord + vec2( 0.0, -2.5) * FHDR_RADIUS1 * g_TextureSize).rgb;
    bloom_sum1 += texture(g_Texture, g_oTexcoord + vec2( 0.0,  2.5) * FHDR_RADIUS1 * g_TextureSize).rgb;
    bloom_sum1 += texture(g_Texture, g_oTexcoord + vec2(-2.5,  0.0) * FHDR_RADIUS1 * g_TextureSize).rgb;
    bloom_sum1 += texture(g_Texture, g_oTexcoord + vec2( 2.5,  0.0) * FHDR_RADIUS1 * g_TextureSize).rgb;
    vec3 bloom_sum2 = texture(g_Texture, g_oTexcoord + vec2(1.5, -1.5) * FHDR_RADIUS2 * g_TextureSize).rgb;
    bloom_sum2 += texture(g_Texture, g_oTexcoord + vec2(-1.5, -1.5) * FHDR_RADIUS2 * g_TextureSize).rgb;
    bloom_sum2 += texture(g_Texture, g_oTexcoord + vec2( 1.5,  1.5) * FHDR_RADIUS2 * g_TextureSize).rgb;
    bloom_sum2 += texture(g_Texture, g_oTexcoord + vec2(-1.5,  1.5) * FHDR_RADIUS2 * g_TextureSize).rgb;
    bloom_sum2 += texture(g_Texture, g_oTexcoord + vec2( 0.0, -2.5) * FHDR_RADIUS2 * g_TextureSize).rgb;
    bloom_sum2 += texture(g_Texture, g_oTexcoord + vec2( 0.0,  2.5) * FHDR_RADIUS2 * g_TextureSize).rgb;
    bloom_sum2 += texture(g_Texture, g_oTexcoord + vec2(-2.5,  0.0) * FHDR_RADIUS2 * g_TextureSize).rgb;
    bloom_sum2 += texture(g_Texture, g_oTexcoord + vec2( 2.5,  0.0) * FHDR_RADIUS2 * g_TextureSize).rgb;
    vec3 HDR = (g_Color.rgb + (bloom_sum2 * 0.01 - bloom_sum1 * 0.005)) * FHDR_DIST;
    g_Color.r = pow(abs(HDR.r + g_Color.r), FHDR_POWER) + HDR.r;
    g_Color.g = pow(abs(HDR.g + g_Color.g), FHDR_POWER) + HDR.g;
    g_Color.b = pow(abs(HDR.b + g_Color.b), FHDR_POWER) + HDR.b;
}
#endif // FAKEHDR_ENABLED
