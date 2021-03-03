#ifndef WD3_HM_C_MD_HM_H_
#define WD3_HM_C_MD_HM_H_

#include <stdbool.h>

#include <modules/hm/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void ghm_initialize(wd3hm* context, int width, int height);
void ghm_parse_argument(wd3hm* context, int argc, char** argv);
bool ghm_create(wd3hm* context);
bool ghm_create_data(wd3hm* context);
bool ghm_create_buffer(wd3hm* context);
bool ghm_create_separation(wd3hm* context);
bool ghm_create_gradient(wd3hm* context);
bool ghm_create_stops(wd3hm* context);
bool ghm_change_stops(wd3hm* context, int count, bool copy);
bool ghm_change(wd3hm* context, int count, bool copy);
bool ghm_data_set(wd3hm* context, int index, double x, double y);
bool ghm_data_check(wd3hm* context);
bool ghm_set_stop(wd3hm* context, int index, const char* color);
void ghm_set_size(wd3hm* context, int index, int size);
double ghm_point_determination(wd3hm* context, double value);
void ghm_buffer_insert(wd3hm* context, int at);
void ghm_buffer_insert_last(wd3hm* context);
void ghm_buffer_shift(wd3hm* context);
void ghm_buffer_set(wd3hm* context, int at);
void ghm_rescale(wd3hm* context);
double ghm_domain_get(wd3hm* context, int flag);
void ghm_domain_set(wd3hm* context, int flag, double value);
void ghm_domain_y_set(wd3hm* context, double from, double to);
void ghm_domain_z_set(wd3hm* context, double from, double to);
void ghm_draw_pixel(wd3hm* context, int x, int y, Uint32 pixel);
bool ghm_set_undefined(wd3hm* context, const char* color);
bool ghm_set_separation(wd3hm* context, int index, const char* color);
bool ghm_set_all_separation(wd3hm* context, const char* color);
void ghm_render(wd3hm* context, int x);
void ghm_render_last(wd3hm* context);
void ghm_render_all(wd3hm* context);
void ghm_render_separation(wd3hm* context);
bool ghm_lock(wd3hm* context);
bool ghm_flip(wd3hm* context);
void ghm_unlock(wd3hm* context);
void ghm_shift(wd3hm* context);
void ghm_shutdown(wd3hm* context);
void ghm_shutdown_data(wd3hm* context);
void ghm_shutdown_buffer(wd3hm* context);
void ghm_shutdown_separation(wd3hm* context);
void ghm_shutdown_gradient(wd3hm* context);
void ghm_shutdown_stops(wd3hm* context);

#ifdef __cplusplus
}
#endif

#endif
