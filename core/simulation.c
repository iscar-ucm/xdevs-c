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

simulator *simulator_new(atomic *m)
{
  simulator *s = (simulator *)malloc(sizeof(simulator));
  s->simulator_type = DEVS_SIMULATOR;
  s->m = m;
  return s;
}

void simulator_delete(simulator *s)
{
  free(s->m);
  free(s);
  return;
}

void simulator_initialize(simulator *s)
{
  s->m->initialize(s->m);
  s->tL = 0.0;
  s->tN = s->tL + s->m->ta(s->m);
}

void simulator_exit(simulator *s)
{
  s->m->exit(s->m);
}

double simulator_ta(const simulator *s)
{
  return s->m->ta(s->m);
}

void simulator_deltfcn(simulator *s, double t)
{
  bool is_input_empty = devs_message_is_empty(&(s->m->input));
  if (!is_input_empty)
  {
    double e = t - s->tL;
    if (t >= s->tN)
    {
      s->m->deltcon(s->m, e);
    }
    else
    {
      s->m->deltext(s->m, e);
    }
  }
  else if (t >= s->tN)
    s->m->deltint(s->m);
  else
    return;
  s->tL = t;
  s->tN = s->tL + s->m->ta(s->m);
}

void simulator_lambda(simulator *s, double t)
{
  if (t >= s->tN)
  {
    s->m->lambda(s->m);
  }
}

void simulator_clear(simulator *s)
{
  devs_message_clear(&(s->m->input));
  devs_message_clear(&(s->m->output));
}

coordinator *coordinator_new(coupled *m)
{
  coordinator *c = (coordinator *)malloc(sizeof(coordinator));
  c->simulator_type = DEVS_COORDINATOR;
  c->m = m;
  list_node *n = m->components.head;
  while (n != NULL)
  {
    int *component_type = (int *)(n->data + 0);
    bool is_coupled = DEVS_IS_COUPLED(*component_type);
    bool is_atomic = DEVS_IS_ATOMIC(*component_type);
    if (is_coupled)
    {
      coordinator *c_child = coordinator_new(n->data);
      list_push_back(&(c->simulators), c_child);
    }
    else if (is_atomic)
    {
      simulator *s_child = simulator_new(n->data);
      list_push_back(&(c->simulators), s_child);
    }
    n = n->next;
  }
  return c;
}

void coordinator_delete(coordinator *c)
{
  list_node *n = c->simulators.head;
  while (n != NULL)
  {
    list_node *next = n->next;
    int *simulator_type = (int *)(n->data + 0);
    bool is_simulator = DEVS_IS_SIMULATOR(*simulator_type);
    bool is_coordinator = DEVS_IS_COORDINADOR(*simulator_type);
    if (is_simulator)
    {
      simulator_delete(n->data);
    }
    else if (is_coordinator)
    {
      coordinator_delete(n->data);
    }
    n = next;
  }
  list_clear(&(c->m->components));
  list_clear(&(c->m->ic));
  list_clear(&(c->m->eic));
  list_clear(&(c->m->eoc));
  free(c->m);
  free(c);
  return;
}

void coordinator_initialize(coordinator *c)
{
  list_node *n = c->simulators.head;
  while (n != NULL)
  {
    int *simulator_type = (int *)(n->data + 0);
    bool is_simulator = DEVS_IS_SIMULATOR(*simulator_type);
    bool is_coordinator = DEVS_IS_COORDINADOR(*simulator_type);
    if (is_simulator)
    {
      simulator_initialize(n->data);
    }
    else if (is_coordinator)
    {
      coordinator_initialize(n->data);
    }
    n = n->next;
  }
  c->tL = 0.0;
  double min_sigma = coordinator_ta(c, 0);
  c->tN = c->tL + min_sigma;
  return;
}

void coordinator_exit(coordinator *c)
{
  list_node *n = c->simulators.head;
  while (n != NULL)
  {
    int *simulator_type = (int *)(n->data + 0);
    bool is_simulator = DEVS_IS_SIMULATOR(*simulator_type);
    bool is_coordinator = DEVS_IS_COORDINADOR(*simulator_type);
    if (is_simulator)
    {
      simulator_exit(n->data);
    }
    else if (is_coordinator)
    {
      coordinator_exit(n->data);
    }
    n = n->next;
  }
}

