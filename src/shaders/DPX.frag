#if DPX_ENABLED == 1
/**
 * DPX/Cineon shader by Loadus
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
void shader_dpx() {
    mat3x3 dpxRGB = mat3x3(
         2.6714711726599600, -1.2672360578624100, -0.4109956021722270,
        -1.0251070293466400,  1.9840911624108900,  0.0439502493584124,
         0.0610009456429445, -0.2236707508128630,  1.1590210416706100
    );
    mat3x3 dpxXYZ = mat3x3(
        0.5003033835433160, 0.3380975732227390, 0.1645897795458570,
        0.2579688942747580, 0.6761952591447060, 0.0658358459823868,
        0.0234517888692628, 0.1126992737203000, 0.8668396731242010
    );
    vec3 dpxB = g_Color.rgb * (1.0 - DPX_CONTRAST) + (0.5 * DPX_CONTRAST);
    vec3 dpxB2 = (1.0 / (1.0 + exp(DPX_RGB_CURVE / 2.0)));
    dpxB = ((1.0 / (1.0 + exp(-DPX_RGB_CURVE * (dpxB - DPX_RGB_C)))) / (-2.0 * dpxB2 + 1.0)) + (-dpxB2 / (-2.0 * dpxB2 + 1.0));

    float dpxVal = max(max(dpxB.r, dpxB.g), dpxB.b);
    vec3 dpxC0 = dpxXYZ * (pow(abs(dpxB / dpxVal), 1.0 / DPX_COLORFULNESS) * dpxVal);
    g_Color.rgb = mix(g_Color.rgb, (dpxRGB * ((1.0 - DPX_SATURATION) * dot(dpxC0, vec3(0.30, 0.59, 0.11)) + DPX_SATURATION * dpxC0)), DPX_STRENGTH);
}
#endif // DPX_ENABLED
