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
#include "simulator.h"

simulator* simulator_new() {
  simulator* sim = (simulator*)malloc(sizeof(simulator));
  return sim;
}

void simulator_delete(simulator* sim) {
  sim->model->atomic_delete(sim->model);
  free(sim);
  return;
}

void simulator_initialize(simulator* sim, const double clock) {
  *tL = 0;
  *tN = *tL + model->ta(model->state);
}

double simulator_ta(const struct atomic* model) {
  return model->ta(model->state);
}
