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
#ifndef _DEVS_H_
#define _DEVS_H_

#include <stdbool.h>

typedef struct st_devs_state {
  double sigma;
  const char* phase;
  void* user_state;
} devs_state;

typedef struct st_devs_node {
  unsigned int port_id;
  void* value;
  struct st_devs_node* prev;
  struct st_devs_node* next;
} devs_node;

typedef struct st_devs_message {
  struct st_devs_node* head;
  struct st_devs_node* tail;
} devs_message;

devs_message* new_devs_message();
bool is_empty_devs_message(const devs_message* msg);
devs_message* push_back_devs_message(devs_message* msg, int port_id, void* value);
devs_state* default_deltext(devs_state* state, const double e, const devs_message* msg);
double default_ta(const devs_state *s);

struct atomic_operations {
  double (*ta) (const devs_state*);
  devs_message* (*lambda) (const devs_state*);
  devs_state* (*deltint) (devs_state*);
  devs_state* (*deltext) (devs_state*, const double, const devs_message*);
  devs_state* (*deltcon) (devs_state*, const double, const devs_message*);
};

devs_state* passivate(devs_state* state);

#endif

