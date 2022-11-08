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
#include "generator.h"

devs_message* lambda(const atomic* self) {
  devs_message* msg = devs_message_new();
  job* j = (job*)malloc(sizeof(job));
  generator_state* s = self->state->user_data;
  j->id = s->job_next_id;
  devs_message_push_back(msg, PORT_OUT, j);
  return msg;
}

void deltint(atomic* self) {
  generator_state* s = self->state->user_data;
  s->job_next_id++;
  return;
}

void deltext(atomic *self, const double e, const devs_message *msg) {
  passivate(self);
}

void initialize(atomic* self) {
  activate(self);
  return;
}

atomic* generator_new(double period) {
  atomic* generator = (atomic*)malloc(sizeof(atomic));
  generator->component_type = DEVS_ATOMIC;
  generator_state* data = (generator_state*)malloc(sizeof(generator_state));
  data->period = period;
  data->job_next_id = 1;
  generator->state = (devs_state*)malloc(sizeof(devs_state));
  generator->state->user_data = data;
  generator->ta = ta_default;
  generator->lambda = lambda;
  generator->deltint = deltint;
  generator->deltext = deltext;
  generator->deltcon = deltcon_default;
  return generator;
}


