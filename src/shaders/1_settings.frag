// Scroll down to "Start of user configuration"
#version 140
#define SHADER_DEBAND           0
#define SHADER_VIBRANCE         1
#define SHADER_SEPIA            2
#define SHADER_NATURAL_VISION   3
#define SHADER_TECHNICOLOR1     4
#define SHADER_TECHNICOLOR2     5
#define SHADER_DPX              6
#define SHADER_FAKE_HDR         7
#define SHADER_TONEMAP          8
#define SHADER_LIFT_GAMMA_GAIN  9
#define SHADER_LEVELS           10
#define SHADER_FXAA3            11
#define SHADER_GAUSS_BLUR_H     12
#define SHADER_GAUSS_BLUR_V     13
#define SHADER_AMD_CAS          14
#define SHADER_NVIDIA_DLS       15
#define SHADER_FAST_SHARPEN     16
#define SHADER_ADAPTIVE_SHARPEN 17
#define SHADER_FILM_NOISE       18
#define SHADERS                 19
//----------------------------------------------------------------
//----------------------------------------------------------------
//------------------ Start of user configuration -----------------
//----------------------------------------------------------------
//---------- NOTE: All shaders are disabled by default -----------

//----------------------------------------------------------------
//---------------- Order the Shaders Will Be Run -----------------
//----------------------------------------------------------------
// Move up or down a SHADER_NAME to change the position it will be run.
// For example, if you want Fake HDR to run before all the other shaders,
// move SHADER_FAKE_HDR, above SHADER_DEBAND,
const int SHADER_ORDER[SHADERS+1] = int[] ( // Don't change this line.

    SHADER_DEBAND,
    SHADER_VIBRANCE,
    SHADER_SEPIA,
    SHADER_NATURAL_VISION,
    SHADER_TECHNICOLOR1,
    SHADER_TECHNICOLOR2,
    SHADER_DPX,
    SHADER_FAKE_HDR,
    SHADER_TONEMAP,
    SHADER_LIFT_GAMMA_GAIN,
    SHADER_LEVELS,
    SHADER_FXAA3,
    SHADER_GAUSS_BLUR_H,
    SHADER_GAUSS_BLUR_V,
    SHADER_AMD_CAS,
    SHADER_NVIDIA_DLS,
    SHADER_FAST_SHARPEN,
    SHADER_ADAPTIVE_SHARPEN,
    SHADER_FILM_NOISE,

SHADERS); // Don't change this line.

//----------------------------------------------------------------
//------------ Adaptive Sharpen configuration section ------------
//----------------------------------------------------------------
// https://gist.github.com/igv/8a77e4eb8276753b54bb94c1c50c317e

// NOTE: This shader can be slow, consider using CAS, DLS or Fast Sharpen if you experience framedrops.

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
//---------------- Deband configuration section ------------------
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
//------------------ DPX configuration section -------------------
//----------------------------------------------------------------
// https://github.com/CeeJayDK/SweetFX/blob/master/Shaders/DPX.fx

// Set to 1 to enable.
#define DPX_ENABLED 0
#if DPX_ENABLED == 1 // Don't change this line.

// Adjust the strength of the effect.
// 0.0 to 1.0
// Default: 0.02
#define DPX_STRENGTH 0.02

// 0.0 to 8.0
// Default: 3.0
#define DPX_SATURATION 3.0

// 0.0 to 1.0
// Default: 0.1
#define DPX_CONTRAST 0.1

// 0.1 to 2.5
// Default: 2.5, 2.5, 2.5
uniform vec3 DPX_COLORFULNESS = vec3(2.5, 2.5, 2.5);

// 1.0 to 15.0
// Default: 8.0, 8.0, 8.0
uniform vec3 DPX_RGB_CURVE = vec3(8.0, 8.0, 8.0);

// 0.2 to 0.5
// Default: 0.36, 0.36, 0.34
uniform vec3 DPX_RGB_C = vec3(0.36, 0.36, 0.34);

#endif
//----------------------------------------------------------------
//---------------- Fake HDR configuration section ----------------
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
//------------- Fast Sharpen configuration section ---------------
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
//-------------- Film Noise configuration section ----------------
//----------------------------------------------------------------
// https://github.com/libretro/glsl-shaders/blob/master/film/shaders/film_noise.glsl

// Adds film noise to the output. Works better on non static images.

// Set to 1 to enable.
#define FILM_NOISE_ENABLED 0
#if FILM_NOISE_ENABLED == 1 // Don't change this line.

