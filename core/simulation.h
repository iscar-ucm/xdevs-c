/*
 * Copyright (C) 2022 José Luis Risco Martín <jlrisco@ucm.es>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contributors:
 *  - José Luis Risco Martín
 */
#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdio.h>
#include "modeling.h"

typedef struct st_simulator {
  atomic* m;
  double tL, tN;
} simulator;

simulator* simulator_new(atomic* m);
void simulator_delete(simulator* s);
void simulator_initialize(simulator* s);
void simulator_exit (simulator* s);
double simulator_ta(const simulator* s);
void simulator_lambda(simulator* s, double t);
void simulator_deltfcn(simulator* s, double t);
void simulator_clear(simulator *s);

typedef struct st_coordinator {
  coupled* m;
  list simulators;
  double tL, tN;
} coordinator;

coordinator* coordinator_new(coupled* m);
void coordinator_delete(coordinator* c);
void coordinator_initialize(coordinator* c);
double coordinator_ta(const coordinator* c);
void coordinator_simulate(coordinator* c, unsigned long nsteps);
void coordinator_exit (coordinator* c);

void coordinator_propagate_input(coordinator* c);
void coordinator_propagate_output(coordinator* c);
void coordinator_clear(coordinator* c);

#endif /* SIMULATION_H */
