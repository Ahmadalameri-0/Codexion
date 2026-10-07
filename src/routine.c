/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 18:02:36 by abani-am          #+#    #+#             */
/*   Updated: 2026/10/04 12:49:13 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

void	print_action(t_coder *coder, char *action)
{
	long long	time_now;

	pthread_mutex_lock(&coder->data->state_mutex);
	if (coder->data->simulation_stop == 1)
	{
		pthread_mutex_unlock(&coder->data->state_mutex);
		return ;
	}
	pthread_mutex_unlock(&coder->data->state_mutex);
	pthread_mutex_lock(&coder->data->write_mutex);
	time_now = get_time_elapsed(coder->data->start_time);
	printf("Milliseconds: %3lldms | Coder ID: %d %s\n",
		time_now, coder->id, action);
	pthread_mutex_unlock(&coder->data->write_mutex);
}

static int	is_stopped(t_coder *coder)
{
	int stop;

	pthread_mutex_lock(&coder->data->state_mutex);
	stop = coder->data->simulation_stop;
	pthread_mutex_unlock(&coder->data->state_mutex);
	return (stop);
}

static void	take_one(t_dongle *dongle, t_coder *coder, t_request req)
{
	long long	rem;

	pthread_mutex_lock(&dongle->lock);
	scheduler_push(&dongle->queue, req);
	while (!is_stopped(coder))
	{
		while (!is_stopped(coder) && (dongle->taken
				|| dongle->queue.items[0].coder_id != coder->id))
			pthread_cond_wait(&dongle->cond, &dongle->lock);
		if (is_stopped(coder))
			break ;
		rem = dongle->last_used_time + coder->data->dongle_cooldown
			- get_current_time();
		if (rem <= 0)
			break ;
		pthread_mutex_unlock(&dongle->lock);
		ft_usleep(rem, coder->data);
		pthread_mutex_lock(&dongle->lock);
	}
	if (!is_stopped(coder))
	{
		scheduler_pop(&dongle->queue);
		dongle->taken = 1;
	}
	pthread_mutex_unlock(&dongle->lock);
	if (!is_stopped(coder))
		print_action(coder, "has taken a dongle");
}

static void	acquire_dongles(t_coder *coder)
{
	t_request	req;

	req.coder_id = coder->id;
	pthread_mutex_lock(&coder->data->state_mutex);
	req.seq = coder->data->fifo_counter++;
	if (coder->data->scheduler == FIFO)
		req.key = req.seq;
	else
		req.key = coder->last_compile_time + coder->data->time_to_burnout;
	pthread_mutex_unlock(&coder->data->state_mutex);
	if (coder->left_dongle->id < coder->right_dongle->id)
		take_one(coder->left_dongle, coder, req);
	else
		take_one(coder->right_dongle, coder, req);
	if (coder->data->nb_coders > 1)
	{
		if (coder->left_dongle->id < coder->right_dongle->id)
			take_one(coder->right_dongle, coder, req);
		else
			take_one(coder->left_dongle, coder, req);
	}
}

static void	work_routine(t_coder *coder)
{
	print_action(coder, "is compiling");
	pthread_mutex_lock(&coder->data->state_mutex);
	coder->last_compile_time = get_current_time();
	coder->compiles_count++;
	pthread_mutex_unlock(&coder->data->state_mutex);
	ft_usleep(coder->data->time_to_compile, coder->data);
	pthread_mutex_lock(&coder->left_dongle->lock);
	coder->left_dongle->last_used_time = get_current_time();
	coder->left_dongle->taken = 0;
	pthread_cond_broadcast(&coder->left_dongle->cond);
	pthread_mutex_unlock(&coder->left_dongle->lock);
	pthread_mutex_lock(&coder->right_dongle->lock);
	coder->right_dongle->last_used_time = get_current_time();
	coder->right_dongle->taken = 0;
	pthread_cond_broadcast(&coder->right_dongle->cond);
	pthread_mutex_unlock(&coder->right_dongle->lock);
	print_action(coder, "is debugging");
	ft_usleep(coder->data->time_to_debug, coder->data);
	print_action(coder, "is refactoring");
	ft_usleep(coder->data->time_to_refactor, coder->data);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		ft_usleep(10, coder->data);
	while (!is_stopped(coder))
	{
		acquire_dongles(coder);
		if (is_stopped(coder))
			break ;
		if (coder->data->nb_coders == 1)
		{
			ft_usleep(coder->data->time_to_burnout, coder->data);
			break ;
		}
		work_routine(coder);
	}
	return (NULL);
}
