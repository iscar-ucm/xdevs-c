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
#include "generator.h"

devs_message* lambda(const devs_state* s) {
  devs_message* msg = new_devs_message(); 
  push_back_devs_message(msg, PORT_OUT, s->user_state);
  return msg;
}

devs_state* deltext(devs_state* state) {
  passivate();
}

struct atomic_operations processor = {
                                      .ta = default_ta,
                                      .lambda = lambda,
                                      .deltext = default_deltext
};

