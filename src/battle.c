#include "battle.h"


static void battle_air(ArmyHP* restrict ahpa, ArmyHP* restrict ahpb, uint32_t hits_a[6], uint32_t hits_b[6]){
    // priorize air units with ground assignment first 
    vec_sub_to_0_hp(ahpa->air.hp_stance_off, hits_b, 4);
    vec_sub_to_0_hp(ahpb->air.hp_stance_off, hits_a, 4);

    vec_sub_to_0_hp(ahpa->air.hp_stance_def, hits_b, 4);
    vec_sub_to_0_hp(ahpb->air.hp_stance_def, hits_a, 4);

    //black and white results
    int priority[2] = {4,3};
    int offset[2] = {4,5};
    for (int bw=0; bw<2; bw++){
        int off = offset[bw];
        for (int i=0; i<2; i++){
            vec_sub_to_0_hp(ahpa->air.hp_stance_off+priority[i], hits_b+off, 1);
            vec_sub_to_0_hp(ahpb->air.hp_stance_off+priority[i], hits_a+off, 1);

            vec_sub_to_0_hp(ahpa->air.hp_stance_def+priority[i], hits_b+off, 1);
            vec_sub_to_0_hp(ahpb->air.hp_stance_def+priority[i], hits_a+off, 1);
        }
    }
}


static void battle_lnd(ArmyHP* restrict ahpa, ArmyHP* restrict ahpb, uint32_t hits_a[6], uint32_t hits_b[6], bool fa[2]){
    // ############################################
    // lnd first
    vec_sub_to_0_hp(ahpa->lnd.hp_stance_off, hits_b, 3);
    vec_sub_to_0_hp(ahpb->lnd.hp_stance_off, hits_a, 3);

    vec_sub_to_0_hp(ahpa->lnd.hp_stance_def, hits_b, 3);
    vec_sub_to_0_hp(ahpb->lnd.hp_stance_def, hits_a, 3);

    //black and white results
    // priority tanks -> Art -> Inf
    int priority[3] = {3,2,1};
    int offset[2] = {4,5};
    for (int bw=0; bw<2; bw++){
        int off = offset[bw];
        for (int i=0; i<2; i++){
            if (fa[1]){
                vec_sub_to_0_hp(ahpa->lnd.hp_stance_off+priority[i], hits_b+off, 1);
                vec_sub_to_0_hp(ahpa->lnd.hp_stance_def+priority[i], hits_b+off, 1);
            }

            if (fa[0]){
                vec_sub_to_0_hp(ahpb->lnd.hp_stance_off+priority[i], hits_a+off, 1);
                vec_sub_to_0_hp(ahpb->lnd.hp_stance_def+priority[i], hits_a+off, 1);
            }
        }
    }
}



static void apply_dice_sea(ArmyHP* ahp, uint32_t hits[6], bool has_fa){
    // ############################################
    // subs and escord direct hits
    vec_sub_to_0_hp(ahp->sea.hp_stance_off, hits, 2);
    vec_sub_to_0_hp(ahp->sea.hp_stance_def, hits, 2);
    bool has_escort = false;
    if (ahp->sea.hp_stance_def[1] > 0)
        has_escort = true;
 
    if (has_escort){
        //damage ships to one hp then apply rest to escort then rest to original target
        vec_sub_to_1_hp(ahp->sea.hp_stance_off+2, hits+2, 1, sea_hp_stance_off[2]);
        vec_sub_to_1_hp(ahp->sea.hp_stance_def+2, hits+2, 1, sea_hp_stance_def[2]);

        vec_sub_to_1_hp(ahp->sea.hp_stance_off+3, hits+3, 1, sea_hp_stance_off[3]);
        vec_sub_to_1_hp(ahp->sea.hp_stance_def+3, hits+3, 1, sea_hp_stance_def[3]);

        // apply to escort for cv and battleship
        vec_sub_to_0_hp(ahp->sea.hp_stance_def+1, hits+2, 1);
        vec_sub_to_0_hp(ahp->sea.hp_stance_def+1, hits+3, 1);
    }
    //apply rest
    vec_sub_to_0_hp(ahp->sea.hp_stance_off+2, hits+2, 2);
    vec_sub_to_0_hp(ahp->sea.hp_stance_def+2, hits+2, 2);

    if (has_fa){
        //black dice
        int priority[2][4] = {{4,3,1,2},{4,3,2}};
        int p_size[2] = {4,3};
        int dice_offset[2] = {4,5};
        
        for (int wb=0; wb<2; wb++){
            int offset = dice_offset[wb];
            if (has_escort){
                for (int i=0; i<2; i++){
                    int p = priority[wb][i];
                    vec_sub_to_1_hp(ahp->sea.hp_stance_off+p, hits+offset, 1, sea_hp_stance_off[p]);
                    vec_sub_to_1_hp(ahp->sea.hp_stance_def+p, hits+offset, 1, sea_hp_stance_def[p]);
                }
                // apply to escort for cv and battleship
                vec_sub_to_0_hp(ahp->sea.hp_stance_def+1, hits+offset, 1);
            }
            //apply rest
            for (int i=0; i<p_size[wb]; i++){
                int p = priority[wb][i];
                vec_sub_to_0_hp(ahp->sea.hp_stance_def+p, hits+offset, 1);
            }
        }
    }
}

