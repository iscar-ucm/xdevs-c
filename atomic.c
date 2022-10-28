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

double ta_default(const atomic* self) {
  return self->state->sigma;
}

void deltext_default(atomic* self, const double e, const devs_message* msg) {
  return;
}

void deltcon_default(atomic* self, const double e, const devs_message* msg) {
  self->deltint(self);
  self->deltext(self, 0, msg);
}

void resume(atomic* self, const double e) {
  self->state->sigma -= e;
  return;
}

void activate(atomic* self) {
  self->state->sigma = 0.0;
  strncpy(self->state->phase, PHASE_ACTIVE, strlen(PHASE_ACTIVE));
  return;
}

void passivate(atomic* self) {
  self->state->sigma = INFINITY;
  strncpy(self->state->phase, PHASE_PASSIVE, strlen(PHASE_PASSIVE));
  return;
}

void hold_in(atomic* self, const double sigma, const char* phase) {
  self->state->sigma = sigma;
  strncpy(self->state->phase, phase, strlen(phase));
  return;
}
