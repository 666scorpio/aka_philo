#include "codexion.h"

int compile(t_coder *coder)
{
    pthread_mutex_lock(&coder->left->mutex);
    pthread_mutex_lock(&coder->right->mutex);
    printf("coder %d is compiling now\n", coder->id);
    coder->last_compile = get_time();
    usleep(coder->time_to_compile * 1000);
    coder->compiles_count += 1;
    usleep(coder->dongle_cooldown * 1000);
    pthread_mutex_unlock(&coder->left->mutex);
    pthread_mutex_unlock(&coder->right->mutex);
    return coder->simulation_running;
}

int debug(t_coder *coder)
{
    printf("coder %d is debugging now\n", coder->id);
    usleep(coder->time_to_debug * 1000);
    return coder->simulation_running;
}

int refactor(t_coder *coder)
{
    printf("coder %d is refactoring now\n", coder->id);
    usleep(coder->time_to_refactor * 1000);
    return coder->simulation_running;   
}
