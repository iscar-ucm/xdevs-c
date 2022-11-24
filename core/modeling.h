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
#ifndef MODELING_H
#define MODELING_H

#include "devs.h"

#define DEVS_ATOMIC 1
#define DEVS_COUPLED 2
#define DEVS_IS_ATOMIC(component_type) (component_type == DEVS_ATOMIC) ? true : false;
#define DEVS_IS_COUPLED(component_type) (component_type == DEVS_COUPLED) ? true : false;

typedef struct st_atomic
{
  int component_type;
  devs_message input;
  devs_message output;
  devs_state state;
  void (*initialize)(struct st_atomic *self);
  double (*ta)(const struct st_atomic *self);
  void (*lambda)(struct st_atomic *self);
  void (*deltint)(struct st_atomic *self);
  void (*deltext)(struct st_atomic *self, const double e);
  void (*deltcon)(struct st_atomic *self, const double e);
  void (*exit)(const struct st_atomic *self);
} atomic;

typedef struct st_coupled
{
  int component_type;
  devs_message input;
  devs_message output;
  list components;
  list ic;
  list eic;
  list eoc;
} coupled;

typedef struct st_coupling
{
  void *component_from;
  unsigned int port_from;
  void *component_to;
  unsigned int port_to;
} coupling;

double ta_default(const atomic *self);
void deltext_default(atomic *self, const double e);
void deltcon_default(atomic *self, const double e);
void exit_default(const atomic *self);
void resume(atomic *self, const double e);
void activate(atomic *self);
void passivate(atomic *self);
void hold_in(atomic *self, const char *phase, const double sigma);
bool phase_is(atomic *self, const char *phase);

coupled* coupled_new();
void add_coupling(coupled *self, void *component_from, unsigned int port_from, void *component_to, unsigned int port_to);

#endif /* MODELING_H */
