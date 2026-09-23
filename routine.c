#include "Codexion.h"

void	print_action(t_coder *coder, char *action)
{
	long long	time_now;

	pthread_mutex_lock(&coder->data->state_mutex);
	if (coder->data->simulation_stop == 1)
	{
		pthread_mutex_unlock(&coder->data->state_mutex);
		return;
	}
	pthread_mutex_unlock(&coder->data->state_mutex);

	pthread_mutex_lock(&coder->data->write_mutex);
	time_now = get_time_elapsed(coder->data->start_time);
	printf("%lld %d %s\n", time_now, coder->id, action);
	pthread_mutex_unlock(&coder->data->write_mutex);
}