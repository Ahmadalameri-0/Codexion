/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 18:02:36 by abani-am          #+#    #+#             */
/*   Updated: 2026/10/11 08:47:35 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

void	print_action(t_coder *coder, char *action)
{
	if (is_stopped(coder))
		return ;
	pthread_mutex_lock(&coder->data->write_mutex);
	printf("Milliseconds: %3lldms | Coder ID: %d %s\n",
		get_time_elapsed(coder->data->start_time), coder->id, action);
	pthread_mutex_unlock(&coder->data->write_mutex);
}

static long long	start_slot(t_coder *coder)
{
	t_data		*data;
	long long	slot;
	long long	pos;

	data = coder->data;
	if (data->scheduler != EDF || data->nb_coders < 2)
		return (0);
	slot = data->time_to_compile + data->dongle_cooldown;
	if (data->nb_coders % 2 == 0)
		return (((coder->id - 1) % 2) * slot);
	pos = ((data->nb_coders - coder->id) * ((data->nb_coders + 1) / 2))
		% data->nb_coders;
	return (pos * slot / (data->nb_coders / 2));
}

static int	acquire_dongles(t_coder *coder)
{
	t_request	req;

	req.coder_id = coder->id;
	pthread_mutex_lock(&coder->data->state_mutex);
	req.seq = coder->data->fifo_counter++;
	if (coder->data->scheduler == FIFO)
		req.key = req.seq;
	else
	{
		req.key = coder->last_compile_time + coder->data->time_to_burnout;
		req.seq += (coder->compiles_count + 1) * 1000000000LL;
		if (coder->compiles_count == 0)
			req.seq = start_slot(coder);
	}
	pthread_mutex_unlock(&coder->data->state_mutex);
	if (!take_dongles(coder, req))
		return (0);
	print_action(coder, "has taken a dongle");
	if (coder->left_dongle != coder->right_dongle)
		print_action(coder, "has taken a dongle");
	return (1);
}

static void	work_routine(t_coder *coder)
{
	print_action(coder, "is compiling");
	pthread_mutex_lock(&coder->data->state_mutex);
	coder->last_compile_time = get_current_time();
	coder->compiles_count++;
	pthread_mutex_unlock(&coder->data->state_mutex);
	ft_usleep(coder->data->time_to_compile, coder->data);
	release_dongles(coder);
	print_action(coder, "is debugging");
	ft_usleep(coder->data->time_to_debug, coder->data);
	print_action(coder, "is refactoring");
	ft_usleep(coder->data->time_to_refactor, coder->data);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	coder->start_at = coder->data->start_time + start_slot(coder);
	while (!is_stopped(coder))
	{
		if (!acquire_dongles(coder))
			break ;
		if (coder->left_dongle == coder->right_dongle)
		{
			ft_usleep(coder->data->time_to_burnout, coder->data);
			break ;
		}
		work_routine(coder);
	}
	return (NULL);
}
