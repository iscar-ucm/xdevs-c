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
#include "devs.h"
#include <stdlib.h>
#include <stdbool.h>

devs_message *devs_message_new()
{
  devs_message *list = (devs_message *)malloc(sizeof(devs_message));
  list->head = NULL;
  list->tail = NULL;
  return list;
}

bool devs_message_is_empty(const devs_message *msg)
{
  return (msg == NULL) || (msg->head == NULL);
}

devs_message *devs_message_push_back(devs_message *msg, int port_id, void *value)
{
  devs_node *node = (devs_node *)malloc(sizeof(devs_node));
  node->port_id = port_id;
  node->value = value;
  node->prev = NULL;
  node->next = NULL;
  if (devs_message_is_empty(msg))
  {
    msg->head = msg->tail = node;
  }
  else
  {
    node->prev = msg->tail;
    msg->tail->next = node;
    msg->tail = node;
  }
  return msg;
}

bool devs_port_is_empty(const devs_message *msg, unsigned int port_id) {
  devs_node *node = msg->head;
  while (node != NULL) {
    if (node->port_id == port_id) {
      return false;
    }
    node = node->next;
  }
  return true;
}

list *devs_port_get_values(const devs_message *msg, unsigned int port_id)
{
  list *values = list_new();
  devs_node *node = msg->head;
  while (node != NULL)
  {
    if (node->port_id == port_id)
    {
      list_push_back(values, node->value);
    }
    node = node->next;
  }
  return values;
}

void *devs_port_get_value(const devs_message *msg, unsigned int port_id)
{
  devs_node *node = msg->head;
  while (node != NULL)
  {
    if (node->port_id == port_id)
    {
      return node->value;
    }
    node = node->next;
  }
  return NULL;
}

list *list_new()
{
  list *l = (list *)malloc(sizeof(list));
  l->head = NULL;
  l->tail = NULL;
  return l;
}

void list_delete(list *l)
{
  if (!list_is_empty(l))
  {
    list_node *first = l->head;
    while (first != NULL)
    {
      list_node *next = first->next;
      free(first);
      first = next;
    }
  }
  free(l);
}

bool list_is_empty(const list *l)
{
  return l->head == NULL;
}

void list_push_back(list *l, void *data)
{
  list_node *node = (list_node *)malloc(sizeof(node));
  node->data = data;
  node->next = NULL;
  if (list_is_empty(l))
  {
    l->head = node;
    node->prev = NULL;
    node->next = NULL;
  }
  else
  {
    l->tail->next = node;
    node->prev = l->tail;
  }
  l->tail = node;
}
