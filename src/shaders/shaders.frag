#version 140
/**
 *  Please see the original licenses / copyright information for the included shaders by scrolling down this file.
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

//----------------------------------------------------------------
//----------------------------------------------------------------
//---------------------- Start of user configuration -------------
//----------------------------------------------------------------
//----------------------------------------------------------------

//----------------------------------------------------------------
//----- AMD Contrast adaptive sharpen configuration section ------
//----------------------------------------------------------------
// https://github.com/DadSchoorse/vkBasalt/blob/master/src/shader/cas.frag.glsl

// Set to 1 to enable.
#define CAS_ENABLED 0
#if CAS_ENABLED == 1 // Don't change this line.

// Default: 0.4
#define CAS_SHARPNESS 0.4

#endif
//----------------------------------------------------------------
//------------ Deband configuration section ----------------------
//----------------------------------------------------------------
// https://github.com/haasn/gentoo-conf/blob/xor/home/nand/.mpv/shaders/deband.glsl

// Reduces color banding - see https://en.wikipedia.org/wiki/Colour_banding

// Set to 1 to enable.
#define DEBAND_ENABLED 0
#if DEBAND_ENABLED == 1 // Don't change this line.

// The threshold of difference below which a pixel is considered to be part of
// a gradient. Higher = more debanding, but setting it too high diminishes image
// details.
// haasn default : 64
//   mpv default : 32
#define DEBAND_THRESHOLD 64

// The range (in source pixels) at which to sample for neighbours. Higher values
// will find more gradients, but lower values will deband more aggressively.
// haasn default : 8
//   mpv default : 16
#define DEBAND_RANGE 16

// The number of debanding iterations to perform. Each iteration samples from
// random positions, so increasing the number of iterations is likely to
// increase the debanding quality. Conversely, it slows the shader down.
// (Each iteration will use a multiple of the configured RANGE, and a
// successively lower THRESHOLD - so setting it much higher has little effect)
// haasn default : 4
//   mpv default : 1
#define DEBAND_ITERATIONS 1

// (Optional) Add some extra noise to the image. This significantly helps cover
// up remaining banding and blocking artifacts, at comparatively little visual
// quality. Higher = more grain. Setting it to 0 disables the effect.
// haasn default : 48
//   mpv default : 48
#define DEBAND_GRAIN 48

#endif
//----------------------------------------------------------------
//------------ Fake HDR configuration section --------------------
//----------------------------------------------------------------
// https://github.com/CeeJayDK/SweetFX/blob/master/Shaders/FakeHDR.fx

// Not actual HDR - It just tries to mimic an HDR look (relatively high performance cost)

// Set to 1 to enable.
#define FAKEHDR_ENABLED 0
#if FAKEHDR_ENABLED == 1 // Don't change this line.

// Power
// Default: 1.30
#define FHDR_POWER   1.3

// Radius 1
// Default: 0.793
#define FHDR_RADIUS1 0.793

// Radius 2
// Raising this seems to make the effect stronger and also brighter.
// Default: 0.87
#define FHDR_RADIUS2 0.87

#endif
//----------------------------------------------------------------
//------------ Fast Sharpen configuration section ----------------
//----------------------------------------------------------------
// https://github.com/libretro/glsl-shaders/blob/master/sharpen/shaders/fast-sharpen.glsl

// Doesn't produce as nice results as adaptive-sharpen but much less intensive to process.

// Set to 1 to enable.
#define FAST_SHARPEN_ENABLED 0
#if FAST_SHARPEN_ENABLED == 1 // Don't change this line.

// Sharpen strength
// Default: 1.2
#define FS_SHARPEN   1.2

// Amount of sharpening
// Default: 0.08
// Reducing this to around 0.01 helps not making everything look white.
#define FS_CONTR     0.08

// Details sharpened
// Default: 1.0
#define FS_DETAILS   1.0

#endif
//----------------------------------------------------------------
//------------ Levels configuration section ----------------------
//----------------------------------------------------------------
// https://github.com/CeeJayDK/SweetFX/blob/master/Shaders/Levels.fx

// Allows you to set a new black and a white level.
// This increases contrast, but clips any colors outside the new range to either black or white
// and so some details in the shadows or highlights can be lost.
// The shader is very useful for expanding the 16-235 TV range to 0-255 PC range.
// You might need it if you're playing a game meant to display on a TV with an emulator that does not do this.
// But it's also a quick and easy way to uniformly increase the contrast of an image.

// Set to 1 to enable.
#define LEVELS_ENABLED 0
#if LEVELS_ENABLED == 1 // Don't change this line.

// The black point is the new black - literally. Everything darker than this will become completely black.
// Default: 16
// 0 to 255
#define LVLS_BLACK_POINT 16

// The new white point. Everything brighter than this becomes completely white.
// Default: 235
// 0 to 255
#define LVLS_WHITE_POINT 235

#endif
//----------------------------------------------------------------
//-------- Nvidia DLS (sharpening) configuration section ---------
//----------------------------------------------------------------
// https://github.com/DadSchoorse/vkBasalt/blob/master/src/shader/dls.frag.glsl

// Set to 1 to enable.
#define NVIDIA_DLS_ENABLED 0
#if NVIDIA_DLS_ENABLED == 1 // Don't change this line.

// Default: 0.5
#define DLS_SHARPEN 0.5

// Default: 0.17
#define DLS_DENOISE 0.17

#endif
//----------------------------------------------------------------
//---------------------- Gaussian Blur H -------------------------
//----------------------------------------------------------------
// https://github.com/libretro/glsl-shaders/blob/master/blurs/blur-gauss-h.glsl

#define GAUSSBLURH_ENABLED 0
#if GAUSSBLURH_ENABLED == 1 // Don't change this line.

// Default: 1.0
#define GAUSSBLURH_STRENGTH 1.0

#endif
//----------------------------------------------------------------
//------------ Techicolor 2 configuration section ----------------
//----------------------------------------------------------------
// https://github.com/CeeJayDK/SweetFX/blob/master/Shaders/Technicolor2.fx

// Set to 1 to enable.
#define TECHNICOLOR2_ENABLED 0
#if TECHNICOLOR2_ENABLED == 1 // Don't change this line.

// Higher means darker and more intense colors.
// Default: 0.2 0.2 0.2
vec3 TC2_COLORSTRENGTH = vec3(0.2, 0.2, 0.2);

// Higher means brighter image.
// Default: 1.0
// 0.5 to 1.5
#define TC2_BRIGHTNESS 1.0

// Additional saturation control since this effect tends to oversaturate the image.
// Default: 1.0
// 0.0 to 1.5
#define TC2_SATURATION 1.0

// Adjust the strength of the effect.
// Default: 1.0
// 0.0 to 1.0
#define TC2_STRENGTH 1.0

#endif
//----------------------------------------------------------------
//------------ Vibrance configuration section --------------------
//----------------------------------------------------------------
// https://github.com/CeeJayDK/SweetFX/blob/master/Shaders/Vibrance.fx

// Vibrance intelligently boosts the saturation of pixels so pixels that had little color get a larger boost than pixels that had a lot.
// This avoids oversaturation of pixels that were already very saturated.

// Set to 1 to enable.
#define VIBRANCE_ENABLED 0
#if VIBRANCE_ENABLED == 1 // Don't change this line.

// Intelligently saturates (or desaturates if you use negative values) the pixels depending on their original saturation.
// -1.0 to 1.0
// Default: 0.15
#define VIB_VIBRANCE 0.15

// A per channel multiplier to the Vibrance strength so you can give more boost to certain colors over others.
// This is handy if you are colorblind and less sensitive to a specific color.
// You can then boost that color more than the others.
// 0.0 to 10.0
// Default: 1.0, 1.0, 1.0
vec3 VIB_RGB_BALANCE = vec3(1.0, 1.0, 1.0);

// Luma type
// 0 -> Perceptual
// 1 -> Even
// Default: 0
#define VIB_LUMA 0

#endif
//----------------------------------------------------------------
//------------ Adaptive Sharpen configuration section ------------
//----------------------------------------------------------------
// NOTE: This shader can be slow, consider using CAS, DLS or Fast Sharpen if you experience framedrops.
// https://gist.github.com/igv/8a77e4eb8276753b54bb94c1c50c317e

// Set to 1 to enable.
#define ADAPTIVE_SHARPEN_ENABLED 0
#if ADAPTIVE_SHARPEN_ENABLED == 1 // Don't change this line.

// Main control of sharpening strength [>0]
// 0.3 <-> 2.0 is a reasonable range of values
// Optimal sharpening strength (according to objective metrics) - 0.5.
// Default: 0.5
#define AS_CURVE_HEIGHT    0.5

#endif
//----------------------------------------------------------------
//----------------------------------------------------------------
//------------------------ End of user configuration -------------
//----------------------------------------------------------------
//----------------------------------------------------------------

uniform sampler2D g_Texture; // AKA Texture or Source in libretro
in vec2 g_oTexcoord;         // AKA TEX0.xy or vTexCoord in libretro
out vec4 g_FragColor;
vec4 g_Color;
vec4 g_SourceSize;
vec2 g_TextureSize;

#if ADAPTIVE_SHARPEN_ENABLED == 1
// Copyright (c) 2015-2021, bacondither
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer
//    in this position and unchanged.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Adaptive sharpen - version 2021-10-17
// Tuned for use post-resize, EXPECTS FULL RANGE GAMMA LIGHT (requires ps >= 3.0)
// Defined values under this row are "optimal" DO NOT CHANGE IF YOU DO NOT KNOW WHAT YOU ARE DOING!

#define AS_curveslope      0.5                  // Sharpening curve slope, high edge values
#define AS_L_overshoot     0.003                // Max light overshoot before compression [>0.001]
#define AS_L_compr_low     0.167                // Light compression, default (0.167=~6x)
#define AS_L_compr_high    0.334                // Light compression, surrounded by edges (0.334=~3x)
#define AS_D_overshoot     0.003                // Max dark overshoot before compression [>0.001]
#define AS_D_compr_low     0.250                // Dark compression, default (0.250=4x)
#define AS_D_compr_high    0.500                // Dark compression, surrounded by edges (0.500=2x)
#define AS_scale_lim       0.1                  // Abs max change before compression [>0.01]
#define AS_scale_cs        0.056                // Compression slope above scale_lim
#define AS_pm_p            1.0                  // Power mean p-value [>0-1.0]
#define AS_max4(a,b,c,d)  ( max(max(a, b), max(c, d)) )
// Soft if, fast linear approx
#define AF_soft_if(a,b,c) ( AF_sat((a + b + c + 0.056/2.5)/(maxedge + 0.03/2.5) - 0.85) )
// Soft limit, modified tanh approx
#define AF_soft_lim(v,s)  ( AF_sat(abs(v/s)*(27.0 + pow(v/s, 2.0))/(27.0 + 9.0*pow(v/s, 2.0)))*s )
// Weighted power mean
#define AF_wpmean(a,b,w)  ( pow(w*pow(abs(a), AS_pm_p) + abs(1.0-w)*pow(abs(b), AS_pm_p), (1.0/AS_pm_p)) )
// Get destination pixel values
#define AF_sat(x)         ( clamp(x, 0.0, 1.0) )
#define AF_dxdy(val)      ( length(fwidth(val)) ) // =~1/2.5 hq edge without c_comp
#define AF_CtL(RGB)       ( sqrt(dot(RGB*RGB, vec3(0.2126, 0.7152, 0.0722))) )

void shader_adaptive_sharpen() {
    float px = g_SourceSize.z;
    float py = g_SourceSize.w;
    // Get points and saturate out of range values (BTB & WTW)
    // [                c22               ]
    // [           c24, c9,  c23          ]
    // [      c21, c1,  c2,  c3, c18      ]
    // [ c19, c10, c4,  c0,  c5, c11, c16 ]
    // [      c20, c6,  c7,  c8, c17      ]
    // [           c15, c12, c14          ]
    // [                c13               ]
    vec3 c19 = clamp(texture(g_Texture, g_oTexcoord + vec2(-3.*px,    0.)).rgb, 0.0, 1.0);
    vec3 c21 = clamp(texture(g_Texture, g_oTexcoord + vec2(-2.*px,   -py)).rgb, 0.0, 1.0);
    vec3 c10 = clamp(texture(g_Texture, g_oTexcoord + vec2(-2.*px,    0.)).rgb, 0.0, 1.0);
    vec3 c20 = clamp(texture(g_Texture, g_oTexcoord + vec2(-2.*px,    py)).rgb, 0.0, 1.0);
    vec3 c24 = clamp(texture(g_Texture, g_oTexcoord + vec2(  -px, -2.*py)).rgb, 0.0, 1.0);
    vec3 c1  = clamp(texture(g_Texture, g_oTexcoord + vec2(  -px,    -py)).rgb, 0.0, 1.0);
    vec3 c4  = clamp(texture(g_Texture, g_oTexcoord + vec2(  -px,     0.)).rgb, 0.0, 1.0);
    vec3 c6  = clamp(texture(g_Texture, g_oTexcoord + vec2(  -px,     py)).rgb, 0.0, 1.0);
    vec3 c15 = clamp(texture(g_Texture, g_oTexcoord + vec2(  -px,  2.*py)).rgb, 0.0, 1.0);
    vec3 c22 = clamp(texture(g_Texture, g_oTexcoord + vec2(   0., -3.*py)).rgb, 0.0, 1.0);
    vec3 c9  = clamp(texture(g_Texture, g_oTexcoord + vec2(   0., -2.*py)).rgb, 0.0, 1.0);
    vec3 c2  = clamp(texture(g_Texture, g_oTexcoord + vec2(   0.,    -py)).rgb, 0.0, 1.0);
    vec3 c0  = clamp(g_Color.rgb,   0.0,                     1.0);
    vec3 c7  = clamp(texture(g_Texture, g_oTexcoord + vec2(   0.,     py)).rgb, 0.0, 1.0);
    vec3 c12 = clamp(texture(g_Texture, g_oTexcoord + vec2(   0.,  2.*py)).rgb, 0.0, 1.0);
    vec3 c13 = clamp(texture(g_Texture, g_oTexcoord + vec2(   0.,  3.*py)).rgb, 0.0, 1.0);
    vec3 c23 = clamp(texture(g_Texture, g_oTexcoord + vec2(   px, -2.*py)).rgb, 0.0, 1.0);
    vec3 c3  = clamp(texture(g_Texture, g_oTexcoord + vec2(   px,    -py)).rgb, 0.0, 1.0);
    vec3 c5  = clamp(texture(g_Texture, g_oTexcoord + vec2(   px,     0.)).rgb, 0.0, 1.0);
    vec3 c8  = clamp(texture(g_Texture, g_oTexcoord + vec2(   px,     py)).rgb, 0.0, 1.0);
    vec3 c14 = clamp(texture(g_Texture, g_oTexcoord + vec2(   px,  2.*py)).rgb, 0.0, 1.0);
    vec3 c18 = clamp(texture(g_Texture, g_oTexcoord + vec2( 2.*px,   -py)).rgb, 0.0, 1.0);
    vec3 c11 = clamp(texture(g_Texture, g_oTexcoord + vec2( 2.*px,    0.)).rgb, 0.0, 1.0);
    vec3 c17 = clamp(texture(g_Texture, g_oTexcoord + vec2( 2.*px,    py)).rgb, 0.0, 1.0);
    vec3 c16 = clamp(texture(g_Texture, g_oTexcoord + vec2( 3.*px,    0.)).rgb, 0.0, 1.0 );
    float e[13] = float[] (
        AF_dxdy(c0),  AF_dxdy(c1),  AF_dxdy(c2),  AF_dxdy(c3),  AF_dxdy(c4),
        AF_dxdy(c5),  AF_dxdy(c6),  AF_dxdy(c7),  AF_dxdy(c8),  AF_dxdy(c9),
        AF_dxdy(c10), AF_dxdy(c11), AF_dxdy(c12)
    );
    // Blur, gauss 3x3
    vec3 blur = (2.0 * (c2 + c4 + c5 + c7) + (c1 + c3 + c6 +c8) + 4.0 * c0) / 16.0;
    // Contrast compression, center = 0.5, scaled to 1/3
    float c_comp = AF_sat(0.266666681f + 0.9 * exp2(dot(blur, vec3(-7.4 / 3.0))));
    // Edge detection
    // Matrix, relative weights
    // [           1          ]
    // [       4,  4,  4      ]
    // [   1,  4,  4,  4,  1  ]
    // [       4,  4,  4      ]
    // [           1          ]
    float edge = (
        length(
            1.38 * abs(blur-c0) +
            1.15 * (abs(blur - c2) + abs(blur - c4) + abs(blur - c5) + abs(blur - c7)) +
            0.92 * (abs(blur - c1) + abs(blur - c3) + abs(blur - c6) + abs(blur - c8)) +
            0.23 * (abs(blur - c9) + abs(blur - c10) + abs(blur - c11) + abs(blur - c12))
        ) * c_comp
    );
    vec2 cs = vec2(AS_L_compr_low,  AS_D_compr_low);
    float luma[25] = float[] (
        AF_CtL(c0),  AF_CtL(c1),  AF_CtL(c2),  AF_CtL(c3),  AF_CtL(c4),  AF_CtL(c5),  AF_CtL(c6),  AF_CtL(c7), AF_CtL(c8),
        AF_CtL(c9),  AF_CtL(c10), AF_CtL(c11), AF_CtL(c12), AF_CtL(c13), AF_CtL(c14), AF_CtL(c15), AF_CtL(c16),
        AF_CtL(c17), AF_CtL(c18), AF_CtL(c19), AF_CtL(c20), AF_CtL(c21), AF_CtL(c22), AF_CtL(c23), AF_CtL(c24)
    );
    float c0_Y = luma[0];
    // Precalculated default squared kernel weights
    const vec3 w1 = vec3(0.5,           1.0, 1.41421356237); // 0.25, 1.0, 2.0
    const vec3 w2 = vec3(0.86602540378, 1.0, 0.54772255751); // 0.75, 1.0, 0.3
    // Transition to a concave kernel if the center edge val is above thr
    vec3 dW = pow(mix( w1, w2, AF_sat(2.4 * edge - 0.82)), vec3(2.0));
    // Use lower weights for pixels in a more active area relative to center pixel area
    // This results in narrower and less visible overshoots around sharp edges
    float modif_e0 = 3.0 * e[0] + 0.02 / 2.5;
    float weights[12]  = float[](
        ( min(modif_e0/e[1],  dW.y) ),
        ( dW.x ),
        ( min(modif_e0/e[3],  dW.y) ),
        ( dW.x ),
        ( dW.x ),
        ( min(modif_e0/e[6],  dW.y) ),
        ( dW.x ),
        ( min(modif_e0/e[8],  dW.y) ),
        ( min(modif_e0/e[9],  dW.z) ),
        ( min(modif_e0/e[10], dW.z) ),
        ( min(modif_e0/e[11], dW.z) ),
        ( min(modif_e0/e[12], dW.z) )
    );
    weights[0] = (max(max((weights[8]  + weights[9])  / 4.0, weights[0]), 0.25) + weights[0]) / 2.0;
    weights[2] = (max(max((weights[8]  + weights[10]) / 4.0, weights[2]), 0.25) + weights[2]) / 2.0;
    weights[5] = (max(max((weights[9]  + weights[11]) / 4.0, weights[5]), 0.25) + weights[5]) / 2.0;
    weights[7] = (max(max((weights[10] + weights[11]) / 4.0, weights[7]), 0.25) + weights[7]) / 2.0;
    // Calculate the negative part of the laplace kernel and the low threshold weight
    float lowthrsum   = 0.0;
    float weightsum   = 0.0;
    float neg_laplace = 0.0;
    for (int pix = 0; pix < 12; ++pix) {
        float lowthr = clamp((20.0 * 4.5*c_comp*e[pix + 1] - 0.221), 0.01, 1.0);
        neg_laplace += luma[pix+1] * weights[pix] * lowthr;
        weightsum   += weights[pix] * lowthr;
        lowthrsum   += lowthr / 12.0;
    }
    neg_laplace = neg_laplace / weightsum;
    // Compute sharpening magnitude function
    float sharpen_val = AS_CURVE_HEIGHT / (AS_CURVE_HEIGHT * AS_curveslope * pow(edge, 3.5) + 0.625);
    // Calculate sharpening diff and scale
    float sharpdiff = (c0_Y - neg_laplace) * (lowthrsum*sharpen_val + 0.01);
    // Calculate local near min & max, partial sort
    float temp;
    for (int i1 = 0; i1 < 24; i1 += 2) {
        temp = luma[i1];
        luma[i1]   = min(luma[i1], luma[i1 + 1]);
        luma[i1+1] = max(temp, luma[i1 + 1]);
    }
    for (int i2 = 24; i2 > 0; i2 -= 2) {
        temp = luma[0];
        luma[0] = min(luma[0], luma[i2]);
        luma[i2] = max(temp, luma[i2]);
        temp = luma[24];
        luma[24] = max(luma[24], luma[i2 - 1]);
        luma[i2 - 1] = min(temp, luma[i2 - 1]);
    }
    for (int i1 = 1; i1 < 24 - 1; i1 += 2) {
        temp = luma[i1];
        luma[i1] = min(luma[i1], luma[i1 + 1]);
        luma[i1 + 1] = max(temp, luma[i1 + 1]);
    }
    for (int i2 = 24 - 1; i2 > 1; i2 -= 2) {
        temp = luma[1];
        luma[1] = min(luma[1], luma[i2]);
        luma[i2] = max(temp, luma[i2]);
        temp = luma[24 - 1];
        luma[24 - 1] = max(luma[24 - 1], luma[i2 - 1]);
        luma[i2 - 1] = min(temp, luma[i2 - 1]);
    }
    float nmax = (max(luma[23], c0_Y) * 3.0 + luma[24]) / 4.0;
    float nmin = (min(luma[1],  c0_Y) * 3.0 + luma[0]) / 4.0;
    float min_dist  = min(abs(nmax - c0_Y), abs(c0_Y - nmin));
    vec2 pn_scale = vec2(AS_L_overshoot, AS_D_overshoot) + min_dist;
    pn_scale = min(pn_scale, AS_scale_lim * (1.0 - AS_scale_cs) + pn_scale * AS_scale_cs);
    // Soft limited anti-ringing with tanh, wpmean to control compression slope
    sharpdiff = AF_wpmean(max(sharpdiff, 0.0), AF_soft_lim(max(sharpdiff, 0.0), pn_scale.x), cs.x ) -
        AF_wpmean(min(sharpdiff, 0.0), AF_soft_lim( min(sharpdiff, 0.0), pn_scale.y ), cs.y)
    ;
    float sharpdiff_lim = AF_sat(c0_Y + sharpdiff) - c0_Y;
    float satmul = (c0_Y + max(sharpdiff_lim * 0.9, sharpdiff_lim) * 1.03 + 0.03) / (c0_Y + 0.03);
    g_Color.rgb = c0_Y + (sharpdiff_lim *  3.0 + sharpdiff) / 4.0 + (c0 - c0_Y) * satmul;
}
#endif // ADAPTIVE_SHARPEN_ENABLED

#if CAS_ENABLED == 1
// LICENSE
// =======
// Copyright (c) 2017-2019 Advanced Micro Devices, Inc. All rights reserved.
// -------
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// -------
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
// -------
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

#ifndef textureLod0Offset
#define textureLod0Offset(img, coord, offset) textureLodOffset(img, coord, 0.0f, offset)
#endif

void shader_amd_cas() {
    // fetch a 3x3 neighborhood around the pixel 'e',
    //  a b c
    //  d(e)f
    //  g h i

    vec3 a = textureLod0Offset(g_Texture, g_oTexcoord, ivec2(-1,-1)).rgb;
    vec3 b = textureLod0Offset(g_Texture, g_oTexcoord, ivec2( 0,-1)).rgb;
    vec3 c = textureLod0Offset(g_Texture, g_oTexcoord, ivec2( 1,-1)).rgb;
    vec3 d = textureLod0Offset(g_Texture, g_oTexcoord, ivec2(-1, 0)).rgb;
    vec3 e = g_Color.rgb;
    vec3 f = textureLod0Offset(g_Texture, g_oTexcoord, ivec2( 1, 0)).rgb;
    vec3 g = textureLod0Offset(g_Texture, g_oTexcoord, ivec2(-1, 1)).rgb;
    vec3 h = textureLod0Offset(g_Texture, g_oTexcoord, ivec2( 0, 1)).rgb;
    vec3 i = textureLod0Offset(g_Texture, g_oTexcoord, ivec2( 1, 1)).rgb;

    // Soft min and max.
    //  a b c             b
    //  d e f * 0.5  +  d e f * 0.5
    //  g h i             h
    // These are 2.0x bigger (factored out the extra multiply).

    vec3 mnRGB  = min(min(min(d,e),min(f,b)),h);
    vec3 mnRGB2 = min(min(min(mnRGB,a),min(g,c)),i);
    mnRGB += mnRGB2;

    vec3 mxRGB  = max(max(max(d,e),max(f,b)),h);
    vec3 mxRGB2 = max(max(max(mxRGB,a),max(g,c)),i);
    mxRGB += mxRGB2;

    // Smooth minimum distance to signal limit divided by smooth max.

    vec3 rcpMxRGB = vec3(1)/mxRGB;
    vec3 ampRGB = clamp((min(mnRGB,2.0-mxRGB) * rcpMxRGB),0,1);

    // Shaping amount of sharpening.
    ampRGB = inversesqrt(ampRGB);
    float peak = 8.0 - 3.0 * CAS_SHARPNESS;
    vec3 wRGB = -vec3(1)/(ampRGB * peak);
    vec3 rcpWeightRGB = vec3(1)/(1.0 + 4.0 * wRGB);

    //                          0 w 0
    //  Filter shape:           w 1 w
    //                          0 w 0

    vec3 window = (b + d) + (f + h);
    g_Color.rgb = clamp((window * wRGB + e) * rcpWeightRGB,0,1);
}
#endif // CAS_ENABLED

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
    vec3 m_d = vec3(
        g_oTexcoord,
        // https://stackoverflow.com/a/28095165
        fract(tan(distance(g_TextureSize * 1.61803398874989484820459, g_TextureSize) * g_Color.r) * g_TextureSize.x)
    ) + vec3(1.0);
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

#if NVIDIA_DLS_ENABLED == 1
/*
  Image sharpening filter from GeForce Experience. Provided by NVIDIA Corporation.

  Copyright 2019 Suketu J. Shah. All rights reserved.
  Redistribution and use in source and binary forms, with or without modification, are permitted provided
  that the following conditions are met:
    1. Redistributions of source code must retain the above copyright notice, this list of conditions
       and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
       and the following disclaimer in the documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse
       or promote products derived from this software without specific prior written permission.
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef textureLod0Offset
#define textureLod0Offset(img, coord, offset) textureLodOffset(img, coord, 0.0f, offset)
#endif

float DLS_GetLumaComponents(float r, float g, float b)
{
    // Y from JPEG spec
    return 0.299 * r + 0.587 * g + 0.114 * b;
}

float DLS_GetLuma(vec4 p)
{
    return DLS_GetLumaComponents(p.x, p.y, p.z);
}

float DLS_Square(float v)
{
    return v * v;
}

// highlight fall-off start (prevents halos and noise in bright areas)
#define DLS_kHighBlock 0.65
// offset reducing sharpening in the shadows
#define DLS_kLowBlock (1.0 / 256.0)
#define DLS_kSharpnessMin (-1.0 / 14.0)
#define DLS_kSharpnessMax (-1.0 / 6.5)
#define DLS_kDenoiseMin (0.001)
#define DLS_kDenoiseMax (-0.1)

void shader_nvidia_dls()
{
    //  e  d  h
    //  a (x) b
    //  g  c  f

    vec4 a = textureLod0Offset(g_Texture, g_oTexcoord, ivec2(-1,  0));
    vec4 b = textureLod0Offset(g_Texture, g_oTexcoord, ivec2( 1,  0));
    vec4 c = textureLod0Offset(g_Texture, g_oTexcoord, ivec2( 0,  1));
    vec4 d = textureLod0Offset(g_Texture, g_oTexcoord, ivec2( 0, -1));

    vec4 e = textureLod0Offset(g_Texture, g_oTexcoord, ivec2(-1, -1));
    vec4 f = textureLod0Offset(g_Texture, g_oTexcoord, ivec2( 1,  1));
    vec4 g = textureLod0Offset(g_Texture, g_oTexcoord, ivec2(-1,  1));
    vec4 h = textureLod0Offset(g_Texture, g_oTexcoord, ivec2( 1, -1));

    float lx = DLS_GetLuma(g_Color);

    float la = DLS_GetLuma(a);
    float lb = DLS_GetLuma(b);
    float lc = DLS_GetLuma(c);
    float ld = DLS_GetLuma(d);

    float le = DLS_GetLuma(e);
    float lf = DLS_GetLuma(f);
    float lg = DLS_GetLuma(g);
    float lh = DLS_GetLuma(h);

    // cross min/max
    float ncmin = min(min(le, lf), min(lg, lh));
    float ncmax = max(max(le, lf), max(lg, lh));

    // plus min/max
    float npmin = min(min(min(la, lb), min(lc, ld)), lx);
    float npmax = max(max(max(la, lb), max(lc, ld)), lx);

    // compute "soft" local dynamic range -- average of 3x3 and plus shape
    float lmin = 0.5 * min(ncmin, npmin) + 0.5 * npmin;
    float lmax = 0.5 * max(ncmax, npmax) + 0.5 * npmax;

    // compute local contrast enhancement kernel
    float lw = lmin / (lmax + DLS_kLowBlock);
    float hw = DLS_Square(1.0 - DLS_Square(max(lmax - DLS_kHighBlock, 0.0) / ((1.0 - DLS_kHighBlock))));

    // noise suppression
    // Note: Ensure that the denoiseFactor is in the range of (10, 1000) on the CPU-side prior to launching this shader.
    // For example, you can do so by adding these lines
    //      const float kDenoiseMin = 0.001f;
    //      const float kDenoiseMax = 0.1f;
    //      float kernelDenoise = 1.0 / (kDenoiseMin + (kDenoiseMax - kDenoiseMin) * min(max(denoise, 0.0), 1.0));
    // where kernelDenoise is the value to be passed in to this shader (the amount of noise suppression is inversely proportional to this value),
    //       denoise is the value chosen by the user, in the range (0, 1)
    float kernelDenoise = 1.0 / (DLS_kDenoiseMin + (DLS_kDenoiseMax - DLS_kDenoiseMin) * DLS_DENOISE);
    float nw = DLS_Square((lmax - lmin) * kernelDenoise);

    // pick conservative boost
    float boost = min(min(lw, hw), nw);

    // run variable-sigma 3x3 sharpening convolution
    // Note: Ensure that the sharpenFactor is in the range of (-1.0/14.0, -1.0/6.5f) on the CPU-side prior to launching this shader.
    // For example, you can do so by adding these lines
    //      const float kSharpnessMin = -1.0 / 14.0;
    //      const float kSharpnessMax = -1.0 / 6.5f;
    //      float kernelSharpness = kSharpnessMin + (kSharpnessMax - kSharpnessMin) * min(max(sharpen, 0.0), 1.0);
    // where kernelSharpness is the value to be passed in to this shader,
    //       sharpen is the value chosen by the user, in the range (0, 1)
    float kernelSharpness = DLS_kSharpnessMin + (DLS_kSharpnessMax - DLS_kSharpnessMin) * DLS_SHARPEN;
    float k = boost * kernelSharpness;

    float accum = lx;
    accum += la * k;
    accum += lb * k;
    accum += lc * k;
    accum += ld * k;
    accum += le * (k * 0.5);
    accum += lf * (k * 0.5);
    accum += lg * (k * 0.5);
    accum += lh * (k * 0.5);

    // normalize (divide the accumulator by the sum of convolution weights)
    accum /= 1.0 + 6.0 * k;

    // accumulator is in linear light space
    float delta = accum - lx;
    g_Color.x += delta;
    g_Color.y += delta;
    g_Color.z += delta;
}
#endif // NVIDIA_DLS_ENABLED

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

#if GAUSSBLURH_ENABLED == 1
// Implementation based on the article "Efficient Gaussian blur with linear sampling"
// http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
/* A version for MasterEffect Reborn, a standalone version, and a custom shader version for SweetFX can be
   found at http://reshade.me/forum/shader-presentation/27-gaussian-blur-bloom-unsharpmask */

