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


typedef struct st_atomic {
  devs_state *state;
  void (*atomic_initialize) (struct st_atomic* self);
  void (*atomic_exit) (const struct st_atomic* self);
  double (*ta) (const struct st_atomic* self);
  devs_message* (*lambda) (const struct st_atomic* self);
  void (*deltint) (struct st_atomic* self);
  void (*deltext) (struct st_atomic* self, const double e, const devs_message* msg);
  void (*deltcon) (struct st_atomic* self, const double e, const devs_message* msg);
} atomic;

double ta_default(const atomic* self);
void deltext_default(atomic *self, const double e, const devs_message *msg);
void deltcon_default(atomic *self, const double e, const devs_message *msg);
void resume(atomic* self, const double e);
void activate(atomic* self);
void passivate(atomic* self);
void hold_in(atomic *self, const double sigma, const char *phase);

#endif /* ATOMIC_H */
