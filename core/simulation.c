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
#include "simulation.h"
#include "devs.h"

simulator *simulator_new(atomic *model)
{
  simulator *c = (simulator *)malloc(sizeof(simulator));
  c->model = model;
  return c;
}

void simulator_delete(simulator *c)
{
  free(c);
  return;
}

void simulator_initialize(simulator *c, atomic *model)
{
  c->model = model;
  c->model->initialize(c->model);
  c->tL = 0.0;
  c->tN = c->tL + c->model->ta(c->model);
}

void simulator_exit(simulator *c)
{
  c->model->exit(c->model);
}

double simulator_ta(const simulator *c)
{
  return c->model->ta(c->model);
}

void simulator_deltfcn(simulator *c)
{
  double t = 0.0;
  bool is_input_empty = devs_message_is_empty(&(c->model->input));
  if (!is_input_empty)
  {
    double e = c->tL;
    if (t == c->tN)
    {
      c->model->deltcon(c->model, e);
    }
    else
    {
      c->model->deltext(c->model, e);
    }
  }
  else if (t == c->tN)
    c->model->deltint(c->model);
  else
    return;
  c->tL = t;
  c->tN = c->tL + c->model->ta(c->model);
  /* TODO: Should I free here the memory associated with msg??? */
}

void simulator_lambda(simulator *s, double t)
{
  if (t >= s->tN)
  {
    s->model->lambda(s->model);
  }
}

coordinator *coordinator_new(coupled *model)
{
  coordinator *c = (coordinator *)malloc(sizeof(coordinator));
  c->model = model;
  list_node *current_node = model->components.head;
  while (current_node != NULL)
  {
    int *component_type = (int *)(current_node->data + 0);
    bool is_coupled = DEVS_IS_COUPLED(*component_type);
    bool is_atomic = DEVS_IS_ATOMIC(*component_type);
    if (is_coupled)
    {
      coordinator *current_sim = coordinator_new(current_node->data);
      list_push_back(&(c->simulators), current_sim);
    }
    else if (is_atomic)
    {
      simulator *current_sim = simulator_new(current_node->data);
      list_push_back(&(c->simulators), current_sim);
    }
    current_node = current_node->next;
  }
  return c;
}

void coordinator_initialize(coordinator *c)
{
  list_node *n = c->simulators.head;
  while (n != NULL)
  {
    simulator *s = n->data;
    simulator_initialize(s, s->model);
    n = n->next;
  }
  c->tL = 0.0;
  c->tN = c->tL + coordinator_ta(c);
}

double coordinator_ta(const coordinator *c)
{
  double tn = INFINITY;
  list_node *n = c->simulators.head;
  while (n != NULL)
  {
    simulator *s = n->data;
    if (s->tN < tn)
      tn = s->tN;
    n = n->next;
  }
  return tn;
}

void coordinator_lambda(coordinator *c, double t)
{
  list_node *n = c->simulators.head;
  while (n != NULL)
  {
    simulator *s = n->data;
    simulator_lambda(s, t);
    n = n->next;
  }
  coordinator_propagate_output(c);
}

void coordinator_deltfcn(coordinator *c, double t)
{
  coordinator_propagate_input(c);
  list_node *n = c->simulators.head;
  while (n != NULL)
  {
    simulator *s = n->data;
    simulator_deltfcn(s);
    n = n->next;
  }
  c->tL = 0;
  c->tN = c->tL + coordinator_ta(c);
}

void coordinator_exit(coordinator *c)
{
  list_node *n = c->simulators.head;
  while (n != NULL)
  {
    simulator *s = n->data;
    simulator_exit(s);
  }
}

void coordinator_simulate(coordinator *c, unsigned long int nsteps)
{
  printf("Starting simulation ...\n");
  unsigned long int steps = 0;
  double t = 0.0;
  while (steps < nsteps && c->tN < INFINITY)
  {
    t = c->tN;
    coordinator_lambda(c, t);
    coordinator_deltfcn(c, t);
    coordinator_clear(c);
    steps++;
  }
}

void coordinator_propagate_input(coordinator *c)
{
  // TODO: Implement this function.
}

void coordinator_propagate_output(coordinator *c)
{
  list_node *n = c->model->ic.head;
  while (n != NULL)
  {
    coupling *link = n->data;
    coupling_progapate_values(link);
    n = n->next;
  }
  n = c->model->eoc.head;
  while (n != NULL)
  {
    coupling *link = n->data;
    coupling_progapate_values(link);
    n = n->next;
  }
}

void coordinator_clear(coordinator *c)
{
  // TODO: Implement this function.
}
