#include "codexion.h"

void    *coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;    
    while (coder->simulation_running == 1)
    {
        if (!compile(coder))
            break;
        if (!debug(coder))
            break;
        if (!refactor(coder))
            break;
    }
    return NULL;
}

int main()
{
    int     n_coders;
    int     time_to_compile;
    int     time_to_debug;
    int     time_to_refactor;
    int     time_to_burnout;
    int     dongle_cooldown;
    char    *scheduler;
    int     i;
    t_coder *coders;
    t_dongle *dongles;
    t_monitor   monitor;
    
    time_to_compile = 5000;
    time_to_debug = 5000;
    time_to_refactor = 5000;
    time_to_burnout = 5000;
    dongle_cooldown = 5000;
    scheduler = "EDF";
    n_coders = 10;
    coders = malloc(sizeof(t_coder) * n_coders);
    dongles = malloc(sizeof(t_dongle) * n_coders);

    i = 0;
    while (i < n_coders)
    {
        pthread_mutex_init(&dongles[i].mutex, NULL);
        i++;
    }
    
    i = 0;
    while (i < n_coders)
    {
        coders[i].id = i;
        coders[i].left = &dongles[i];
        coders[i].right = &dongles[(i + 1) % n_coders];
        coders[i].dongle_cooldown = dongle_cooldown;
        coders[i].time_to_burnout = time_to_burnout;
        coders[i].time_to_compile = time_to_compile;
        coders[i].time_to_debug = time_to_debug;
        coders[i].time_to_refactor = time_to_refactor;
        coders[i].compiles_count = 0;
        coders[i].simulation_running = 1;
        i++;
    }

    i = 0;
    while (i < n_coders)
    {
        i++;
    }

    i = 0;
    while (i < n_coders)
    {
        pthread_create(&coders[i].thread, NULL, coder_routine, &coders[i]);
        i++;
    }
    monitor.coders = coders;
    monitor.n_coders = n_coders;
    monitor.number_of_compiles_required = 3;
    pthread_create(&monitor.monitor_thread_id, NULL, monitor_routine, &monitor);
    i = 0;
    while (i < n_coders)
    {
        pthread_join(coders[i].thread, NULL);
        i++;
    }
    pthread_join(monitor.monitor_thread_id, NULL);
}