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
#include "atomic.h"

double ta_default(const devs_state *s) {
  return s->sigma;
}

devs_state* deltext_default(devs_state* state, const double e, const devs_message* msg) {
  return NULL;
}

devs_state* resume(devs_state* state, const double e) {
  state->sigma -= e;
  return state;
}

devs_state* activate(devs_state* state) {
  state->sigma = 0.0;
  strncpy(state->phase, PHASE_ACTIVE, strlen(PHASE_ACTIVE));
  return state;
}

devs_state* passivate(devs_state* state) {
  state->sigma = INFINITY;
  strncpy(state->phase, PHASE_PASSIVE, strlen(PHASE_PASSIVE));
  return state;
}

devs_state* hold_in(devs_state* state, double sigma, const char* phase) {
  state->sigma = sigma;
  strncpy(state->phase, phase, strlen(phase));
  return state;
}
