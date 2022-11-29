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

#include "coupled.h"

coupled *coupled_ho_new(int width, int depth, double preparation_time, double int_delay_time, double ext_delay_time)
{
    coupled *ho = (coupled *)malloc(sizeof(coupled));
    if (depth == 1)
    {
        atomic *atomic = dsatomic_new(preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(ho->components), atomic);
        add_coupling(ho, ho, DSCOUPLED_IN, atomic, DSATOMIC_IN);
        add_coupling(ho, atomic, DSATOMIC_OUT, ho, DSCOUPLED_OUT);
    }
    else
    {
        coupled *ho_child = coupled_ho_new(width, depth - 1, preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(ho->components), ho_child);
        add_coupling(ho, ho, DSCOUPLED_IN, ho_child, DSCOUPLED_IN);
        add_coupling(ho, ho, DSCOUPLED_IN, ho_child, DSCOUPLED_IN_AUX);
        add_coupling(ho, ho_child, DSCOUPLED_OUT, ho, DSCOUPLED_OUT);
        atomic *atomic_prev = 0;
        for (int i = 0; i < (width - 1); ++i)
        {
            atomic *atomic = dsatomic_new(preparation_time, int_delay_time, ext_delay_time);
            list_push_back(&(ho->components), atomic);
            add_coupling(ho, ho, DSCOUPLED_IN_AUX, atomic, DSATOMIC_IN);
            add_coupling(ho, atomic, DSATOMIC_OUT, ho, DSCOUPLED_OUT_AUX);
            if (atomic_prev != 0)
            {
                add_coupling(ho, atomic_prev, DSATOMIC_OUT, atomic, DSATOMIC_IN);
            }
            atomic_prev = atomic;
        }
    }
    return ho;
}
