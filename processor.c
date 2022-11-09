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
#include "processor.h"

devs_message *lambda(const atomic *self) {
  devs_message *msg = devs_message_new();
  processor_state *s = self->state->user_data;
  devs_message_push_back(msg, PORT_OUT, clone_job(s->current_job));
  return msg;
}

void deltint(atomic *self) {
  processor_state *s = self->state->user_data;
  // Free current job:
  if (s->current_job != NULL) {
    free(s->current_job);
    s->current_job = NULL;
  }
  // Update clock:
  s->clock += self->state->sigma;
  // Passivate the current model:
  passivate(self);
  return;
}

void deltext(atomic *self, const double e, const devs_message *msg) {
  resume(self, e);
  processor_state *s = self->state->user_data;
  // Update clock:
  s->clock += e;
  // If the model is available, then take the job:
  if (s->current_job == NULL) {
    s->current_job = (job *)devs_message_get_value(msg, PORT_IN);
    s->current_job->time = s->clock;
    hold_in(self, s->processing_time, "active");
  }    
}

void initialize(atomic *self) {
  activate(self);
  return;
}

atomic *processor_new(double period) {
  atomic *processor = (atomic *)malloc(sizeof(atomic));
  processor->component_type = DEVS_ATOMIC;
  processor_state *data = (processor_state *)malloc(sizeof(processor_state));
  data->current_job = NULL;
  processor->state = (devs_state *)malloc(sizeof(devs_state));
  processor->state->user_data = data;
  processor->ta = ta_default;
  processor->lambda = lambda;
  processor->deltint = deltint;
  processor->deltext = deltext;
  processor->deltcon = deltcon_default;
  return processor;
}
