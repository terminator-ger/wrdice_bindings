#include "battle.h"

static inline int roll_die(void){
    return (uint64_t) rand() * 12 / RAND_MAX;
}


void roll_batch(int n_dice, uint32_t results[6]){
    for (int i = 0; i<n_dice; i++){
        int res = roll_die();
        switch(res){
            case 0: case 1: case 2: case 3:
                results[0] += 1;
                break;
            case 4: case 5: case 6: 
                results[1] += 1;
                break;
            case 7: case 8:
                results[2] += 1;
                break;
            case 9:
                results[3] += 1;
                break;
            case 10:
                results[4] += 1;
                break;
            case 11:
                results[5] += 1;
                break;
        }
    }
}


uint32_t get_dice_air(const Army* army){
    uint32_t n_dice = 0;
    for(int tvpe=0; tvpe<5; tvpe++){
        // dice form air
        n_dice += (army->stance_air.stance_off[tvpe] * air_vs_air_stance_off[tvpe]);
        n_dice += (army->stance_air.stance_def[tvpe] * air_vs_air_stance_def[tvpe]);
        // dice from lnd
        n_dice += (army->stance_lnd.stance_off[tvpe] * lnd_vs_air_stance_off[tvpe]);
        n_dice += (army->stance_lnd.stance_def[tvpe] * lnd_vs_air_stance_def[tvpe]);
        // dice from sea
        n_dice += (army->stance_sea.stance_off[tvpe] * sea_vs_air_stance_off[tvpe]);
        n_dice += (army->stance_sea.stance_def[tvpe] * sea_vs_air_stance_def[tvpe]);
    }
    return n_dice;
}


uint32_t get_dice_ground(const Army* army){
    uint32_t n_dice = 0;
    for(int tvpe=0; tvpe<5; tvpe++){
        // dice from air
        n_dice += (army->stance_air.stance_off[tvpe] * air_vs_ground_stance_off[tvpe]);
        n_dice += (army->stance_air.stance_def[tvpe] * air_vs_ground_stance_def[tvpe]);
        // dice from lnd
        n_dice += (army->stance_lnd.stance_off[tvpe] * lnd_vs_ground_stance_off[tvpe]);
        n_dice += (army->stance_lnd.stance_def[tvpe] * lnd_vs_ground_stance_def[tvpe]);
        // dice from sea
        n_dice += (army->stance_sea.stance_off[tvpe] * sea_vs_ground_stance_off[tvpe]);
        n_dice += (army->stance_sea.stance_def[tvpe] * sea_vs_ground_stance_def[tvpe]);
    }
    return n_dice;
}

void get_dice_for_army(const Army* army, Dice* dice){
    for(int tvpe=0; tvpe<5; tvpe++){
        // dice for air
        dice->air.vs_air[tvpe] =  (army->stance_air.stance_off[tvpe] * air_vs_air_stance_off[tvpe]);
        dice->air.vs_air[tvpe] += (army->stance_air.stance_def[tvpe] * air_vs_air_stance_def[tvpe]);

        dice->air.vs_gnd[tvpe] =  (army->stance_air.stance_off[tvpe] * air_vs_ground_stance_off[tvpe]);
        dice->air.vs_gnd[tvpe] += (army->stance_air.stance_def[tvpe] * air_vs_ground_stance_def[tvpe]);
        
        // dice for lnd
        dice->lnd.vs_air[tvpe] =  (army->stance_lnd.stance_off[tvpe] * lnd_vs_air_stance_off[tvpe]);
        dice->lnd.vs_air[tvpe] += (army->stance_lnd.stance_def[tvpe] * lnd_vs_air_stance_def[tvpe]);
        
        dice->lnd.vs_gnd[tvpe] =  (army->stance_lnd.stance_off[tvpe] * lnd_vs_ground_stance_off[tvpe]);
        dice->lnd.vs_gnd[tvpe] += (army->stance_lnd.stance_def[tvpe] * lnd_vs_ground_stance_def[tvpe]);
        
        // dice for sea
        dice->sea.vs_air[tvpe] =  (army->stance_sea.stance_off[tvpe] * sea_vs_air_stance_off[tvpe]);
        dice->sea.vs_air[tvpe] += (army->stance_sea.stance_def[tvpe] * sea_vs_air_stance_def[tvpe]);
        
        dice->sea.vs_gnd[tvpe] =  (army->stance_sea.stance_off[tvpe] * sea_vs_ground_stance_off[tvpe]);
        dice->sea.vs_gnd[tvpe] += (army->stance_sea.stance_def[tvpe] * sea_vs_ground_stance_def[tvpe]);
   }
    CLIP_VEC(dice->air.vs_air, 5, 0, 30);
    CLIP_VEC(dice->air.vs_gnd, 5, 0, 30);
    CLIP_VEC(dice->lnd.vs_air, 5, 0, 30);
    CLIP_VEC(dice->lnd.vs_gnd, 5, 0, 30);
    CLIP_VEC(dice->sea.vs_air, 5, 0, 30);
    CLIP_VEC(dice->sea.vs_gnd, 5, 0, 30);
 
    dice->total[0] = CLIP(get_dice_air(army), 0, 30);
    dice->total[1] = CLIP(get_dice_ground(army), 0, 30);
}

void get_dice_for_armies(const Army* army_a, Dice* dice_a, 
                         const Army* army_b, Dice* dice_b, 
                         bool with_batch_cap){
    get_dice_for_army(army_a, dice_a);
    get_dice_for_army(army_b, dice_b);
    apply_batch_cap(army_a, army_b, (int*)&dice_a->total[1], (int*)&dice_b->total[1]);
}