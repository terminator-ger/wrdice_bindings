#include "battle.h"

void update_unit_count(Army *restrict army, const ArmyHP *restrict hp)
{
    for (int tvpe = 0; tvpe < 5; tvpe++)
    {
        army->n_units_air[tvpe] = div_ceil(hp->air.hp_stance_off[tvpe], air_hp_stance_off[tvpe]);
        army->n_units_air[tvpe] += div_ceil(hp->air.hp_stance_def[tvpe], air_hp_stance_def[tvpe]);

        army->n_units_lnd[tvpe] = div_ceil(hp->lnd.hp_stance_off[tvpe], lnd_hp_stance_off[tvpe]);
        army->n_units_lnd[tvpe] += div_ceil(hp->lnd.hp_stance_def[tvpe], lnd_hp_stance_def[tvpe]);

        army->n_units_sea[tvpe] = div_ceil(hp->sea.hp_stance_off[tvpe], sea_hp_stance_off[tvpe]);
        army->n_units_sea[tvpe] += div_ceil(hp->sea.hp_stance_def[tvpe], sea_hp_stance_def[tvpe]);
    }
}

void update_max_size(Survived *restrict surv, uint32_t count)
{
    surv->size = MAX(surv->size, count + 1);
}

void udpate_statistics(const Army *restrict army_a, const Army *restrict army_b, SimStats *stats)
{
    for (int tvpe = 0; tvpe < 3; tvpe++)
    {
        // update unit count for lnd
        stats->army_a.stats_lnd[tvpe].count[army_a->n_units_lnd[tvpe]] += 1;
        stats->army_b.stats_lnd[tvpe].count[army_b->n_units_lnd[tvpe]] += 1;
        update_max_size(&stats->army_a.stats_lnd[tvpe], army_a->n_units_lnd[tvpe]);
        update_max_size(&stats->army_b.stats_lnd[tvpe], army_b->n_units_lnd[tvpe]);
    }
    for (int tvpe = 2; tvpe < 4; tvpe++)
    {
        // update unit count for air
        stats->army_a.stats_air[tvpe].count[army_a->n_units_air[tvpe]] += 1;
        stats->army_b.stats_air[tvpe].count[army_b->n_units_air[tvpe]] += 1;
        update_max_size(&stats->army_a.stats_air[tvpe], army_a->n_units_air[tvpe]);
        update_max_size(&stats->army_b.stats_air[tvpe], army_b->n_units_air[tvpe]);
    }
    for (int tvpe = 0; tvpe < 4; tvpe++)
    {
        // update unit count for sea
        stats->army_a.stats_sea[tvpe].count[army_a->n_units_sea[tvpe]] += 1;
        stats->army_b.stats_sea[tvpe].count[army_b->n_units_sea[tvpe]] += 1;
        update_max_size(&stats->army_a.stats_sea[tvpe], army_a->n_units_sea[tvpe]);
        update_max_size(&stats->army_b.stats_sea[tvpe], army_b->n_units_sea[tvpe]);
    }

    // Win - Loss Distribution
    bool surv_a_lnd = vec_n_components(army_a->n_units_lnd, 3) > 0 ? true : false;
    bool surv_b_lnd = vec_n_components(army_b->n_units_lnd, 3) > 0 ? true : false;

    bool surv_a_sea = vec_n_components(army_a->n_units_sea, 4) > 0 ? true : false;
    bool surv_b_sea = vec_n_components(army_b->n_units_sea, 4) > 0 ? true : false;

    bool surv_a = surv_a_lnd || surv_a_sea;
    bool surv_b = surv_b_lnd || surv_b_sea;

    if (surv_a && !surv_b)
    {
        stats->br.win_a += 1;
    }
    else if (!surv_a && surv_b)
    {
        stats->br.win_b += 1;
    }
    else if (surv_a && surv_b)
    {
        stats->br.draw += 1;
    }
    else if (!surv_a && !surv_b)
    {
        stats->br.death += 1;
    }
}

void update_escaped_subs(ArmyHP *restrict ahp, int *restrict escaped_subs)
{
    if (ahp->sea.hp_stance_def[0] % sea_hp_stance_def[0] == 1)
    {
        *escaped_subs += 1;
        ahp->sea.hp_stance_def[0] -= 1;
    }
    if (ahp->sea.hp_stance_off[0] % sea_hp_stance_off[0] == 1)
    {
        *escaped_subs += 1;
        ahp->sea.hp_stance_off[0] -= 1;
    }
}

void add_escaped_subs(Army *restrict army, const int *restrict esc_subs)
{
    army->n_units_sea[0] += *esc_subs;
    army->escaped_subs += *esc_subs;
}

void update_stats_counts(Survived stats[5], int N)
{
    for (int i = 0; i < 5; i++)
    {
        vec_div_scalar(stats[i].odds, stats[i].count, N, stats[i].size);
    }
}