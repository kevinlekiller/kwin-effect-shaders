#if GAUSSBLURH_ENABLED == 1
// Implementation based on the article "Efficient Gaussian blur with linear sampling"
// http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
/* A version for MasterEffect Reborn, a standalone version, and a custom shader version for SweetFX can be
   found at http://reshade.me/forum/shader-presentation/27-gaussian-blur-bloom-unsharpmask */

void shader_gauss_blur_h() {
    vec2 PIXEL_SIZE = g_SourceSize.zw;
    float sampleOffsets2 = 1.4347826;
    float sampleOffsets3 = 3.3478260;
    float sampleOffsets4 = 5.2608695;
    float sampleOffsets5 = 7.1739130;

    float sampleWeights2 = 0.27276957;
    float sampleWeights3 = 0.11690125;
    float sampleWeights4 = 0.024067905;
    float sampleWeights5 = 0.0021112196;

    g_Color *= 0.16818994;
    g_Color += texture(g_Texture, g_oTexcoord + vec2(sampleOffsets2 * GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights2;
    g_Color += texture(g_Texture, g_oTexcoord - vec2(sampleOffsets2 * GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights2;
    g_Color += texture(g_Texture, g_oTexcoord + vec2(sampleOffsets3 * GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights3;
    g_Color += texture(g_Texture, g_oTexcoord - vec2(sampleOffsets3 * GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights3;
    g_Color += texture(g_Texture, g_oTexcoord + vec2(sampleOffsets4 * GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights4;
    g_Color += texture(g_Texture, g_oTexcoord - vec2(sampleOffsets4 * GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights4;
    g_Color += texture(g_Texture, g_oTexcoord + vec2(sampleOffsets5 * GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights5;
    g_Color += texture(g_Texture, g_oTexcoord - vec2(sampleOffsets5 * GAUSSBLURH_STRENGTH * PIXEL_SIZE.x, 0.0)) * sampleWeights5;
}
#endif // GAUSSBLURH_ENABLED