// The amount of noise.
// Default 2.0
#define FILM_NOISE_STRENGTH 2.0

#endif
//----------------------------------------------------------------
//---------------- FXAA3 configuration section -------------------
//----------------------------------------------------------------
// https://gist.github.com/kosua20/0c506b81b3812ac900048059d2383126

// Set to 1 to enable.
#define FXAA3_ENABLED 0
#if FXAA3_ENABLED == 1 // Don't change this line.

// Choose the amount of sub-pixel aliasing removal.
// This can effect sharpness.
//   1.00 - upper limit (softer)
//   0.75 - default amount of filtering
//   0.50 - lower limit (sharper, less sub-pixel aliasing removal)
//   0.25 - almost off
//   0.00 - completely off
// Default: 0.75
#define FXAA3_QUALITY_SUBPIX 0.75

// The minimum amount of local contrast required to apply algorithm.
//   0.333 - too little (faster)
//   0.250 - low quality
//   0.166 - default
//   0.125 - high quality
//   0.063 - overkill (slower)
// Default: 0.166
#define FXAA3_QUALITY_EDGE_THRESHOLD 0.166

// Trims the algorithm from processing darks.
//   0.0833 - upper limit (default, the start of visible unfiltered edges)
//   0.0625 - high quality (faster)
//   0.0312 - visible limit (slower)
// Default: 0.0833
#define FXAA3_QUALITY_EDGE_THRESHOLD_MIN 0.0833

// Choose the quality preset.
//
// OPTIONS
// -----------------------------------------------------------------------
// 10 to 15 - default medium dither (10=fastest, 15=highest quality)
// 20 to 29 - less dither, more expensive (20=fastest, 29=highest quality)
// 39       - no dither, very expensive
//
// NOTES
// -----------------------------------------------------------------------
// 12 = slightly faster then FXAA 3.9 and higher edge quality (default)
// 13 = about same speed as FXAA 3.9 and better than 12
// 23 = closest to FXAA 3.9 visually and performance wise
//  _ = the lowest digit is directly related to performance
// _  = the highest digit is directly related to style
// Default: 12
#define FXAA3_QUALITY__PRESET 12


#endif
//----------------------------------------------------------------
//------------ Gaussian Blur H configuration section -------------
//----------------------------------------------------------------
// https://github.com/libretro/glsl-shaders/blob/master/blurs/blur-gauss-h.glsl

#define GAUSSBLURH_ENABLED 0
#if GAUSSBLURH_ENABLED == 1 // Don't change this line.

// Default: 1.0
#define GAUSSBLURH_STRENGTH 1.0

#endif
//----------------------------------------------------------------
//------------ Gaussian Blur V configuration section -------------
//----------------------------------------------------------------
// https://github.com/libretro/glsl-shaders/blob/master/blurs/blur-gauss-v.glsl

#define GAUSSBLURV_ENABLED 0
#if GAUSSBLURV_ENABLED == 1 // Don't change this line.

// Default: 1.0
#define GAUSSBLURV_STRENGTH 1.0

#endif
//----------------------------------------------------------------
//-------------- Levels configuration section --------------------
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
// 0 to 255
// Default: 16
#define LVLS_BLACK_POINT 16

// The new white point. Everything brighter than this becomes completely white.
// 0 to 255
// Default: 235
#define LVLS_WHITE_POINT 235

#endif
//----------------------------------------------------------------
//------------ Lift Gamma Gain configuration section -------------
//----------------------------------------------------------------
// https://github.com/CeeJayDK/SweetFX/blob/master/Shaders/LiftGammaGain.fx

// Set to 1 to enable.
#define LIFT_GAMMA_GAIN_ENABLED 0
#if LIFT_GAMMA_GAIN_ENABLED == 1 // Don't change this line.

// Adjust shadows for red, green and blue.
// 0.0 to 2.0
// Default: 1.0, 1.0, 1.0
uniform vec3 LGG_RGB_LIFT = vec3(1.0, 1.0, 1.0);

// Adjust midtones for red, green and blue
// 0.0 to 2.0
// Default: 1.0, 1.0, 1.0
uniform vec3 LGG_RGB_GAMMA = vec3(1.0, 1.0, 1.0);

