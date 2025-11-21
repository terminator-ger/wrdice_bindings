#ifndef UNIT_VALUES_H
#define UNIT_VALUES_H

#include <stdint.h>

// Unit Combat Values
// A = Offensive/ Ground     (lower row)
// B = Defensive/ AA/ Escort (upper row) 
static const uint32_t air_vs_ground_stance_off[5]  = {0,0,3,0,0};
static const uint32_t air_vs_ground_stance_def[5]  = {0,0,0,4,0};
static const uint32_t air_vs_air_stance_off[5]     = {0,0,0,1,0};
static const uint32_t air_vs_air_stance_def[5]     = {0,0,3,1,0};

static const uint32_t lnd_vs_ground_stance_off[5]  = {2,2,4,0,0};
static const uint32_t lnd_vs_ground_stance_def[5]  = {1,1,2,0,0};
static const uint32_t lnd_vs_air_stance_off[5]     = {0,0,0,0,0};
static const uint32_t lnd_vs_air_stance_def[5]     = {0,2,1,0,0};

static const uint32_t sea_vs_ground_stance_off[5]  = {2,3,2,4,0};
static const uint32_t sea_vs_ground_stance_def[5]  = {2,2,1,3,0};
static const uint32_t sea_vs_air_stance_off[5]     = {0,1,1,1,0};
static const uint32_t sea_vs_air_stance_def[5]     = {0,2,2,2,0};

// Unit HP Values
static const uint32_t air_hp_stance_off[5] = {0,0,2,2,0};
static const uint32_t air_hp_stance_def[5] = {0,0,2,2,0};

static const uint32_t lnd_hp_stance_off[5] = {1,2,2,0,0};
static const uint32_t lnd_hp_stance_def[5] = {2,2,4,0,0};

static const uint32_t sea_hp_stance_off[5] = {2,2,3,3,0};
static const uint32_t sea_hp_stance_def[5] = {2,2,3,3,0};

#endif