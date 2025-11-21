#include "battle.h"

void print_army(const Army* army){
    printf("\n");
    printf("Army:\n");
    printf("Land:\n");
    printf("    INF: %i", army->n_units_lnd[0]);
    printf("    ART: %i", army->n_units_lnd[1]);
    printf("    ARM: %i", army->n_units_lnd[2]);
    printf("\n");
    printf("AIR:\n");
    printf("    FGT: %i", army->n_units_air[2]);
    printf("    BOM: %i", army->n_units_air[3]);
    printf("\n");
    printf("SEA:\n");
    printf("    SUB: %i", army->n_units_sea[0]);
    printf("    CRS: %i", army->n_units_sea[1]);
    printf("    CV : %i", army->n_units_sea[2]);
    printf("    BS : %i", army->n_units_sea[3]);
    printf("\n");
}

void print_stats(const SimStats* ss){
    printf("Army A:\n");
    printf("Land:\n");
    char *lbl[] = {"INF", "ART", "ARM"};
    for (int tvpe = 0; tvpe < 3; tvpe++){
        printf("%s\n", lbl[tvpe]);
        for (int i = 0; i < 10; i++){
            printf("%i -> %.2f \n", i, ss->army_a.stats_lnd[tvpe].odds[i]);
        }
    }
    printf("\n");
    printf("Army B:\n");
    printf("Land:\n");
    for (int tvpe = 0; tvpe < 3; tvpe++){
        printf("%s\n", lbl[tvpe]);
        for (int i = 0; i < 10; i++){
            printf("%i -> %.2f \n", i, ss->army_b.stats_lnd[tvpe].odds[i]);
        }
    }
}


void run_simulation(const Army* restrict army_a, 
                    const Army* restrict army_b, 
                    SimStats*   restrict stats,
                    bool                 with_force_advantage,
                    bool                 with_batch_cap){  
    int N = 5000; 

    print_army(army_a);
    print_army(army_b);

    for (int i=0; i<N; i++){
        simulate_battle(army_a, army_b, stats, with_force_advantage, with_batch_cap);
    }

    update_stats_counts(stats->army_a.stats_air, N);
    update_stats_counts(stats->army_a.stats_lnd, N);
    update_stats_counts(stats->army_a.stats_sea, N);

    update_stats_counts(stats->army_b.stats_air, N);
    update_stats_counts(stats->army_b.stats_lnd, N);
    update_stats_counts(stats->army_b.stats_sea, N);
    stats->br.win_a /= N;
    stats->br.win_b /= N;
    stats->br.draw  /= N;
    stats->br.death /= N;

    print_stats(stats);
}
