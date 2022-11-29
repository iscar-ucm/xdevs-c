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
  s->event = (long *)malloc(sizeof(long));
  *(s->event) = 1;
  hold_in(self, "active", s->preparation_time);
}

void generator_lambda(atomic *self)
{
  generator_state *s = self->state.user_data;
  devs_message_push_back(&(self->output), GENERATOR_OUT, s->event);
}

void generator_deltint(atomic *self)
{
  generator_state *s = self->state.user_data;
  (*(s->event))++;
  if (*(s->event) > s->max_events)
    passivate(self);
  else
    hold_in(self, "active", s->period);
}

void generator_deltext(atomic *self, const double e)
{
  resume(self, e);
  passivate(self);
}

void generator_exit(atomic *self)
{
  generator_state *s = self->state.user_data;
  free(s->event);
  return;
}

atomic *generator_new(double preparation_time, double period, long max_events)
{
  atomic *generator = atomic_new();
  generator_state *data = (generator_state *)malloc(sizeof(generator_state));
  data->preparation_time = preparation_time;
  data->period = period;
  data->max_events = max_events;
  generator->state.user_data = data;
  generator->initialize = generator_init;
  generator->ta = ta_default;
  generator->lambda = generator_lambda;
  generator->deltint = generator_deltint;
  generator->deltext = generator_deltext;
  generator->deltcon = deltcon_default;
  generator->exit = generator_exit;
  return generator;
}
