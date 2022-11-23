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
  strncpy(self->state.phase, PHASE_ACTIVE, strlen(PHASE_ACTIVE)+1);
  return;
}

void passivate(atomic* self) {
  self->state.sigma = INFINITY;
  strncpy(self->state.phase, PHASE_PASSIVE, strlen(PHASE_PASSIVE)+1);
  return;
}

void hold_in(atomic* self, const char* phase, const double sigma) {
  self->state.sigma = sigma;
  strncpy(self->state.phase, phase, strlen(phase)+1);
  return;
}

bool phase_is(atomic *self, const char *phase) {
  int res = strcmp(self->state.phase, phase);
  return res == 0;
}

coupled* coupled_new() {
  coupled *m = (coupled*) malloc(sizeof(coupled));
  m->component_type = DEVS_COUPLED;
  m->input.head = NULL;
  m->input.tail = NULL;
  m->output.head = NULL;
  m->output.tail = NULL;
  m->components.size = 0;
  m->components.head = NULL;
  m->components.tail = NULL;
  m->ic.size = 0;
  m->ic.head = NULL;
  m->ic.tail = NULL;
  m->eic.size = 0;
  m->eic.head = NULL;
  m->eic.tail = NULL;
  m->eoc.size = 0;
  m->eoc.head = NULL;
  m->eoc.tail = NULL;
  return m;
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
  atomic* a_from = (atomic*)c->component_from;
  int p_from = c->port_from;
  devs_message message_from = a_from->output;
  atomic* a_to = (atomic*)c->component_to;
  int p_to = c->port_to;
  devs_message message_to = a_to->output;
  devs_node* n = message_from.head;
  while(n!=NULL) {
    if (n->port_id == p_from) {
      devs_message_push_back(&(message_to), p_to, n->value);
    }
    n = n->next;
  }
}
