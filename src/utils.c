/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 11:54:02 by abani-am          #+#    #+#             */
/*   Updated: 2026/09/22 16:31:28 by abani-am         ###   ########.fr       */
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

void	ft_usleep(long long time_in_ms, t_data *data)
{
	long long	start_time;

	start_time = get_current_time();
	while ((get_current_time() - start_time) < time_in_ms)
	{
		pthread_mutex_lock(&data->state_mutex);
		if(data->simulation_stop == 1)
		{
			pthread_mutex_unlock(&data->state_mutex);
			break;
		}
		pthread_mutex_unlock(&data->simulation_stop);
		usleep(500);
	}
}
