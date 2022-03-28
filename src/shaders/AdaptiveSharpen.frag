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
