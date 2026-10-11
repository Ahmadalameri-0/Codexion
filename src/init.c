/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:47:46 by abani-am          #+#    #+#             */
/*   Updated: 2026/10/10 12:31:38 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

static int	init_dongles(t_data *data)
{
	int	i;

	data->dongle = malloc(sizeof(t_dongle) * data->nb_coders);
	if (!data->dongle)
		return (1);
	i = 0;
	while (i < data->nb_coders)
	{
		data->dongle[i].id = i;
		data->dongle[i].last_used_time = -data->dongle_cooldown;
		data->dongle[i].taken = 0;
		data->dongle[i].queue.size = 0;
		data->dongle[i].queue.capacity = data->nb_coders;
		data->dongle[i].queue.items
			= malloc(sizeof(t_request) * data->nb_coders);
		if (!data->dongle[i].queue.items)
			return (1);
		i++;
	}
	return (0);
}

static int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->write_mutex, NULL) != 0
		|| pthread_mutex_init(&data->state_mutex, NULL) != 0
		|| pthread_mutex_init(&data->table_lock, NULL) != 0
		|| pthread_cond_init(&data->table_cond, NULL) != 0)
		return (1);
	return (0);
}

static int	init_coders(t_data *data)
{
	int	i;

	data->coders = malloc(sizeof(t_coder) * data->nb_coders);
	if (!data->coders)
		return (1);
	i = 0;
	while (i < data->nb_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].compiles_count = 0;
		data->coders[i].last_compile_time = data->start_time;
		data->coders[i].left_dongle = &data->dongle[i];
		data->coders[i].right_dongle = &data->dongle[(i + 1)
			% data->nb_coders];
		data->coders[i].data = data;
		i++;
	}
	return (0);
}

int	init_simulation(t_data *data)
{
	data->simulation_stop = 0;
	data->fifo_counter = 0;
	data->start_time = get_current_time();
	if (init_dongles(data) != 0 || init_mutexes(data) != 0
		|| init_coders(data) != 0)
	{
		printf("Error: initialization failed.\n");
		return (1);
	}
	return (0);
}
