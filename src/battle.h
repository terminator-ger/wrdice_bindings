#ifndef BATTLE_H
#define BATTLE_H

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "unit_values.h"
#include "vec_op.h"
#if _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FFI_PLUGIN_EXPORT
#endif

void update_stats_counts(Survived stats[5], int N);
void simulate_battle(const Army *restrict army_a,
                     const Army *restrict army_b,
                     SimStats *restrict stats,
                     bool with_force_advantage,
                     bool with_batch_cap);
void update_force_advantage(Army *restrict aa, Army *restrict ab, bool fa[2]);

void roll_batch(int n_dice, uint32_t results[6]);
uint32_t get_dice_air(const Army *army);
uint32_t get_dice_ground(const Army *army);

void apply_batch_cap(const Army *restrict aa, const Army *restrict ab, int *dice_a, int *dice_b);
#endif