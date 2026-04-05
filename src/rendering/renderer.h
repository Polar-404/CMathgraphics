#ifdef __cplusplus
extern "C" {
#endif

void init_opengl();
void render_frame();
void cleanup_opengl();
void update_points(float* data, int count);

#ifdef __cplusplus
}
#endif