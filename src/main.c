
#include "wrdice.h"
#include <stdio.h>

void test_dice()
{
    Army army_a = {
        .n_units_sea = {10, 4, 4},
        .stance_sea.stance_off = {10, 4, 4},
        .n_units_air = {0, 0, 4},
        .stance_air.stance_def = {0, 0, 4}};
    Dice d;

    get_dice_for_army(&army_a, &d);
    for (int i = 0; i < 5; i++)
    {
        printf("%i ", d.air.vs_air[i]);
    }
}

void run_sim()
{
    Army army_a = {
        .n_units_sea = {10, 4, 4, 0},
        .stance_sea.stance_off = {10, 4, 4, 0},
        .n_units_air = {0, 0, 4},
        .stance_air.stance_def = {0, 0, 4}};

    Army army_b = {
        .n_units_sea = {5, 2, 3, 1},
        .stance_sea.stance_def = {5, 2, 3, 1},
        .n_units_air = {0, 0, 4},
        .stance_air.stance_off = {0, 0, 4}};
    SimStats stats = {};

    run_simulation(&army_a, &army_b, &stats, true, true);
}

int main()
{
    run_sim();
}
