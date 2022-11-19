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
#ifndef TRANSDUCER_H
#define TRANSDUCER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "job.h"
#include "../../core/modeling.h"

#define TRANSDUCER_ARRIVED 0
#define TRANSDUCER_SOLVED 1
#define TRANSDUCER_OUT 2

typedef struct st_transducer_state {
  double clock, total_ta, obs_time;
  list *jobs_arrived, *jobs_solved;
} transducer_state;

atomic *transducer_new(double obs_time);

#endif /* TRANSDUCER_H */
