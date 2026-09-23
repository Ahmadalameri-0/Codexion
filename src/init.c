/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:47:46 by abani-am          #+#    #+#             */
/*   Updated: 2026/09/21 16:07:19 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

static int	init_dongles_and_mutexes(t_data *data)
{
	int i;

	data->dongle = malloc(sizeof(t_dongle) * data->nb_coders);
	if (!data->dongle)
		return (1);

	i = 0;
	while (i < data->nb_coders)
	{
		data->dongle[i].id = i;
		data->dongle[i].last_used_time = 0;
		if(pthread_mutex_init(&data->dongle[i].lock, NULL) != 0)
			return (1);
		i++;
	}

	if (pthread_mutex_init(&data->write_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->state_mutex, NULL) != 0)
		return (1);
	return (0);
}
static int	init_coders(t_data *data)
{
	int i;

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
		data->coders[i].right_dongle = &data->dongle[(i + 1) % data->nb_coders];
		data->coders[i].data = data;
		i++;
	}
	return (0);
}

int	init_simulation(t_data *data)
{
	data->simulation_stop = 0;
	data->start_time = get_current_time();
	if (init_dongles_and_mutexes(data) != 0)
	{
		printf("Error: Failed to initialize dongles and mutexs.\n");
		return (1);
	}

	if (init_coders(data) != 0)
	{
		printf("Error: Failed to initialize coders.\n");
		return (1);
	}
	return (0);
}