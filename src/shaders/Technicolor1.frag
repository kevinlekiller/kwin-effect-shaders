#if TECHNICOLOR1_ENABLED == 1
/**
 * Technicolor version 1.1
 * Original by Prod80
 * Optimized by CeeJay.dk
 *
 * Ported to glsl by kevinlekiller 2022
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
void shader_technicolor1() {
    float output_r = dot(vec3(1.05, 0.620, 0.0),  g_Color.rgb * (1.0 / (TC1_RGB_NEGATIVE_AMOUNT.r * TC1_POWER)));
    float output_g = dot(vec3(0.30, 1.0,   0.0),  g_Color.rgb * (1.0 / (TC1_RGB_NEGATIVE_AMOUNT.g * TC1_POWER)));
    float output_b = dot(vec3(1.0,  0.0,   1.05), g_Color.rgb * (1.0 / (TC1_RGB_NEGATIVE_AMOUNT.b * TC1_POWER)));

    g_Color.rgb = mix(
        g_Color.rgb,
        vec3(output_r + 0.0, output_r + 1.3, output_r + 1.00) *
        vec3(output_g + 1.0, output_g + 0.0, output_g + 1.05) *
        vec3(output_b + 1.6, output_b + 1.6, output_b + 0.05),
        TC1_STRENGTH
    );
}
#endif // TECHNICOLOR1_ENABLED