double coordinator_ta(const coordinator *c, double t)
{
  double tn = INFINITY;
  list_node *n = c->simulators.head;
  while (n != NULL)
  {
    bool is_simulator = DEVS_IS_SIMULATOR(*((int *)(n->data + 0)));
    double sub_tn = (is_simulator) ? ((simulator*)(n->data))->tN : ((coordinator*)(n->data))->tN;
    if (sub_tn < tn)
      tn = sub_tn;
    n = n->next;
  }
  return tn - t;
}

void coordinator_lambda(coordinator *c, double t)
{
  list_node *n = c->simulators.head;
  while (n != NULL)
  {
    int *simulator_type = (int *)(n->data + 0);
    bool is_simulator = DEVS_IS_SIMULATOR(*simulator_type);
    bool is_coordinator = DEVS_IS_COORDINADOR(*simulator_type);
    if (is_simulator)
    {
      simulator_lambda(n->data, t);
    }
    else if (is_coordinator)
    {
      coordinator_lambda(n->data, t);
    }
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
    int *simulator_type = (int *)(n->data + 0);
    bool is_simulator = DEVS_IS_SIMULATOR(*simulator_type);
    bool is_coordinator = DEVS_IS_COORDINADOR(*simulator_type);
    if (is_simulator)
    {
      simulator_deltfcn(n->data, t);
    }
    else if (is_coordinator)
    {
      coordinator_deltfcn(n->data, t);
    }
    n = n->next;
  }
  c->tL = t;
  c->tN = c->tL + coordinator_ta(c, t);
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
  list_node *n = c->m->eic.head;
  while (n != NULL)
  {
    coupling *link = n->data;
    devs_node *msg_node = ((coupled *)(link->component_from))->input.head;
    bool is_atomic_to = DEVS_IS_ATOMIC(*((int *)(link->component_to + 0)));
    devs_message *ports_addr = (is_atomic_to) ? &(((atomic *)(link->component_to))->input) : &(((coupled *)(link->component_to))->input);
    while (msg_node != NULL)
    {
      if (msg_node->port_id == link->port_from)
      {
        devs_message_push_back(ports_addr, link->port_to, msg_node->value);
      }
      msg_node = msg_node->next;
    }
    n = n->next;
  }
}

void coordinator_propagate_output(coordinator *c)
{
  list_node *n = c->m->ic.head;
  while (n != NULL)
  {
    coupling *link = n->data;
    bool is_atomic_from = DEVS_IS_ATOMIC(*((int *)(link->component_from + 0)));
    bool is_atomic_to = DEVS_IS_ATOMIC(*((int *)(link->component_to + 0)));
    devs_node *msg_node = (is_atomic_from) ? ((atomic *)(link->component_from))->output.head : ((coupled *)(link->component_from))->output.head;
    devs_message *ports_addr = (is_atomic_to) ? &(((atomic *)(link->component_to))->input) : &(((coupled *)(link->component_to))->input);
    while (msg_node != NULL)
    {
      if (msg_node->port_id == link->port_from)
      {
        devs_message_push_back(ports_addr, link->port_to, msg_node->value);
      }
      msg_node = msg_node->next;
    }
    n = n->next;
  }
  n = c->m->eoc.head;
  while (n != NULL)
  {
    coupling *link = n->data;
    devs_message *ports_addr = &(((coupled *)(link->component_to))->output);
    bool is_atomic_from = DEVS_IS_ATOMIC(*((int *)(link->component_from + 0)));
    devs_node *msg_node = (is_atomic_from) ? ((atomic *)(link->component_from))->output.head : ((coupled *)(link->component_from))->output.head;
    while (msg_node != NULL)
    {
      if (msg_node->port_id == link->port_from)
      {
        devs_message_push_back(ports_addr, link->port_to, msg_node->value);
      }
      msg_node = msg_node->next;
    }
    n = n->next;
  }
}

void coordinator_clear(coordinator *c)
{
  list_node *n = c->simulators.head;
  while (n != NULL)
  {
    int *simulator_type = (int *)(n->data + 0);
    bool is_simulator = DEVS_IS_SIMULATOR(*simulator_type);
    bool is_coordinator = DEVS_IS_COORDINADOR(*simulator_type);
    if (is_simulator)
    {
      simulator_clear(n->data);
    }
    else if (is_coordinator)
    {
      coordinator_clear(n->data);
    }
    n = n->next;
  }
  devs_message_clear(&(c->m->input));
  devs_message_clear(&(c->m->output));
}
