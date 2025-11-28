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


int get_dice_air(const Army* army){
    int n_dice = 0;
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


int get_dice_ground(const Army* army){
    int n_dice = 0;
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
    dice->total[0] = (uint32_t) get_dice_air(army);
    dice->total[1] = (uint32_t) get_dice_ground(army);
}