// Adjust highlights for red, green and blue.
// 0.0 to 2.0
// Default: 1.0, 1.0, 1.0
uniform vec3 LGG_RGB_GAIN = vec3(1.0, 1.0, 1.0);

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
//------------ Natural Vision configuration section --------------
//----------------------------------------------------------------
// https://github.com/libretro/glsl-shaders/blob/master/misc/natural-vision.glsl

// NOTE: This shader causes issues with green colors when used with some other shaders.

// Set to 1 to enable.
#define NATURAL_VISION_ENABLED 0
#if NATURAL_VISION_ENABLED == 1 // Don't change this line.

// Input Gamma
// Default: 2.2
#define NATURAL_VISION_GIN  2.2

// Output Gamma
// Default: 2.2
#define NATURAL_VISION_GOUT 2.2

// Luminance mutiplier
// Default: 1.1
#define NATURAL_VISION_Y    1.1

// Orange to Cyan multiplier
// Default: 1.1
#define NATURAL_VISION_I    1.1

// Magenta to Green multiplier
// Default 1.1
#define NATURAL_VISION_Q    1.1

#endif
//----------------------------------------------------------------
//----------------- Sepia configuration section ------------------
//----------------------------------------------------------------
// https://github.com/CeeJayDK/SweetFX/blob/master/Shaders/Sepia.fx

// Set to 1 to enable.
#define SEPIA_ENABLED 0
#if SEPIA_ENABLED == 1 // Don't change this line.

// Adjust the strength of the effect.
// 0.0 to 1.0
// Default: 0.58
#define SEPIA_STRENGTH 0.58

// Default: 0.55, 0.43, 0.42
uniform vec3 SEPIA_TINT = vec3(0.55, 0.43, 0.42);

#endif
//----------------------------------------------------------------
//------------- Techicolor 1 configuration section ---------------
//----------------------------------------------------------------
// https://github.com/CeeJayDK/SweetFX/blob/master/Shaders/Technicolor.fx

// Set to 1 to enable.
#define TECHNICOLOR1_ENABLED 0
#if TECHNICOLOR1_ENABLED == 1 // Don't change this line.

// Default: 4.0
// 0.0 to 8.0
#define TC1_POWER 4.0

// Default: 0.88 0.88 0.88
uniform vec3 TC1_RGB_NEGATIVE_AMOUNT = vec3(0.88, 0.88, 0.88);

// Adjust the strength of the effect.
// Default: 0.4
// 0.0 to 1.0
#define TC1_STRENGTH 0.4

#endif
//----------------------------------------------------------------
//------------- Techicolor 2 configuration section ---------------
//----------------------------------------------------------------
// https://github.com/CeeJayDK/SweetFX/blob/master/Shaders/Technicolor2.fx

// Set to 1 to enable.
#define TECHNICOLOR2_ENABLED 0
#if TECHNICOLOR2_ENABLED == 1 // Don't change this line.

// Higher means darker and more intense colors.
// Default: 0.2 0.2 0.2
uniform vec3 TC2_COLORSTRENGTH = vec3(0.2, 0.2, 0.2);

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
//---------------- Tonemap configuration section -----------------
//----------------------------------------------------------------
// https://github.com/CeeJayDK/SweetFX/blob/master/Shaders/Tonemap.fx

// Set to 1 to enable.
#define TONEMAP_ENABLED 0
#if TONEMAP_ENABLED == 1 // Don't change this line.

// Adjust midtones. 1.0 is neutral.
// 0.0 to 2.0
// Default: 1.0
#define TONEMAP_GAMMA 1.0

// Adjust exposure
// -1.0 to 1.0
// Default: 0.0
#define TONEMAP_EXPOSURE 0.0

// Adjust saturation
// -1.0 to 1.0
// Default: 0.0
#define TONEMAP_SATURATION 0.0

// Brightens the shadows and fades the colors
// 0.0 to 1.0
// Default: 0.0
#define TONEMAP_BLEACH 0.0

// How much of the color tint to remove
// 0.0 to 1.0
// Default: 0.0
#define TONAMEP_DEFOG 0.0

// Which color tint to remove
// Default: 0.0, 0.0, 1.0
uniform vec3 TONEMAP_FOGCOLOR = vec3(0.0, 0.0, 1.0);

#endif
//----------------------------------------------------------------
//--------------- Vibrance configuration section -----------------
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
uniform vec3 VIB_RGB_BALANCE = vec3(1.0, 1.0, 1.0);

// Luma type
// 0 -> Perceptual
// 1 -> Even
// Default: 0
#define VIB_LUMA 0

#endif
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------- End of user configuration --------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
