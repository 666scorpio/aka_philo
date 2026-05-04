#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct  s_dongle
{
    pthread_mutex_t mutex;
} t_dongle;

typedef struct s_data
{
    t_coder     *coder;
    int     time_to_compile;
    int     time_to_debug;
    int     time_to_refactor;
    int     time_to_burnout;
    int     dongle_cooldown;
} t_data;

typedef struct   s_coder
{
    int     id;
    pthread_t thread;
    long    last_compile;
    int     nb_compile;
    t_dongle    *left;
    t_dongle    *right;
} t_coder;

void    *coder_routine(void *arg)
{
    t_data *data = (t_data *)arg;

    pthread_mutex_lock(&data->coder->left->mutex);
    pthread_mutex_lock(&data->coder->right->mutex);
    printf("coder %d is compiling now\n", data->coder->id);
    sleep(data->time_to_compile);
    sleep(data->dongle_cooldown);
    pthread_mutex_unlock(&data->coder->left->mutex);
    pthread_mutex_unlock(&data->coder->right->mutex);
    printf("coder %d is debuging now\n", data->coder->id);
    sleep(data->time_to_debug);
    printf("coder %d is refactoring now\n", data->coder->id);
    sleep(data->time_to_refactor);
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
    
    time_to_compile = 5;
    time_to_debug = 5;
    time_to_refactor = 5;
    time_to_burnout = 5;
    dongle_cooldown = 5;
    scheduler = "EDF";
    n_coders = 10;
    t_data data[n_coders];
    t_coder *coders = malloc(sizeof(t_coder) * n_coders);
    t_dongle *dongles = malloc(sizeof(t_dongle) * n_coders);

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
        i++;
    }

    // loop to generate data for all the threads
    i = 0;
    while (i < n_coders)
    {
        data[i].coder = &coders[i];
        data[i].dongle_cooldown = dongle_cooldown;
        data[i].time_to_burnout = time_to_burnout;
        data[i].time_to_compile = time_to_compile;
        data[i].time_to_debug = time_to_debug;
        data[i].time_to_refactor = time_to_refactor;
        i++;
    }

    i = 0;
    while (i < n_coders)
    {
        pthread_create(&coders[i].thread, NULL, coder_routine, &data[i]);
        i++;
    }
    
    i = 0;
    while (i < n_coders)
    {
        pthread_join(coders[i].thread, NULL);
        i++;
    }
}