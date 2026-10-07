/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 14:19:19 by abani-am          #+#    #+#             */
/*   Updated: 2026/09/30 12:02:48 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

void	stop_simulation(t_data *data)
{
	int i;

	pthread_mutex_lock(&data->state_mutex);
	data->simulation_stop = 1;
	pthread_mutex_unlock(&data->state_mutex);
	i = 0;
	while (i < data->nb_coders)
	{
		pthread_mutex_lock(&data->dongle[i].lock);
		pthread_cond_broadcast(&data->dongle[i].cond);
		pthread_mutex_unlock(&data->dongle[i].lock);
		i++;
	}
}

static int	check_death(t_data *data)
{
	int			i;
	long long	elapsed;

	i = 0;
	while (i < data->nb_coders)
	{
		pthread_mutex_lock(&data->state_mutex);
		elapsed = get_current_time() - data->coders[i].last_compile_time;
		pthread_mutex_unlock(&data->state_mutex);
		if (elapsed >= data->time_to_burnout)
		{
			pthread_mutex_lock(&data->write_mutex);
			printf("Time: %3lldms | Coder ID: %d burned out\n",
				   get_time_elapsed(data->start_time), data->coders[i].id);
			pthread_mutex_unlock(&data->write_mutex);
			stop_simulation(data);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_completion(t_data *data)
{
	int		i;
	int		finished;

	i = 0;
	finished = 0;
	while (i < data->nb_coders)
	{
		pthread_mutex_lock(&data->state_mutex);
		if (data->coders[i].compiles_count >= data->nb_compiles_required)
			finished++;
		pthread_mutex_unlock(&data->state_mutex);
		i++;
	}
	if (finished == data->nb_coders)
	{
		stop_simulation(data);
		return (1);
	}
	return (0);
}

void	monitor_routine(t_data *data)
{
	while (1)
	{
		if (check_death(data) == 1)
			break ;
		if (check_completion(data) == 1)
			break ;
		usleep(1000);
	}
}
