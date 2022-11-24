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
#include "../../core/devs.h"
#include "generator.h"
#include "../../core/modeling.h"
#include "processor.h"
#include "transducer.h"
#include "../../core/simulation.h"

int main(int argc, char *argv[])
{
  coupled* gpt = coupled_new();
  gpt->component_type = DEVS_COUPLED;

  atomic *g = generator_new(1.0);
  atomic *p = processor_new(3.0);
  atomic *t = transducer_new(100.0);
  list_push_back(&(gpt->components), g);
  list_push_back(&(gpt->components), p);
  list_push_back(&(gpt->components), t);

  add_coupling(gpt, g, GENERATOR_OUT, p, PROCESSOR_IN);
  add_coupling(gpt, g, GENERATOR_OUT, t, TRANSDUCER_ARRIVED);
  add_coupling(gpt, p, PROCESSOR_OUT, t, TRANSDUCER_SOLVED);
  add_coupling(gpt, t, TRANSDUCER_OUT, g, GENERATOR_IN);

  coordinator *c = coordinator_new(gpt);
  coordinator_initialize(c);
  coordinator_simulate(c, 20);
  coordinator_exit(c);
  coordinator_delete(c);

  return 0;
}
