#if NATURAL_VISION_ENABLED == 1
// GPL2 or later : https://web.archive.org/web/20210307115242/http://www.razyboard.com/system/morethread-natural-vision-shader-aa-shader-v2o-best-shader-for-2d-pete_bernert-266904-5685248-0.html

void shader_natural_vision() {
    g_Color.rgb = pow(g_Color.xyz, vec3(NATURAL_VISION_GIN, NATURAL_VISION_GIN, NATURAL_VISION_GIN));
    g_Color.rgb *= mat3x3(0.299, 0.587, 0.114, 0.595716, -0.274453, -0.321263, 0.211456, -0.522591,  0.311135);
    g_Color.rgb = vec3(pow(g_Color.x, NATURAL_VISION_Y), g_Color.y * NATURAL_VISION_I, g_Color.z * NATURAL_VISION_Q);
    g_Color.rgb = clamp(g_Color.rgb, vec3(0, -0.595716, -0.522591), vec3(1, 0.595716, 0.522591));
    g_Color.rgb *= mat3x3(1, 0.95629572,  0.62102442, 1, -0.27212210, -0.64738060, 1, -1.10698902,  1.70461500);
    g_Color.rgb = pow(g_Color.rgb, vec3(1.0 / NATURAL_VISION_GOUT, 1.0 / NATURAL_VISION_GOUT, 1.0 / NATURAL_VISION_GOUT));
}
#endif // NATURAL_VISION_ENABLED
