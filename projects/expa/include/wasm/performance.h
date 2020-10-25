#ifndef GOS_EXPA_WASM_PERFORMANCE_H_
#define GOS_EXPA_WASM_PERFORMANCE_H_

#include <stdbool.h>
#include <time.h>

#include <wasm/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void gos_performance_initialize(gos_performance* performance);
bool gos_performance_initialize_sd(gos_performance* performance, size_t size);

void gos_performance_reset(gos_performance* performance);
void gos_performance_reset_sd(gos_performance* performance);

void gos_performance_update(gos_performance* performance, double value);
bool gos_performance_update_sd(gos_performance* performance, double value);

void gos_performance_calculate(gos_performance* performance);
void gos_performance_calculate_sd(gos_performance* performance);

int gos_performance_format(gos_performance* performance, char* b, size_t s);
int gos_performance_format_sd(gos_performance* performance, char* b, size_t s);

int gos_performance_start(gos_performance* performance);
double gos_performance_completed_s(gos_performance* performance);
double gos_performance_completed_ms(gos_performance* performance);

double gos_performance_measure_s(struct timespec* before);
double gos_performance_measure_ms(struct timespec* before);

void gos_performance_shutdown(gos_performance* performance);

#ifdef __cplusplus
}
#endif

#endif
