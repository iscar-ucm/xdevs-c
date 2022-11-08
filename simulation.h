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

#include "modeling.h"

typedef struct st_simulator {
  atomic* model;
  double tL, tN;
} simulator;

simulator* simulator_new(atomic* model);
void simulator_delete(simulator* sim);
void simulator_initialize(simulator* sim, atomic* model);
void simulator_exit (simulator* sim);
double simulator_ta(const simulator* sim);
devs_message* simulator_lambda(const simulator* sim);
void simulator_deltfcn(simulator* sim, const devs_message* msg);
// void simulator_clear(simulator *sim);

typedef struct st_coordinator {
  coupled* model;
  list* simulators;
  double tL, tN;
} coordinator;

coordinator* coordinator_new(const coupled* model);
void coordinator_delete(coordinator* sim);
void coordinator_initialize(coordinator* sim, coupled* model);
void coordinator_exit (coordinator* sim);
double coordinator_ta(const coordinator* sim);
devs_message* coordinator_lambda(const coordinator* sim);
void coordinator_deltfcn(coordinator* sim, const devs_message* msg);

#endif /* SIMULATION_H */
