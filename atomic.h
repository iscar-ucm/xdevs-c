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
#ifndef ATOMIC_H
#define ATOMIC_H

#include "devs.h"

double ta_default(const devs_state *s);
devs_state *deltext_default(devs_state *state, const double e,
                            const devs_message *msg);

typedef struct st_atomic {
  devs_state *state;
  struct st_atomic* (*atomic_new) (void* user_data);
  double (*ta) (const devs_state* state);
  devs_message* (*lambda) (const devs_state* state);
  devs_state* (*deltint) (devs_state* state);
  devs_state* (*deltext) (devs_state* state, const double e, const devs_message* msg);
  devs_state* (*deltcon) (devs_state* state, const double e, const devs_message* msg);
} atomic;

devs_state *resume(devs_state *state, const double e);
devs_state* activate(devs_state *state);
devs_state *passivate(devs_state *state);
devs_state *hold_in(devs_state *state, double sigma, const char *phase);

#endif /* ATOMIC_H */
