#if ADVANCED_CARTOON_ENABLED == 1
// Advanced Cartoon shader I and II
// by guest(r) (guest.r@gmail.com)
// license: GNU-GPL

in vec2 AC_Coord_0;
in vec2 AC_Coord_1;
in vec2 AC_Coord_2;
in vec2 AC_Coord_3;
in vec2 AC_Coord_4;
in vec2 AC_Coord_5;
in vec2 AC_Coord_6;
in vec2 AC_Coord_7;
in vec2 AC_Coord_8;

void shader_advanced_cartoon() {
    vec3 c00 = texture(g_Texture, AC_Coord_5.xy).xyz;
    vec3 c10 = texture(g_Texture, AC_Coord_1.xy).xyz;
    vec3 c20 = texture(g_Texture, AC_Coord_8.xy).xyz;
    vec3 c01 = texture(g_Texture, AC_Coord_3.xy).xyz;
    vec3 c11 = texture(g_Texture, AC_Coord_0.xy).xyz;
    vec3 c21 = texture(g_Texture, AC_Coord_4.xy).xyz;
    vec3 c02 = texture(g_Texture, AC_Coord_7.xy).xyz;
    vec3 c12 = texture(g_Texture, AC_Coord_2.xy).xyz;
    vec3 c22 = texture(g_Texture, AC_Coord_6.xy).xyz;
    vec3 dt = vec3(1.0,1.0,1.0);

    float d1 = dot(abs(c00 - c22), dt);
    float d2 = dot(abs(c20 - c02), dt);
    float hl = dot(abs(c01 - c21), dt);
    float vl = dot(abs(c10 - c12), dt);
    float d = ADVANCED_CARTOON_BLACK_BORDER_SEN * (d1 + d2 + hl + vl) / (dot(c11, dt) + 0.15);

    float lc, f;
    vec3 frct;

    if(ADVANCED_CARTOON_MUTE_COLORS < 0.5) {
        lc = 4.0 * length(c11);
        f = fract(lc);
        f *= f;
        lc = 0.25 * (floor(lc) + f * f) + 0.05;
        c11 = 4.0 * normalize(c11);
        vec3 frct = fract(c11);
        frct *= frct;
        c11 = floor(c11) + 0.05 * dt + frct * frct;
        g_Color.xyz = 0.25 * lc * (1.1 - d * sqrt(d)) *c11;
    } else {
        lc = 5.0 * length(c11);
        lc = 0.2 * (floor(lc) + pow(fract(lc), 4.0));
        c11 = 4.0 * normalize(c11);
        frct = fract(c11);
        frct *= frct;
        c11 = floor(c11) + frct * frct;
        c11 = 0.25 * (c11) * lc;
        lc *= 0.577;
        c11 = mix(c11, lc * dt, lc);
        g_Color.xyz = (1.1 - pow(d, 1.5)) * c11;
    }
}
#endif
