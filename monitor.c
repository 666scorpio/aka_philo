#include "codexion.h"

int check_bournout(t_coder *coder)
{
    if (get_time() - coder->last_compile >= coder->time_to_burnout)
        return 1;
    return 0;
}

int check_if_we_reached_the_number_of_compiles_required(t_monitor *monitor)
{
    int i;
    int c;

    i = 0;
    c = 0;
    while (i < monitor->n_coders)
    {
        if (monitor->coders[i].compiles_count >= monitor->number_of_compiles_required)
        {
            monitor->coders[i].simulation_running = 0;
            c++;
        }
        i++;
    }
    return(c == monitor->n_coders);
}

void    *monitor_routine(void *arg)
{
    int i;
    int x;
    int running;
    t_monitor   *monitor;

    running = 1;
    monitor = (t_monitor *)arg;
    while(running)
    {
        i = 0;
        while (i < monitor->n_coders)
        {
            if (check_if_we_reached_the_number_of_compiles_required(monitor))
            {
                running = 0;
                x = 0;
                while (x < monitor->n_coders)
                    monitor->coders[x++].simulation_running = 0;
            }
            if (monitor->coders[i].simulation_running)
            {
                if (check_bournout(&monitor->coders[i]))
                {
                    printf("coder %d burnouted\n", monitor->coders[i].id);
                    running = 0;
                    x = 0;
                    while (x < monitor->n_coders)
                        monitor->coders[x++].simulation_running = 0;
                    return NULL;
                }
                if (monitor->coders[i].compiles_count >= monitor->number_of_compiles_required)
                {
                    monitor->coders[i].simulation_running = 0;
                    x = 0;
                    while (x < monitor->n_coders)
                        monitor->coders[x++].simulation_running = 0;
                }
            }
            i++;
        }
    }
    return NULL;
}