
#include "wrdice.h"


int main(){
    Army army_a = {
        .n_units_lnd={10,4,4}, 
        .stance_lnd.stance_off={10,4,4},
        .n_units_air={0,0,4},
        .stance_air.stance_def={0,0,4}
    };

    Army army_b = {
        .n_units_lnd={5,2,3},
        .stance_lnd.stance_def={5,2,3},
        .n_units_air={0,0,4},
        .stance_air.stance_off={0,0,4}
    };
    SimStats stats = {};

    run_simulation(&army_a, &army_b, &stats, true, true);
}