void shader_gauss_blur_h() {
    vec2 PIXEL_SIZE = g_SourceSize.zw;
    float sampleOffsets1 = 0.0;
    float sampleOffsets2 = 1.4347826;
    float sampleOffsets3 = 3.3478260;
    float sampleOffsets4 = 5.2608695;
    float sampleOffsets5 = 7.1739130;

    float sampleWeights1 = 0.16818994;
    float sampleWeights2 = 0.27276957;
    float sampleWeights3 = 0.11690125;
    float sampleWeights4 = 0.024067905;
    float sampleWeights5 = 0.0021112196;

    vec4 color = texture(g_Texture, g_oTexcoord) * sampleWeights1;
    color += texture(g_Texture, g_oTexcoord + vec2(sampleOffsets2* GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights2;
    color += texture(g_Texture, g_oTexcoord - vec2(sampleOffsets2* GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights2;
    color += texture(g_Texture, g_oTexcoord + vec2(sampleOffsets3* GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights3;
    color += texture(g_Texture, g_oTexcoord - vec2(sampleOffsets3* GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights3;
    color += texture(g_Texture, g_oTexcoord + vec2(sampleOffsets4* GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights4;
    color += texture(g_Texture, g_oTexcoord - vec2(sampleOffsets4* GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights4;
    color += texture(g_Texture, g_oTexcoord + vec2(sampleOffsets5* GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights5;
    color += texture(g_Texture, g_oTexcoord - vec2(sampleOffsets5* GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights5;

    g_Color = vec4(color);
}
#endif // GAUSSBLURH_ENABLED

#if LEVELS_ENABLED == 1
/**
 * Levels version 1.2
 * by Christian Cann Schuldt Jensen ~ CeeJay.dk
 *
 * Allows you to set a new black and a white level.
 * This increases contrast, but clips any colors outside the new range to either black or white
 * and so some details in the shadows or highlights can be lost.
 *
 * The shader is very useful for expanding the 16-235 TV range to 0-255 PC range.
 * You might need it if you're playing a game meant to display on a TV with an emulator that does not do this.
 * But it's also a quick and easy way to uniformly increase the contrast of an image.
 *
 * -- Version 1.0 --
 * First release
 * -- Version 1.1 --
 * Optimized to only use 1 instruction (down from 2 - a 100% performance increase :) )
 * -- Version 1.2 --
 * Added the ability to highlight clipping regions of the image with #define HighlightClipping 1
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

void shader_levels() {
    float black_point_float = LVLS_BLACK_POINT / 255.0;
    // Avoid division by zero if the white and black point are the same
    float white_point_float = LVLS_WHITE_POINT == LVLS_BLACK_POINT ? (255.0 / 0.00025) : (255.0 / (LVLS_WHITE_POINT - LVLS_BLACK_POINT));
    g_Color.rgb = g_Color.rgb * white_point_float - (black_point_float *  white_point_float);
}
#endif // LEVELS_ENABLED

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

void main() {
    g_Color = texture(g_Texture, g_oTexcoord).rgba;
    g_TextureSize = textureSize(g_Texture, 0);
    g_SourceSize = vec4(g_TextureSize, 1.0 / g_TextureSize);
#if DEBAND_ENABLED == 1
    shader_deband();
#endif // DEBAND_ENABLED

#if TECHNICOLOR2_ENABLED ==1
    shader_technicolor2();
#endif // TECHNICOLOR2_ENABLED

#if VIBRANCE_ENABLED == 1
    shader_vibrance();
#endif // VIBRANCE_ENABLED

#if FAKEHDR_ENABLED == 1
    shader_fake_hdr();
#endif // FAKEHDR_ENABLED

#if LEVELS_ENABLED == 1
    shader_levels();
#endif // LEVELS_ENABLED

#if GAUSSBLURH_ENABLED == 1
    shader_gauss_blur_h();
#endif // GAUSSBLURH_ENABLED

#if CAS_ENABLED == 1
    shader_amd_cas();
#endif // CAS_ENABLED

#if NVIDIA_DLS_ENABLED == 1
    shader_nvidia_dls();
#endif // NVIDIA_DLS_ENABLED

#if FAST_SHARPEN_ENABLED == 1
    shader_fast_sharpen();
#endif // FAST_SHARPEN_ENABLED

#if ADAPTIVE_SHARPEN_ENABLED == 1
    shader_adaptive_sharpen();
#endif // ADAPTIVE_SHARPEN_ENABLED

    g_FragColor = g_Color;
}
