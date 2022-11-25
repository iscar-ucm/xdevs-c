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

void generator_init(atomic *self)
{
  generator_state *s = (generator_state *)self->state.user_data;
  s->job_next_id = 1;
  hold_in(self, "active", s->period);
}

void generator_lambda(atomic *self)
{
  job *j = (job *)malloc(sizeof(job));
  generator_state *s = self->state.user_data;
  j->id = s->job_next_id;
  devs_message_push_back(&(self->output), GENERATOR_OUT, j);
}

void generator_deltint(atomic *self)
{
  generator_state *s = self->state.user_data;
  s->job_next_id++;
  hold_in(self, "active", s->period);
}

void generator_deltext(atomic *self, const double e)
{
  resume(self, e);
  passivate(self);
}

atomic *generator_new(double period)
{
  atomic *generator = atomic_new();
  generator_state *data = (generator_state *)malloc(sizeof(generator_state));
  data->period = period;
  generator->state.user_data = data;
  generator->initialize = generator_init;
  generator->ta = ta_default;
  generator->lambda = generator_lambda;
  generator->deltint = generator_deltint;
  generator->deltext = generator_deltext;
  generator->deltcon = deltcon_default;
  generator->exit = exit_default;
  return generator;
}
