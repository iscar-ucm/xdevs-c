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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <limits.h>
#include "../../core/simulation.h"
#include "coupled.h"
#include "generator.h"
#include "atomic.h"

extern long NUM_DELT_INTS;
extern long NUM_DELT_EXTS;
extern long NUM_EVENT_EXT;

#define BENCH_LI 0
#define BENCH_HI 1
#define BENCH_HO 2
#define BENCH_HOmod 3

const char *bench_names[] = {"LI", "HI", "HO", "HOmod"};

void usage(char *name)
{
    fprintf(stderr, "Usage: %s <-w width> <-d depth> [-m max_events=1] <-b {HI | HO | HOmod }>\nwith w>=2, d>=2, m>=1\n", name);
}

long compute_external_transitions(int benchmark, int max_events, int width, int depth) {
  long num_events = 0;
  switch(benchmark) {
  case BENCH_LI:
    num_events = max_events * ((width - 1) * (depth - 1) + 1);
    break;
  case BENCH_HI:
    num_events = max_events * (((width * width - width) / 2) * (depth - 1) + 1);
    break;
  case BENCH_HO:
    num_events = max_events * (((width * width - width) / 2) * (depth - 1) + 1);
    break;
  case BENCH_HOmod: {
    int gamma1 = width - 1;
    int gamma2 = width * (width - 1) / 2;
    int gamma3 = gamma1;
    int delta1 = depth - 1;
    int delta3 = (depth - 1) * (depth - 2) / 2;
    num_events =  max_events * (1 + delta1 * gamma1 * gamma1 + (delta1 + gamma3 * delta3) * (gamma1 + gamma2));
  }
    break;
  default: // We assume HO here
    num_events = max_events * (((width * width - width) / 2) * (depth - 1) + 1);
    break;
  }
  return num_events;
}

long compute_internal_transitions(int benchmark, int max_events, int width, int depth) {
  long num_events = 0;
  switch(benchmark) {
  case BENCH_LI:
    num_events = compute_external_transitions(benchmark, max_events, width, depth);
    break;
  case BENCH_HI:
    num_events = compute_external_transitions(benchmark, max_events, width, depth);
    break;
  case BENCH_HO:
    num_events = compute_external_transitions(benchmark, max_events, width, depth);
    break;
  case BENCH_HOmod:
    num_events = compute_external_transitions(benchmark, max_events, width, depth);
    break;
  default: // We assume HO here
    num_events = compute_external_transitions(benchmark, max_events, width, depth);
  }
  return num_events;
}

