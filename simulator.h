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
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "atomic.h"

typedef struct st_simulator {
  atomic* model;
  double clock, tL, tN;
} simulator;

simulator* simulator_new();
void simulator_delete(simulator* sim);
void simulator_initialize(simulator* sim, const atomic* model, const double clock);
void simulator_exit ();
double simulator_ta(simulator* sim);
devs_message* simulator_lambda(const simulator* sim);
void simulator_deltfcn(simulator* sim);
void simulator_clear(simulator* sim);

#endif /* SIMULATOR_H */
