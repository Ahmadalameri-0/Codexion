/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 11:54:02 by abani-am          #+#    #+#             */
/*   Updated: 2026/10/10 12:29:43 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

// Gets the current time and sets in tv
long long	get_current_time(void)
{
	struct timeval	time_value;

	if (gettimeofday(&time_value, NULL) == -1)
	{
		return (-1);
	}
	return ((time_value.tv_sec * 1000LL) + (time_value.tv_usec / 1000));
}

long long	get_time_elapsed(long long start_time)
{
	return (get_current_time() - start_time);
}

int	is_stopped(t_coder *coder)
{
	int	stop;

	pthread_mutex_lock(&coder->data->state_mutex);
	stop = coder->data->simulation_stop;
	pthread_mutex_unlock(&coder->data->state_mutex);
	return (stop);
}

void	ft_usleep(long long time_in_ms, t_data *data)
{
	long long	start_time;
	int			stop;

	start_time = get_current_time();
	while ((get_current_time() - start_time) < time_in_ms)
	{
		pthread_mutex_lock(&data->state_mutex);
		stop = data->simulation_stop;
		pthread_mutex_unlock(&data->state_mutex);
		if (stop == 1)
			break ;
		usleep(500);
	}
}

void	timed_wait(t_data *data, long long ms)
{
	struct timespec	ts;
	long long		abs_ms;

	abs_ms = get_current_time() + ms;
	ts.tv_sec = abs_ms / 1000;
	ts.tv_nsec = (abs_ms % 1000) * 1000000;
	pthread_cond_timedwait(&data->table_cond, &data->table_lock, &ts);
}
