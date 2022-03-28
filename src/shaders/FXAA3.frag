#if FXAA3_ENABLED == 1
/*============================================================================
                    NVIDIA FXAA 3.11 by TIMOTHY LOTTES
------------------------------------------------------------------------------
COPYRIGHT (C) 2010, 2011 NVIDIA CORPORATION. ALL RIGHTS RESERVED.
------------------------------------------------------------------------------
TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
*AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL NVIDIA
OR ITS SUPPLIERS BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR
CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR
LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION,
OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE
THIS SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
DAMAGES.
*/
#if (FXAA3_QUALITY__PRESET == 10)
    #define FXAA3_QUALITY__PS 3
    #define FXAA3_QUALITY__P0 1.5
    #define FXAA3_QUALITY__P1 3.0
    #define FXAA3_QUALITY__P2 12.0
#elif (FXAA3_QUALITY__PRESET == 11)
    #define FXAA3_QUALITY__PS 4
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 3.0
    #define FXAA3_QUALITY__P3 12.0
#elif (FXAA3_QUALITY__PRESET == 12)
    #define FXAA3_QUALITY__PS 5
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 4.0
    #define FXAA3_QUALITY__P4 12.0
#elif (FXAA3_QUALITY__PRESET == 13)
    #define FXAA3_QUALITY__PS 6
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 2.0
    #define FXAA3_QUALITY__P4 4.0
    #define FXAA3_QUALITY__P5 12.0
#elif (FXAA3_QUALITY__PRESET == 14)
    #define FXAA3_QUALITY__PS 7
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 2.0
    #define FXAA3_QUALITY__P4 2.0
    #define FXAA3_QUALITY__P5 4.0
    #define FXAA3_QUALITY__P6 12.0
#elif (FXAA3_QUALITY__PRESET == 15)
    #define FXAA3_QUALITY__PS 8
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 2.0
    #define FXAA3_QUALITY__P4 2.0
    #define FXAA3_QUALITY__P5 2.0
    #define FXAA3_QUALITY__P6 4.0
    #define FXAA3_QUALITY__P7 12.0
#elif (FXAA3_QUALITY__PRESET == 20)
    #define FXAA3_QUALITY__PS 3
    #define FXAA3_QUALITY__P0 1.5
    #define FXAA3_QUALITY__P1 2.0
    #define FXAA3_QUALITY__P2 8.0
#elif (FXAA3_QUALITY__PRESET == 21)
    #define FXAA3_QUALITY__PS 4
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 8.0
#elif (FXAA3_QUALITY__PRESET == 22)
    #define FXAA3_QUALITY__PS 5
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 2.0
    #define FXAA3_QUALITY__P4 8.0
#elif (FXAA3_QUALITY__PRESET == 23)
    #define FXAA3_QUALITY__PS 6
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 2.0
    #define FXAA3_QUALITY__P4 2.0
    #define FXAA3_QUALITY__P5 8.0
#elif (FXAA3_QUALITY__PRESET == 24)
    #define FXAA3_QUALITY__PS 7
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 2.0
    #define FXAA3_QUALITY__P4 2.0
    #define FXAA3_QUALITY__P5 3.0
    #define FXAA3_QUALITY__P6 8.0
#elif (FXAA3_QUALITY__PRESET == 25)
    #define FXAA3_QUALITY__PS 8
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 2.0
    #define FXAA3_QUALITY__P4 2.0
    #define FXAA3_QUALITY__P5 2.0
    #define FXAA3_QUALITY__P6 4.0
    #define FXAA3_QUALITY__P7 8.0
#elif (FXAA3_QUALITY__PRESET == 26)
    #define FXAA3_QUALITY__PS 9
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 2.0
    #define FXAA3_QUALITY__P4 2.0
    #define FXAA3_QUALITY__P5 2.0
    #define FXAA3_QUALITY__P6 2.0
    #define FXAA3_QUALITY__P7 4.0
    #define FXAA3_QUALITY__P8 8.0
