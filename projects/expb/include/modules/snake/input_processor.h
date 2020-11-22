#ifndef INPUT_PROCESSOR_H
#define INPUT_PROCESSOR_H

#include <stdbool.h>

#include <modules/snake/types.h>

void input_processor_init();
bool add_binding(key_state_binding *binding);
bool rm_binding(key_state_binding *binding);
void process_input(bool* goer);
void activate_state(game_state state);
void deactivate_state(game_state state);
bool is_state_active(game_state state);

bool test_input_processor();

#endif
