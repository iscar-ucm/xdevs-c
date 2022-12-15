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

coupled *coupled_li_new(int width, int depth, double preparation_time, double int_delay_time, double ext_delay_time)
{
    coupled *li = coupled_new();
    if (depth == 1)
    {
        atomic *atomic = dsatomic_new(preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(li->components), atomic);
        add_coupling(li, li, DSCOUPLED_IN, atomic, DSATOMIC_IN);
        add_coupling(li, atomic, DSATOMIC_OUT, li, DSCOUPLED_OUT);
    }
    else
    {
        coupled *li_child = coupled_li_new(width, depth - 1, preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(li->components), li_child);
        add_coupling(li, li, DSCOUPLED_IN, li_child, DSCOUPLED_IN);
        add_coupling(li, li_child, DSCOUPLED_OUT, li, DSCOUPLED_OUT);
        for (int i = 0; i < (width - 1); ++i)
        {
            atomic *atomic = dsatomic_new(preparation_time, int_delay_time, ext_delay_time);
            list_push_back(&(li->components), atomic);
            add_coupling(li, li, DSCOUPLED_IN, atomic, DSATOMIC_IN);
        }
    }
    return li;
}

coupled *coupled_hi_new(int width, int depth, double preparation_time, double int_delay_time, double ext_delay_time)
{
    coupled *hi = coupled_new();
    if (depth == 1)
    {
        atomic *atomic = dsatomic_new(preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(hi->components), atomic);
        add_coupling(hi, hi, DSCOUPLED_IN, atomic, DSATOMIC_IN);
    }
    else
    {
        coupled *hi_child = coupled_hi_new(width, depth - 1, preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(hi->components), hi_child);
        add_coupling(hi, hi, DSCOUPLED_IN, hi_child, DSCOUPLED_IN);
        add_coupling(hi, hi_child, DSCOUPLED_OUT, hi, DSCOUPLED_OUT);
        atomic *atomic_prev = 0;
        for (int i = 0; i < (width - 1); ++i)
        {
            atomic *atomic = dsatomic_new(preparation_time, int_delay_time, ext_delay_time);
            list_push_back(&(hi->components), atomic);
            add_coupling(hi, hi, DSCOUPLED_IN, atomic, DSATOMIC_IN);
            if (atomic_prev != 0)
            {
                add_coupling(hi, atomic_prev, DSATOMIC_OUT, atomic, DSATOMIC_IN);
            }
            atomic_prev = atomic;
        }
    }
    return hi;
}

coupled *coupled_ho_new(int width, int depth, double preparation_time, double int_delay_time, double ext_delay_time)
{
    coupled *ho = coupled_new();
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

coupled *coupled_homod_new(int width, int depth, double preparation_time, double int_delay_time, double ext_delay_time)
{
    coupled *homod = coupled_new();
    if (depth == 1)
    {
        atomic *atomic = dsatomic_new(preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(homod->components), atomic);
        add_coupling(homod, homod, DSCOUPLED_IN, atomic, DSATOMIC_IN);
        add_coupling(homod, atomic, DSATOMIC_OUT, homod, DSCOUPLED_OUT);
    }
    else
    {
        coupled *homod_child = coupled_homod_new(width, depth - 1, preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(homod->components), homod_child);
        add_coupling(homod, homod, DSCOUPLED_IN, homod_child, DSCOUPLED_IN);
        add_coupling(homod, homod_child, DSCOUPLED_OUT, homod, DSCOUPLED_OUT);

        // First layer of atomic models:
        list *prev_layer = list_new();
        for (int i = 0; i < (width - 1); ++i)
        {
            atomic *atomic = dsatomic_new(preparation_time, int_delay_time, ext_delay_time);
            list_push_back(&(homod->components), atomic);
            add_coupling(homod, homod, DSCOUPLED_IN_AUX, atomic, DSATOMIC_IN);
            add_coupling(homod, atomic, DSATOMIC_OUT, homod_child, DSCOUPLED_IN_AUX);
            list_push_back(prev_layer, atomic);
        }
        // Second layer of atomic models:
        list *curr_layer = list_new();
        for (int i = 0; i < (width - 1); ++i)
        {
            atomic *atomic = dsatomic_new(preparation_time, int_delay_time, ext_delay_time);
            list_push_back(&(homod->components), atomic);
            if (i == 0)
            {
                add_coupling(homod, homod, DSCOUPLED_IN_AUX, atomic, DSATOMIC_IN);
            }
            list_push_back(curr_layer, atomic);
        }
        list_node *first = curr_layer->head;
        while (first != NULL)
        {
            atomic *from_atomic = (atomic *)(first->data);
            list_node *second = prev_layer->head;
            while (second != NULL)
            {
                atomic *to_atomic = (atomic *)(second->data);
                add_coupling(homod, from_atomic, DSATOMIC_OUT, to_atomic, DSATOMIC_IN);
                second = second->next;
            }
            first = first->next;
        }

        // Rest of the tree
        list_clear(prev_layer);
        prev_layer = curr_layer;
        curr_layer = list_new();
        while (list_size(prev_layer) > 1)
        {
            list_node *first_prev = prev_layer->head;
            while (first_prev->next != NULL)
            {
                atomic *atomic = dsatomic_new(preparation_time, int_delay_time, ext_delay_time);
                list_push_back(&(homod->components), atomic);
                if (list_size(curr_layer) == 0)
                {
                    add_coupling(homod, homod, DSCOUPLED_IN_AUX, atomic, DSATOMIC_IN);
                }
                list_push_back(curr_layer, atomic);
                first_prev = first_prev->next;
            }
            list_node *first_curr = curr_layer->head;
            first_prev = prev_layer->head;
            while(first_curr!=NULL) {
                atomic *from_atomic = (atomic *)(first_curr->data);
                atomic* to_atomic = (atomic *)(first_prev->next->data);
                add_coupling(homod, from_atomic, DSATOMIC_OUT, to_atomic, DSATOMIC_IN);
                first_curr = first_curr->next;
                first_prev = first_prev->next;
            }
            list_clear(prev_layer);
            prev_layer = curr_layer;
            curr_layer = list_new();
        }
    }
    return homod;
}