#elif (FXAA3_QUALITY__PRESET == 27)
    #define FXAA3_QUALITY__PS 10
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 2.0
    #define FXAA3_QUALITY__P4 2.0
    #define FXAA3_QUALITY__P5 2.0
    #define FXAA3_QUALITY__P6 2.0
    #define FXAA3_QUALITY__P7 2.0
    #define FXAA3_QUALITY__P8 4.0
    #define FXAA3_QUALITY__P9 8.0
#elif (FXAA3_QUALITY__PRESET == 28)
    #define FXAA3_QUALITY__PS 11
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 2.0
    #define FXAA3_QUALITY__P4 2.0
    #define FXAA3_QUALITY__P5 2.0
    #define FXAA3_QUALITY__P6 2.0
    #define FXAA3_QUALITY__P7 2.0
    #define FXAA3_QUALITY__P8 2.0
    #define FXAA3_QUALITY__P9 4.0
    #define FXAA3_QUALITY__P10 8.0
#elif (FXAA3_QUALITY__PRESET == 29)
    #define FXAA3_QUALITY__PS 12
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.5
    #define FXAA3_QUALITY__P2 2.0
    #define FXAA3_QUALITY__P3 2.0
    #define FXAA3_QUALITY__P4 2.0
    #define FXAA3_QUALITY__P5 2.0
    #define FXAA3_QUALITY__P6 2.0
    #define FXAA3_QUALITY__P7 2.0
    #define FXAA3_QUALITY__P8 2.0
    #define FXAA3_QUALITY__P9 2.0
    #define FXAA3_QUALITY__P10 4.0
    #define FXAA3_QUALITY__P11 8.0
#elif (FXAA3_QUALITY__PRESET == 39)
    #define FXAA3_QUALITY__PS 12
    #define FXAA3_QUALITY__P0 1.0
    #define FXAA3_QUALITY__P1 1.0
    #define FXAA3_QUALITY__P2 1.0
    #define FXAA3_QUALITY__P3 1.0
    #define FXAA3_QUALITY__P4 1.0
    #define FXAA3_QUALITY__P5 1.5
    #define FXAA3_QUALITY__P6 2.0
    #define FXAA3_QUALITY__P7 2.0
    #define FXAA3_QUALITY__P8 2.0
    #define FXAA3_QUALITY__P9 2.0
    #define FXAA3_QUALITY__P10 4.0
    #define FXAA3_QUALITY__P11 8.0
#endif
// Default to preset 12 if invalid preset is used.
#ifndef FXAA3_QUALITY__PS
    #define FXAA3_QUALITY__PS 5
#endif
#ifndef FXAA3_QUALITY__P0
    #define FXAA3_QUALITY__P0 1.0
#endif
#ifndef FXAA3_QUALITY__P1
    #define FXAA3_QUALITY__P1 1.5
#endif
#ifndef FXAA3_QUALITY__P2
    #define FXAA3_QUALITY__P2 2.0
#endif
#ifndef FXAA3_QUALITY__P3
    #define FXAA3_QUALITY__P3 4.0
#endif
#ifndef FXAA3_QUALITY__P4
    #define FXAA3_QUALITY__P4 12.0
