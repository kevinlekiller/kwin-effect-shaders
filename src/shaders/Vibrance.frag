#if VIBRANCE_ENABLED == 1
/**
  Vibrance
  by Christian Cann Schuldt Jensen ~ CeeJay.dk

  Vibrance intelligently boosts the saturation of pixels so pixels that had little color get a larger boost than pixels that had a lot.
  This avoids oversaturation of pixels that were already very saturated.
  History:
  Version 1.0 by Ceejay.dk
  - Original
  Version 1.1 by CeeJay.dk
  - Introduced RBG balance to help colorblind users
  Version 1.1.1
  - Minor UI improvements for Reshade 3.x

  Ported to glsl by kevinlekiller - 2022
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

#if VIB_LUMA == 1
vec3 VIB_coefLuma = vec3(0.333333, 0.333334, 0.333333);
#else
vec3 VIB_coefLuma = vec3(0.212656, 0.715158, 0.072186);
#endif
vec3 VIB_coeffVibrance = vec3(VIB_RGB_BALANCE * -VIB_VIBRANCE);

void shader_vibrance() {
    float luma = dot(VIB_coefLuma, g_Color.rgb);
    float max_color = max(g_Color.r, max(g_Color.g, g_Color.b));
    float min_color = min(g_Color.r, min(g_Color.g, g_Color.b));
    float color_saturation = max_color - min_color;
    vec3 p_col = vec3(vec3(vec3(vec3(sign(VIB_coeffVibrance) * color_saturation) - 1.0) * VIB_coeffVibrance) + 1.0);
    g_Color.r = mix(luma, g_Color.r, p_col.r);
    g_Color.g = mix(luma, g_Color.g, p_col.g);
    g_Color.b = mix(luma, g_Color.b, p_col.b);
}
#endif // VIBRANCE_ENABLED
