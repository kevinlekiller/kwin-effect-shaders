#if FILM_NOISE_ENABLED == 1
// film noise
// by hunterk
// license: public domain
// Modified by kevinlekiller to only use the grain part.
void shader_film_noise() {
    float grain = (g_oTexcoord.x + 4.0) * (g_oTexcoord.y + 4.0) * ((mod(g_Random, 800.0) + 10.0) * 10.0);
    g_Color += (mod((mod(grain, 13.0) + 1.0) * (mod(grain, 123.0) + 1.0), 0.01)-0.005) * FILM_NOISE_STRENGTH;
}
#endif // FILM_NOISE_ENABLED
