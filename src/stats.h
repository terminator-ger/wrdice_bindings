#ifndef STATS_H
#define STATS_H
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "wrdice.h"

void add_escaped_subs(Army *restrict army, const int *restrict esc_subs);
void update_escaped_subs(ArmyHP *restrict ahp, int *restrict escaped_subs);
void udpate_statistics(const Army *restrict army_a, const Army *restrict army_b, SimStats *stats);
void update_max_size(Survived *restrict surv, uint32_t count);
void update_unit_count(Army *restrict army, const ArmyHP *restrict hp);

#endif
