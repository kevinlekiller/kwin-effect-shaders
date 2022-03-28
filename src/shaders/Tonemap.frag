#if TONEMAP_ENABLED == 1
/**
 * Tonemap version 1.1
 * by Christian Cann Schuldt Jensen ~ CeeJay.dk
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
void shader_tonemap() {
    // Defog
    g_Color.rgb = clamp(g_Color.rgb - TONAMEP_DEFOG * TONEMAP_FOGCOLOR * 2.55, 0.0, 1.0);
    // Exposure
    g_Color.rgb *= pow(2.0, TONEMAP_EXPOSURE);
    // Gamma
    g_Color.r = pow(g_Color.r, TONEMAP_GAMMA);
    g_Color.g = pow(g_Color.g, TONEMAP_GAMMA);
    g_Color.b = pow(g_Color.b, TONEMAP_GAMMA);

    float tmLum = dot(vec3(0.2126, 0.7152, 0.0722), g_Color.rgb);
    vec3 tmA2 = TONEMAP_BLEACH * g_Color.rgb;

    g_Color.rgb += (
        (1.0 - tmA2) * (tmA2 * mix(
            2.0 * g_Color.rgb * tmLum, 1.0 - 2.0 *
            (1.0 - tmLum) *
            (1.0 - g_Color.rgb), clamp(10.0 * (tmLum - 0.45), 0.0, 1.0)
        ))
    );

    float tmGrayDiv = 1.0 / 3.0;
    vec3 tmDiffColor = g_Color.rgb - vec3(dot(g_Color.r, tmGrayDiv), dot(g_Color.g, tmGrayDiv), dot(g_Color.b, tmGrayDiv));
    g_Color.rgb = (g_Color.rgb + tmDiffColor * TONEMAP_SATURATION) / (1 + (tmDiffColor * TONEMAP_SATURATION)); // Saturation
}
#endif // TONEMAP_ENABLED
