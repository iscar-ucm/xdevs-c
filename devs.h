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

typedef struct st_devs_state
{
  double sigma;
  char phase[MAX_PHASE_LENGTH];
  void *user_data;
} devs_state;

typedef struct st_devs_node
{
  unsigned int port_id;
  void *value;
  struct st_devs_node *prev;
  struct st_devs_node *next;
} devs_node;

typedef struct st_devs_message
{
  struct st_devs_node *head;
  struct st_devs_node *tail;
} devs_message;

typedef struct st_list_node
{
  void *data;
  struct st_list_node *prev;
  struct st_list_node *next;
} list_node;

typedef struct st_list
{
  list_node *head;
  list_node *tail;
} list;

/// @brief Generates a new DEVS message.
/// @return A pointer to the new message.
devs_message *devs_message_new();

/// @brief Checks if a DEVS message is empty.
/// @param msg DEVS message to check.
/// @return A boolean value indicating if the message is empty.
bool devs_message_is_empty(const devs_message *msg);

/// @brief Inserts a new event in a DEVS message.
/// @param msg DEVS message to insert the event.
/// @param port_id Port identifier where the message is going to be inserted.
/// @param value Value associated to the event.
/// @return The new DEVS message.
devs_message *devs_message_push_back(devs_message *msg, int port_id, void *value);

/// @brief From a given DEVS message, returns all the values/events associated to a given port.
/// @param msg DEVS message from which we want to extract the events.
/// @param port_id Input port.
/// @return All the events associated to the given port, as a linked list.
list* devs_message_get_values(const devs_message* msg, unsigned int port_id);

/// @brief Get the first value from a given DEVS message and input port.
/// @param msg The DEVS message.
/// @param port_id The input port idenfitier.
/// @return The first value of the DEVS message.
void* devs_message_get_value(const devs_message* msg, unsigned int port_id);

list *list_new();
void list_delete(list *l);
bool list_is_empty(const list *l);
void list_push_back(list *l, void *data);

#endif
