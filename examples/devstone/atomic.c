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

static long NUM_DELT_INTS = 0;
static long NUM_DELT_EXTS = 0;
static long NUM_EVENT_EXT = 0;

void dsatomic_init(atomic *self)
{
  dsatomic_state *s = self->state.user_data;
  s->event = (long *)malloc(sizeof(long));
  *(s->event) = 0;
  passivate(self);
}

void dsatomic_lambda(atomic *self)
{
  dsatomic_state *s = self->state.user_data;
  devs_message_push_back(&(self->output), DSATOMIC_OUT, s->event);
}

void dsatomic_deltint(atomic *self)
{
  NUM_DELT_INTS++;
  dsatomic_state *s = self->state.user_data;
  if (s->int_delay_time > 0)
  {
    dhrystone(s->int_delay_time);
  }
  passivate(self);
}

void dsatomic_deltext(atomic *self, const double e)
{
  resume(self, e);
  NUM_DELT_EXTS++;
  dsatomic_state *s = self->state.user_data;
  if (s->ext_delay_time > 0)
    dhrystone(s->ext_delay_time);
  NUM_EVENT_EXT += devs_message_size(&(self->input));
  hold_in(self, "active", s->preparation_time);
}

void dsatomic_exit(atomic *self)
{
  dsatomic_state *s = self->state.user_data;
  free(s->event);
  return;
}

atomic *dsatomic_new(double preparation_time, double int_delay_time, double ext_delay_time)
{
  atomic *dsatomic = atomic_new();
  dsatomic_state *data = (dsatomic_state *)malloc(sizeof(dsatomic_state));
  data->preparation_time = preparation_time;
  data->int_delay_time = int_delay_time;
  data->ext_delay_time = ext_delay_time;
  dsatomic->state.user_data = data;
  dsatomic->initialize = dsatomic_init;
  dsatomic->ta = ta_default;
  dsatomic->lambda = dsatomic_lambda;
  dsatomic->deltint = dsatomic_deltint;
  dsatomic->deltext = dsatomic_deltext;
  dsatomic->deltcon = deltcon_default;
  dsatomic->exit = dsatomic_exit;
  return dsatomic;
}
