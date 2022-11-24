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
#include "transducer.h"
#include "../../core/devs.h"
#include "job.h"

void transducer_init(atomic *self) {
  transducer_state *s = self->state.user_data;
  hold_in(self, "active", s->obs_time);
  return;
}

void transducer_exit(atomic *self) {
  transducer_state *s = self->state.user_data;
  list_node* n = s->jobs_arrived->head;
  while(n!=NULL) {
    free(n->data);
    n = n->next;
  }
  list_delete(s->jobs_arrived);
  list_delete(s->jobs_solved);
  return;
}

void transducer_lambda(atomic *self) {
  devs_message_push_back(&(self->output), TRANSDUCER_OUT, job_new(-1, -1));
  return;
}

void transducer_deltint(atomic *self) {
  transducer_state *s = self->state.user_data;
  s->clock += self->state.sigma;
  if(phase_is(self, "active")) {
    double avg_ta_time = 0.0, throughput = 0.0;
    if(!list_is_empty(s->jobs_solved)) {
      avg_ta_time = s->total_ta / list_size(s->jobs_solved);
      throughput = (s->clock > 0) ? list_size(s->jobs_solved)/s->clock : 0.0;      
    }
    printf("End time: %f\n", s->clock);
    printf("Jobs arrived: %d\n", list_size(s->jobs_arrived));
    printf("Jobs solved: %d\n", list_size(s->jobs_solved));
    printf("Average TA = %f\n", avg_ta_time);
    printf("Throughput = %f\n", throughput);
    hold_in(self, "done", 0.0);
  }
  else {
    passivate(self);
  }
  return;
}

void transducer_deltext(atomic *self, const double e) {
  resume(self, e);
  transducer_state *s = self->state.user_data;
  s->clock += e;

  if (phase_is(self, "active")) {
    job* j;
    if ((j = devs_port_get_value(&(self->input), TRANSDUCER_ARRIVED))!=NULL) {
      printf("Start job %d, @ t = %f\n",  + j->id, s->clock);
      j->time = s->clock;
      list_push_back(s->jobs_arrived, j);
    }
    if ((j = devs_port_get_value(&(self->input), TRANSDUCER_SOLVED))!=NULL) {
      s->total_ta += (s->clock - j->time);
      printf("Finish job %d @ t = %f\n", j->id, s->clock);
      j->time = s->clock;
      list_push_back(s->jobs_solved, j);
    }
  }
}

atomic *transducer_new(double obs_time) {
  atomic *transducer = (atomic *)malloc(sizeof(atomic));
  transducer->component_type = DEVS_ATOMIC;
  transducer_state *data = (transducer_state *)malloc(sizeof(transducer_state));
  data->clock = 0.0;
  data->total_ta = 0.0;
  data->obs_time = obs_time;
  data->jobs_arrived = list_new();
  data->jobs_solved = list_new();
  transducer->state.user_data = data;
  transducer->initialize = transducer_init;
  transducer->ta = ta_default;
  transducer->lambda = transducer_lambda;
  transducer->deltint = transducer_deltint;
  transducer->deltext = transducer_deltext;
  transducer->deltcon = deltcon_default;
  transducer->exit = transducer_exit;
  return transducer;
}
