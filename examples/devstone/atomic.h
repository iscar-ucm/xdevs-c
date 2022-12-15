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
#ifndef DSATOMIC_H
#define DSATOMIC_H

#include <stdlib.h>
#include <string.h>
#include "../../core/modeling.h"

#define DSATOMIC_IN 0
#define DSATOMIC_OUT 1

typedef struct st_dsatomic_state {
  double preparation_time, int_delay_time, ext_delay_time;
  long* event;
} dsatomic_state;

atomic *dsatomic_new(double preparation_time, double int_delay_time, double ext_delay_time);
void dhrystone(double delay);

#endif /* DSATOMIC_H */
