#if ADVANCED_CARTOON_ENABLED == 1
// Advanced Cartoon shader I and II
// by guest(r) (guest.r@gmail.com)
// license: GNU-GPL

out vec2 AC_Coord_0;
out vec2 AC_Coord_1;
out vec2 AC_Coord_2;
out vec2 AC_Coord_3;
out vec2 AC_Coord_4;
out vec2 AC_Coord_5;
out vec2 AC_Coord_6;
out vec2 AC_Coord_7;
out vec2 AC_Coord_8;

uniform vec2 AC_OGL2Param = vec2(ADVANCED_CARTOON_BORDER, ADVANCED_CARTOON_BORDER);
void shader_advanced_cartoon() {
   float x = (g_SourceSize.z / 8.0) * AC_OGL2Param.x;
   float y = (g_SourceSize.w / 8.0) * AC_OGL2Param.y;
   vec2 dg1 = vec2( x, y);
   vec2 dg2 = vec2(-x, y);
   vec2 dx  = vec2(x, 0.0);
   vec2 dy  = vec2(0.0, y);
   AC_Coord_0.xy = g_oTexcoord.xy;
   AC_Coord_1.xy = g_oTexcoord.xy - dy;
   AC_Coord_2.xy = g_oTexcoord.xy + dy;
   AC_Coord_3.xy = g_oTexcoord.xy - dx;
   AC_Coord_4.xy = g_oTexcoord.xy + dx;
   AC_Coord_5.xy = g_oTexcoord.xy - dg1;
   AC_Coord_6.xy = g_oTexcoord.xy + dg1;
   AC_Coord_7.xy = g_oTexcoord.xy - dg2;
   AC_Coord_8.xy = g_oTexcoord.xy + dg2;
}
#endif
