/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 09:12:52 by abani-am          #+#    #+#             */
/*   Updated: 2026/10/03 09:12:53 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

static void	clean_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		pthread_mutex_destroy(&data->dongle[i].lock);
		pthread_cond_destroy(&data->dongle[i].cond);
		free(data->dongle[i].queue.items);
		i++;
	}
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	free(data->dongle);
	free(data->coders);
}

static int	check_finished(t_data *data)
{
	int	i;
	int	finished;

	if (data->nb_compiles_required <= 0)
		return (0);
	i = -1;
	finished = 0;
	while (++i < data->nb_coders)
	{
		pthread_mutex_lock(&data->state_mutex);
		if (data->coders[i].compiles_count >= data->nb_compiles_required)
			finished++;
		pthread_mutex_unlock(&data->state_mutex);
	}
	if (finished == data->nb_coders)
	{
		pthread_mutex_lock(&data->state_mutex);
		data->simulation_stop = 1;
		pthread_mutex_unlock(&data->state_mutex);
		return (1);
	}
	return (0);
}

static int	check_death(t_data *data, int i)
{
	long long	elapsed;

	pthread_mutex_lock(&data->state_mutex);
	elapsed = get_current_time() - data->coders[i].last_compile_time;
	if (elapsed >= data->time_to_burnout)
	{
		data->simulation_stop = 1;
		pthread_mutex_unlock(&data->state_mutex);
		pthread_mutex_lock(&data->write_mutex);
		printf("Time: %3lldms | Coder ID: %d burned out\n",
			get_time_elapsed(data->start_time), data->coders[i].id);
		pthread_mutex_unlock(&data->write_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->state_mutex);
	return (0);
}

static void	monitor_routine(t_data *data)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < data->nb_coders)
		{
			if (check_death(data, i))
				return ;
		}
		if (check_finished(data))
			return ;
		usleep(1000);
	}
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (parse_args(argc, argv, &data) != 0)
		return (1);
	if (init_simulation(&data) != 0)
		return (1);
	i = -1;
	while (++i < data.nb_coders)
	{
		if (pthread_create(&data.coders[i].thread, NULL,
				coder_routine, &data.coders[i]) != 0)
			return (1);
	}
	monitor_routine(&data);
	i = -1;
	while (++i < data.nb_coders)
		pthread_join(data.coders[i].thread, NULL);
	clean_simulation(&data);
	return (0);
}
