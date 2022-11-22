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
#include "modeling.h"
#include <string.h>

double ta_default(const atomic *self) { return self->state.sigma; }

void deltext_default(atomic *self, const double e) { return; }

void deltcon_default(atomic* self, const double e) {
  self->deltint(self);
  self->deltext(self, 0);
}

void exit_default(const atomic *self) { return; }

void resume(atomic* self, const double e) {
  self->state.sigma -= e;
  return;
}

void activate(atomic* self) {
  self->state.sigma = 0.0;
  strncpy(self->state.phase, PHASE_ACTIVE, strlen(PHASE_ACTIVE));
  return;
}

void passivate(atomic* self) {
  self->state.sigma = INFINITY;
  strncpy(self->state.phase, PHASE_PASSIVE, strlen(PHASE_PASSIVE));
  return;
}

void hold_in(atomic* self, const char* phase, const double sigma) {
  self->state.sigma = sigma;
  strncpy(self->state.phase, phase, strlen(phase));
  return;
}

bool phase_is(atomic *self, const char *phase) {
  int res = strcmp(self->state.phase, phase);
  return res == 0;
}

void add_coupling(coupled* self, void *component_from, unsigned int port_from, void *component_to, unsigned int port_to) {
  coupling* c = (coupling*)malloc(sizeof(coupling));
  c->component_from = component_from;
  c->port_from = port_from;
  c->component_to = component_to;
  c->port_to = port_to;
  if (component_from == self) {
    list_push_back(&(self->eic), c);
  } else if (component_to == self) {
    list_push_back(&(self->eoc), c);
  } else {
    list_push_back(&(self->ic), c);
  }
}

void coupling_propagate_values(coupling *c) {
  atomic* from = (atomic*)c->component_from;
  devs_message message_from = from->output;
  atomic* to = (atomic*)c->component_to;
  devs_message message_to = to->output;
  // TODO: Continue here!!!
}