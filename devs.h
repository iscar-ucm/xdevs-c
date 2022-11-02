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

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define MAX_PHASE_LENGTH 24
#define PHASE_PASSIVE "PASSIVE"
#define PHASE_ACTIVE "ACTIVE"

typedef struct st_devs_state {
  double sigma;
  char phase[MAX_PHASE_LENGTH];
  void* user_data;
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

devs_message *devs_message_new();
bool devs_message_is_empty(const devs_message *msg);
devs_message *devs_message_push_back(devs_message *msg, int port_id,
                                     void *value);

typedef struct st_list_node {
  void* data;
  struct st_list_node* prev;
  struct st_list_node* next;
} list_node;

typedef struct st_list {
  list_node* head;
  list_node* tail;
} list;

list *list_new();
void list_delete(list* l);
bool list_is_empty(const list* l);
void list_push_back(list* l, void* data);

#endif