long compute_total_events(int benchmark, int max_events, int width, int depth) {
  long num_events = 0;
  switch(benchmark) {
  case BENCH_LI:
    num_events = compute_external_transitions(benchmark, max_events, width, depth);
    break;
  case BENCH_HI:
    num_events = compute_external_transitions(benchmark, max_events, width, depth);
    break;
  case BENCH_HO:
    num_events = compute_external_transitions(benchmark, max_events, width, depth);
    break;
  case BENCH_HOmod: {
    int w1 = width-1, w2 = width-2;
    int d0 = depth, d1 = depth-1, d2 = depth-2;
    num_events = 1 + 2*w1+w1*w1*d2*(2+w1*d1)+(w1*w1+(w2*w1)/2)*((2*d1+w1*d0*d1-2*w1*d1)/2);
    num_events = max_events * num_events;
    break;
  }
  default: // We assume HO here
    num_events = compute_external_transitions(benchmark, max_events, width, depth);
  }
  return num_events;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    // Common parameters
    double preparation_time = 0.0;
    double period = 1.0;
    long max_events = 1;
    double int_delay_time = 0;
    double ext_delay_time = 0;
    struct rusage resources;
    int benchmark = -1;
    int width = -1, depth = -1;
    int opt;

    // TODO
    // Evaluate parameteres: benchmark, width and depth
    while ((opt = getopt(argc, argv, "w:d:b:m:")) != -1)
    {
        switch (opt)
        {
        case 'w':
            width = atoi(optarg);
            break;
        case 'd':
            depth = atoi(optarg);
            break;
        case 'b':
            if (strncmp(optarg, "LI", 2) == 0)
            {
                benchmark = BENCH_LI;
            }
            else if (strncmp(optarg, "HI", 2) == 0)
            {
                benchmark = BENCH_HI;
            }
            else if (strncmp(optarg, "HOmod", 5) == 0)
            { // Be carefull!, this comparison must go before the next one!
                benchmark = BENCH_HOmod;
            }
            else if (strncmp(optarg, "HO", 2) == 0)
            {
                benchmark = BENCH_HO;
            }
            else
            {
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            break;
        case 'm':
            max_events = atoi(optarg);
            break;
        default: /* '?' */
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if ((width < 2) || (depth < 2) || (max_events < 1))
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    coupled *framework = coupled_new();
    atomic *generator = generator_new(preparation_time, period, 1);
    list_push_back(&(framework->components), generator);
    coupled *coupled_stone = 0;

    clock_t ts_start = clock();

    switch (benchmark)
    {
    case BENCH_LI:
        coupled_stone = coupled_li_new(width, depth, preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(framework->components), coupled_stone);
        add_coupling(framework, generator, GENERATOR_OUT, coupled_stone, DSCOUPLED_IN);
        break;
    case BENCH_HI:
        coupled_stone = coupled_hi_new(width, depth, preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(framework->components), coupled_stone);
        add_coupling(framework, generator, GENERATOR_OUT, coupled_stone, DSCOUPLED_IN);
        break;
    case BENCH_HO:
        coupled_stone = coupled_ho_new(width, depth, preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(framework->components), coupled_stone);
        add_coupling(framework, generator, GENERATOR_OUT, coupled_stone, DSCOUPLED_IN);
        add_coupling(framework, generator, GENERATOR_OUT, coupled_stone, DSCOUPLED_IN_AUX);
        break;
    case BENCH_HOmod:
        coupled_stone = coupled_homod_new(width, depth, preparation_time, int_delay_time, ext_delay_time);
        list_push_back(&(framework->components), coupled_stone);
        add_coupling(framework, generator, GENERATOR_OUT, coupled_stone, DSCOUPLED_IN);
        add_coupling(framework, generator, GENERATOR_OUT, coupled_stone, DSCOUPLED_IN_AUX);
        break;
    }

    clock_t ts_end_model_creation = clock();

    // struct timespec ts_start1, ts_start2, ts_end;
    //	time_t start = clock();
    coordinator *c = coordinator_new(framework);
    coordinator_initialize(c);

    clock_t ts_end_engine_setup = clock();

    coordinator_simulate(c, LONG_MAX);

    clock_t ts_end_simulation = clock();

    coordinator_exit(c);
    coordinator_delete(c);

    double time_model = ((double)(ts_end_model_creation - ts_start)) / CLOCKS_PER_SEC;
    double time_engine = ((double)(ts_end_engine_setup - ts_end_model_creation)) / CLOCKS_PER_SEC;
    double time_sim = ((double)(ts_end_simulation - ts_end_engine_setup)) / CLOCKS_PER_SEC;

    if (getrusage(RUSAGE_SELF, &resources))
    {
        perror("rusage");
    }

    printf("STATS\n");
    printf("Benchmark: %s (%d)\n", bench_names[benchmark], benchmark);
    printf("PreparationTime: %f\n", preparation_time);
    printf("Period: %f\n", period);
    printf("MaxEvents: %ld\n", max_events);
    printf("Width: %d\n", width);
    printf("Depth: %d\n", depth);
    printf("IntDelayTime: %f\n", int_delay_time);
    printf("ExtDelatTime: %f\n", ext_delay_time);
    printf("Num delta_int: %ld, [%ld]\n", NUM_DELT_INTS, compute_internal_transitions(benchmark, max_events, width, depth));
    printf("Num delta_ext: %ld, [%ld]\n", NUM_DELT_EXTS, compute_external_transitions(benchmark, max_events, width, depth));
    printf("Num event_ext: %ld, [%ld]\n", NUM_EVENT_EXT, compute_total_events(benchmark, max_events, width, depth));
    printf("Model creation time (s): %f\n", time_model);
    printf("Engine setup time (s): %f\n", time_engine);
    printf("Simulation time (s): %f\n", time_sim);
    printf("TOTAL time (s): %f\n", time_model + time_engine + time_sim);
    printf("MEMORY (KiB): %ld\n", resources.ru_maxrss);

    return 0;
}