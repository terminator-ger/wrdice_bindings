#ifndef VEC_OP_H
#define VEC_OP_H

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "unit_values.h"
#include "wrdice.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLIP(a, l, u) (MAX(l, MIN(a, u)))

static inline int div_ceil(int nom, int denom){
    return denom ? ((nom + denom - 1) / denom) : 0;
}

static inline void vec_min(int* array, int size, int lower){
    for(int i = 0; i < size; i++){
        array[i] = MAX(lower, array[i]);
    }
}


static inline void army_to_hp(const Army* army, ArmyHP* ahp){
    for(int tvpe=0; tvpe<5;tvpe++){
        ahp->lnd.hp_stance_off[tvpe] += (army->stance_lnd.stance_off[tvpe] * lnd_hp_stance_off[tvpe]);
        ahp->lnd.hp_stance_def[tvpe] += (army->stance_lnd.stance_def[tvpe] * lnd_hp_stance_def[tvpe]);

        ahp->air.hp_stance_off[tvpe] += (army->stance_air.stance_off[tvpe] * air_hp_stance_off[tvpe]);
        ahp->air.hp_stance_def[tvpe] += (army->stance_air.stance_def[tvpe] * air_hp_stance_def[tvpe]);

        ahp->sea.hp_stance_off[tvpe] += (army->stance_sea.stance_off[tvpe] * sea_hp_stance_off[tvpe]);
        ahp->sea.hp_stance_def[tvpe] += (army->stance_sea.stance_def[tvpe] * sea_hp_stance_def[tvpe]);
    }
}


static inline void vec_div_scalar(float* restrict left, const uint32_t* restrict right, int scalar, int n){
    float f = 1.0/scalar;
    for (int i = 0; i < n; i++){
        left[i] =  right[i] * f;
    }
}


static inline void vec_sub_to_1_hp(uint32_t* restrict left, uint32_t* restrict right, int n, uint32_t hp){
    for (int i = 0; i < n; i++){
        int dmg = MIN((hp - 1), right[i]);
        left[i] -= dmg;
        right[i] -= dmg;
    }
}


static inline void vec_sub_to_0_hp(uint32_t* restrict left, uint32_t* restrict right, int n){
    for (int i = 0; i < n; i++){
        int dmg = MIN(left[i], right[i]);
        left[i] -= dmg;
        right[i] -= dmg;
    }
}

static inline int vec_n_components(const uint32_t* restrict left, int n){
    int cnt = 0; 
    for (int i = 0; i < n; i++){
        if (left[i] > 0){
            cnt += 1;
        }
    }
    return cnt;
}

#endif

