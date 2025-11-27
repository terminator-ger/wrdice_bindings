#ifndef WR_DICE_H
#define WR_DICE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#if _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FFI_PLUGIN_EXPORT
#endif

typedef struct{
    uint32_t vs_air[5];
    uint32_t vs_gnd[5];
}DiceDistribution;

typedef struct{
    DiceDistribution air;
    DiceDistribution lnd;
    DiceDistribution sea;
} Dice;

typedef struct {
    float win_a;
    float win_b;
    float draw; 
    float death;
} BattleResult;

typedef struct {
    uint32_t    size;
    uint32_t    count[256];
    float       odds[256];
} Survived;

typedef struct{
    Survived stats_lnd[5];
    Survived stats_air[5];
    Survived stats_sea[5];
} Stats;

typedef struct{
    Stats army_a;
    Stats army_b;
    BattleResult br;
} SimStats;

typedef struct{
    uint32_t stance_off[5];
    uint32_t stance_def[5];
} Stance;

typedef struct{
    uint32_t n_units_lnd[5];
    uint32_t n_units_air[5];
    uint32_t n_units_sea[5];
    Stance   stance_lnd;
    Stance   stance_air;
    Stance   stance_sea;
} Army;

typedef struct{
    uint32_t hp_stance_off[5];
    uint32_t hp_stance_def[5];
} HP;

typedef struct{
    HP lnd;
    HP air;
    HP sea;
} ArmyHP;


FFI_PLUGIN_EXPORT void run_simulation(const Army* restrict army_a, 
                                      const Army* restrict army_b, 
                                      SimStats*   restrict stats,
                                      bool                 with_force_advantage,
                                      bool                 with_batch_cap);
 

#endif