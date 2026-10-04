/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 11:21:25 by abani-am          #+#    #+#             */
/*   Updated: 2026/10/04 12:48:59 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

static void	swap_requests(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_up(t_heap *heap, int i)
{
	int			parent;
	t_request	*itm;

	itm = heap->items;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (itm[parent].key < itm[i].key)
			break ;
		if (itm[parent].key == itm[i].key && itm[parent].seq < itm[i].seq)
			break ;
		swap_requests(&itm[parent], &itm[i]);
		i = parent;
	}
}

static void	sift_down(t_heap *heap, int i)
{
	int			left;
	int			right;
	int			smallest;
	t_request	*itm;

	itm = heap->items;
	left = i * 2 + 1;
	right = i * 2 + 2;
	smallest = i;
	if (left < heap->size && (itm[left].key < itm[smallest].key
			|| (itm[left].key == itm[smallest].key
				&& itm[left].seq < itm[smallest].seq)))
		smallest = left;
	if (right < heap->size && (itm[right].key < itm[smallest].key
			|| (itm[right].key == itm[smallest].key
				&& itm[right].seq < itm[smallest].seq)))
		smallest = right;
	if (smallest != i)
	{
		swap_requests(&itm[i], &itm[smallest]);
		sift_down(heap, smallest);
	}
}

void	scheduler_push(t_heap *heap, t_request req)
{
	if (heap->size >= heap->capacity)
		return ;
	heap->items[heap->size] = req;
	heap->size++;
	sift_up(heap, heap->size - 1);
}

t_request	scheduler_pop(t_heap *heap)
{
	t_request	top;

	if (heap->size == 0)
	{
		top.coder_id = -1;
		top.key = -1;
		top.seq = -1;
		return (top);
	}
	top = heap->items[0];
	heap->size--;
	heap->items[0] = heap->items[heap->size];
	sift_down(heap, 0);
	return (top);
}