#endif
void shader_fxaa3() {
    vec2 fxaaQualityRcpFrame = 1.0 / g_TextureSize.xy;
    vec2 posM;
    posM.x = g_oTexcoord.xy.x;
    posM.y = g_oTexcoord.xy.y;
    vec4 rgbyM = g_Color.xyzw;
    float lumaS = textureLodOffset(g_Texture, posM, 0.0, ivec2( 0, 1)).y;
    float lumaE = textureLodOffset(g_Texture, posM, 0.0, ivec2( 1, 0)).y;
    float lumaN = textureLodOffset(g_Texture, posM, 0.0, ivec2( 0,-1)).y;
    float lumaW = textureLodOffset(g_Texture, posM, 0.0, ivec2(-1, 0)).y;
    float rangeMax = max(max(lumaN, lumaW), max(lumaE, max(lumaS, rgbyM.y)));
    float range = rangeMax - min(min(lumaN, lumaW), min(lumaE, min(lumaS, rgbyM.y)));
    if (range < max(FXAA3_QUALITY_EDGE_THRESHOLD_MIN, rangeMax * FXAA3_QUALITY_EDGE_THRESHOLD)) {
        return;
    }
    float lumaNW = textureLodOffset(g_Texture, posM, 0.0, ivec2(-1,-1)).y;
    float lumaSE = textureLodOffset(g_Texture, posM, 0.0, ivec2( 1, 1)).y;
    float lumaNE = textureLodOffset(g_Texture, posM, 0.0, ivec2( 1,-1)).y;
    float lumaSW = textureLodOffset(g_Texture, posM, 0.0, ivec2(-1, 1)).y;
    float lumaNS = lumaN + lumaS;
    float lumaWE = lumaW + lumaE;
    float subpixNSWE = lumaNS + lumaWE;
    float lumaNESE = lumaNE + lumaSE;
    float lumaNWNE = lumaNW + lumaNE;
    float lumaNWSW = lumaNW + lumaSW;
    float edgeHorz = abs((-2.0 * lumaW) + lumaNWSW)        + (abs((-2.0 * rgbyM.y) + lumaNS) * 2.0) + abs((-2.0 * lumaE) + lumaNESE);
    float edgeVert = abs((-2.0 * lumaS) + lumaSW + lumaSE) + (abs((-2.0 * rgbyM.y) + lumaWE) * 2.0) + abs((-2.0 * lumaN) + lumaNWNE);
    float lengthSign = fxaaQualityRcpFrame.x;
    bool horzSpan = edgeHorz >= edgeVert;
    if (!horzSpan) {
        lumaN = lumaW;
        lumaS = lumaE;
    } else {
        lengthSign = fxaaQualityRcpFrame.y;
    }
    float gradientN = lumaN - rgbyM.y;
    float gradientS = lumaS - rgbyM.y;
    float lumaNN = lumaN + rgbyM.y;
    bool pairN = abs(gradientN) >= abs(gradientS);
    if (pairN) {
        lengthSign = -lengthSign;
    }
    float subpixC = clamp(abs(((subpixNSWE * 2.0 + (lumaNWSW + lumaNESE)) * (1.0/12.0)) - rgbyM.y) * (1.0/range), 0.0, 1.0);
    vec2 posB;
    posB.x = posM.x;
    posB.y = posM.y;
    vec2 offNP;
    offNP.x = (!horzSpan) ? 0.0 : fxaaQualityRcpFrame.x;
    offNP.y = ( horzSpan) ? 0.0 : fxaaQualityRcpFrame.y;
    if (!horzSpan) {
        posB.x += lengthSign * 0.5;
    } else {
        posB.y += lengthSign * 0.5;
    }
    vec2 posN;
    posN.x = posB.x - offNP.x * FXAA3_QUALITY__P0;
    posN.y = posB.y - offNP.y * FXAA3_QUALITY__P0;
    vec2 posP;
    posP.x = posB.x + offNP.x * FXAA3_QUALITY__P0;
    posP.y = posB.y + offNP.y * FXAA3_QUALITY__P0;
    float lumaEndN = textureLod(g_Texture, posN, 0.0).y;
    float lumaEndP = textureLod(g_Texture, posP, 0.0).y;
    if (!pairN) {
        lumaNN = lumaS + rgbyM.y;
    }
    float gradientScaled = max(abs(gradientN), abs(gradientS)) * 1.0/4.0;
    float subpixF = (((-2.0)*subpixC) + 3.0) * (subpixC * subpixC);
    bool lumaMLTZero = (rgbyM.y - lumaNN * 0.5) < 0.0;
    lumaEndN -= lumaNN * 0.5;
    lumaEndP -= lumaNN * 0.5;
    bool doneN = abs(lumaEndN) >= gradientScaled;
    bool doneP = abs(lumaEndP) >= gradientScaled;
    if (!doneN) {
        posN.x -= offNP.x * FXAA3_QUALITY__P1;
    }
    if (!doneN) {
        posN.y -= offNP.y * FXAA3_QUALITY__P1;
    }
    bool doneNP = (!doneN) || (!doneP);
    if (!doneP) {
        posP.x += offNP.x * FXAA3_QUALITY__P1;
    }
    if (!doneP) {
        posP.y += offNP.y * FXAA3_QUALITY__P1;
    }

    for (int fxaaQualityP = 0; fxaaQualityP < FXAA3_QUALITY__PS; fxaaQualityP++) {
        float pMult;
        bool endLoop = false;
        switch (fxaaQualityP) {
            case 2:
                pMult = FXAA3_QUALITY__P2;
                break;
#ifdef FXAA3_QUALITY__P3
            case 3:
                pMult = FXAA3_QUALITY__P3;
                break;
#endif
#ifdef FXAA3_QUALITY__P4
            case 4:
                pMult = FXAA3_QUALITY__P4;
                break;
#endif
#ifdef FXAA3_QUALITY__P5
            case 5:
                pMult = FXAA3_QUALITY__P5;
                break;
#endif
#ifdef FXAA3_QUALITY__P6
            case 6:
                pMult = FXAA3_QUALITY__P6;
                break;
#endif
#ifdef FXAA3_QUALITY__P7
            case 7:
                pMult = FXAA3_QUALITY__P7;
                break;
#endif
#ifdef FXAA3_QUALITY__P8
            case 8:
                pMult = FXAA3_QUALITY__P8;
                break;
#endif
#ifdef FXAA3_QUALITY__P9
            case 9:
                pMult = FXAA3_QUALITY__P9;
                break;
#endif
#ifdef FXAA3_QUALITY__P10
            case 10:
                pMult = FXAA3_QUALITY__P10;
                break;
#endif
#ifdef FXAA3_QUALITY__P11
            case 11:
                pMult = FXAA3_QUALITY__P11;
                break;
#endif
#ifdef FXAA3_QUALITY__P12
            case 12:
                pMult = FXAA3_QUALITY__P12;
                break;
#endif
            default:
                endLoop = true;
                break;
        }
        if (endLoop) {
            break;
        }
        if (!doneN) {
            lumaEndN = textureLod(g_Texture, posN.xy, 0.0).y;
        }
        if (!doneP) {
            lumaEndP = textureLod(g_Texture, posP.xy, 0.0).y;
        }
        if (!doneN) {
            lumaEndN = lumaEndN - lumaNN * 0.5;
        }
        if (!doneP) {
            lumaEndP = lumaEndP - lumaNN * 0.5;
        }
        doneN = abs(lumaEndN) >= gradientScaled;
        doneP = abs(lumaEndP) >= gradientScaled;
        if (!doneN) {
            posN.x -= offNP.x * pMult;
        }
        if (!doneN) {
            posN.y -= offNP.y * pMult;
        }
        doneNP = (!doneN) || (!doneP);
        if (!doneP) {
            posP.x += offNP.x * pMult;
        }
        if (!doneP) {
            posP.y += offNP.y * pMult;
        }
        if (!doneNP) {
            break;
        }
    }
    float dstN = posM.x - posN.x;
    float dstP = posP.x - posM.x;
    if (!horzSpan) {
        dstN = posM.y - posN.y;
        dstP = posP.y - posM.y;
    }
    float pixelOffsetSubpix = max(
        ((dstN < dstP) ? (lumaEndN < 0.0) != lumaMLTZero : (lumaEndP < 0.0) != lumaMLTZero) ? ((min(dstN, dstP) * (-(1.0/(dstP + dstN)))) + 0.5) : 0.0,
        (subpixF * subpixF) * FXAA3_QUALITY_SUBPIX
    );
    if(!horzSpan) {
        posM.x += pixelOffsetSubpix * lengthSign;
    } else {
        posM.y += pixelOffsetSubpix * lengthSign;
    }
    g_Color.rgb = textureLod(g_Texture, posM, 0.0).xyz;
}
#endif // FXAA3_ENABLED
