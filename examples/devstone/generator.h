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
#ifndef GENERATOR_H
#define GENERATOR_H

#include "../../core/modeling.h"

#define GENERATOR_OUT 0

typedef struct st_generator_state
{
  double preparation_time, period;
  long *event;
  long max_events;
} generator_state;

/**
 * @brief      Creates a new generator atomic model.
 *
 * @details    Creates a new generator DEVS atomic model.
 *
 * @param      period Jobs generation period.
 *
 * @return     a pointer to the atomic model.
 */
atomic *generator_new(double preparation_time, double period, long max_events);

#endif /* GENERATOR_H */