static void battle_sea(ArmyHP* restrict ahpa, ArmyHP* restrict ahpb, uint32_t hits_a[6], uint32_t hits_b[6], bool fa[2]){
    apply_dice_sea(ahpa, hits_b, fa[1]);
    apply_dice_sea(ahpb, hits_a, fa[0]);
}


static void apply_batch_cap(const Army* restrict aa, const Army* restrict ab, int* dice_a, int* dice_b){
    int unit_types_a = vec_n_components(aa->n_units_lnd, 3);
    int unit_types_b = vec_n_components(ab->n_units_lnd, 3);
    switch(unit_types_a-unit_types_b){
        case 1:
            *dice_b = MIN(*dice_b, 20);
            break;
        case 2:
            *dice_b = MIN(*dice_b, 10);
            break;
        case -1:
            *dice_a = MIN(*dice_a, 20);
            break;
        case -2:
            *dice_a = MIN(*dice_a, 10);
            break;
    }
}

void simulate_battle(const Army* restrict army_a, 
                     const Army* restrict army_b, 
                     SimStats* restrict   stats,
                     bool                 with_force_advantage,
                     bool                 with_batch_cap)
{   
    Army aa = *army_a;
    Army ab = *army_b;
    ArmyHP ahpa = {};
    ArmyHP ahpb = {};
    army_to_hp(army_a, &ahpa);
    army_to_hp(army_b, &ahpb);
    int escaped_subs[2] = {0,0};

    int n_dice_army_a_air = CLIP(get_dice_air(&aa), 0, 30);
    int n_dice_army_b_air = CLIP(get_dice_air(&ab), 0, 30);

    // air combat
    for (int round = 0; round<3; round++){
        uint32_t hits_a[6] = {}; 
        uint32_t hits_b[6] = {};
        int dice_used_in_batch_a = CLIP(n_dice_army_a_air, 0, 10);
        int dice_used_in_batch_b = CLIP(n_dice_army_b_air, 0, 10);
        roll_batch(dice_used_in_batch_a, hits_a);
        roll_batch(dice_used_in_batch_b, hits_b);

        n_dice_army_a_air -= dice_used_in_batch_a;
        n_dice_army_b_air -= dice_used_in_batch_b;
        battle_air(&ahpa, &ahpb, hits_a, hits_b);
    }

    update_unit_count(&aa, &ahpa);
    update_unit_count(&ab, &ahpb);
    
    int n_dice_army_a_ground = CLIP(get_dice_ground(&aa), 0, 30);
    int n_dice_army_b_ground = CLIP(get_dice_ground(&ab), 0, 30);
    if (with_batch_cap){
        apply_batch_cap(&aa, &ab, &n_dice_army_a_ground, &n_dice_army_b_ground);
    }

    bool fa[2] = {false, false};
    update_force_advantage(&aa, &ab, fa);

    // ground combat
    for (int round = 0; round<3; round++){
        uint32_t hits_a[6] = {}; 
        uint32_t hits_b[6] = {};
        int dice_used_in_batch_a = CLIP(n_dice_army_a_ground, 0, 10);
        int dice_used_in_batch_b = CLIP(n_dice_army_b_ground, 0, 10);
        roll_batch(dice_used_in_batch_a, hits_a);
        roll_batch(dice_used_in_batch_b, hits_b);

        n_dice_army_a_ground -= dice_used_in_batch_a;
        n_dice_army_b_ground -= dice_used_in_batch_b;

        battle_lnd(&ahpa, &ahpb, hits_a, hits_b, fa);
        battle_sea(&ahpa, &ahpb, hits_a, hits_b, fa);

        //after one batch each submarine still on 1 hp escapes!
        update_escaped_subs(&ahpa, &escaped_subs[0]);
        update_escaped_subs(&ahpb, &escaped_subs[1]);
    }


    //update unit count 
    update_unit_count(&aa, &ahpa);
    update_unit_count(&ab, &ahpb);
    add_escaped_subs(&aa, &escaped_subs[0]);
    add_escaped_subs(&ab, &escaped_subs[1]);

    udpate_statistics(&aa, &ab, stats);
    
}

void update_force_advantage(Army* restrict aa, Army* restrict ab, bool fa[2]){
    int unit_types_a = vec_n_components(aa->n_units_lnd, 3);
    int unit_types_b = vec_n_components(ab->n_units_lnd, 3);
    if (unit_types_a > unit_types_b){
        fa[0] = true;
    }else if (unit_types_b > unit_types_a){
        fa[1] = true;
    }
    else if (unit_types_a == unit_types_b){
        fa[0] = true;
        fa[1] = true;
    }
}