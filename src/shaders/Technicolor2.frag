#if TECHNICOLOR2_ENABLED == 1
/**
 * Technicolor2 version 1.0
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
void shader_technicolor2() {
    vec3 temp = 1.0 - g_Color.rgb;
    vec3 target = temp.grg;
    vec3 target2 = temp.bbr;
    vec3 temp2 = g_Color.rgb * target;
    temp2 *= target2;
    temp = temp2 * TC2_COLORSTRENGTH;
    temp2 *= TC2_BRIGHTNESS;
    target = temp.grg;
    target2 = temp.bbr;
    temp = g_Color.rgb - target;
    temp += temp2;
    temp2 = temp - target2;
    g_Color.r = mix(g_Color.r, temp2.r, TC2_STRENGTH);
    g_Color.g = mix(g_Color.g, temp2.g, TC2_STRENGTH);
    g_Color.b = mix(g_Color.b, temp2.b, TC2_STRENGTH);
    g_Color.r = mix(dot(g_Color.r, 0.333), g_Color.r, TC2_SATURATION);
    g_Color.g = mix(dot(g_Color.g, 0.333), g_Color.g, TC2_SATURATION);
    g_Color.b = mix(dot(g_Color.b, 0.333), g_Color.b, TC2_SATURATION);
}
#endif // TECHNICOLOR2_ENABLED
