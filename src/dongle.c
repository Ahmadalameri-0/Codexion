/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 12:03:38 by abani-am          #+#    #+#             */
/*   Updated: 2026/10/10 12:13:34 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

static int	is_my_turn(t_coder *coder, t_dongle *dongle)
{
	return (!dongle->taken && dongle->queue.size > 0
		&& dongle->queue.items[0].coder_id == coder->id);
}

static long long	cooldown_left(t_coder *coder)
{
	long long	left;
	long long	right;

	left = coder->left_dongle->last_used_time;
	right = coder->right_dongle->last_used_time;
	if (right > left)
		left = right;
	left += coder->data->dongle_cooldown - get_current_time();
	right = coder->start_at - get_current_time();
	if (right > left)
		left = right;
	if (left < 0)
		return (0);
	return (left);
}

static int	wait_turn(t_coder *coder)
{
	long long	rem;

	rem = -1;
	while (!is_stopped(coder))
	{
		rem = -1;
		if (is_my_turn(coder, coder->left_dongle)
			&& is_my_turn(coder, coder->right_dongle))
			rem = cooldown_left(coder);
		if (rem == 0)
			return (1);
		if (rem > 0)
			timed_wait(coder->data, rem);
		else
			pthread_cond_wait(&coder->data->table_cond,
				&coder->data->table_lock);
	}
	return (0);
}

int	take_dongles(t_coder *coder, t_request req)
{
	int	ok;

	pthread_mutex_lock(&coder->data->table_lock);
	scheduler_push(&coder->left_dongle->queue, req);
	if (coder->right_dongle != coder->left_dongle)
		scheduler_push(&coder->right_dongle->queue, req);
	pthread_cond_broadcast(&coder->data->table_cond);
	ok = wait_turn(coder);
	if (ok)
	{
		scheduler_pop(&coder->left_dongle->queue);
		coder->left_dongle->taken = 1;
		if (coder->right_dongle != coder->left_dongle)
			scheduler_pop(&coder->right_dongle->queue);
		coder->right_dongle->taken = 1;
		coder->start_at = 0;
	}
	pthread_mutex_unlock(&coder->data->table_lock);
	return (ok);
}

void	release_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->table_lock);
	coder->left_dongle->last_used_time = get_current_time();
	coder->right_dongle->last_used_time = get_current_time();
	coder->left_dongle->taken = 0;
	coder->right_dongle->taken = 0;
	pthread_cond_broadcast(&coder->data->table_cond);
	pthread_mutex_unlock(&coder->data->table_lock);
}
