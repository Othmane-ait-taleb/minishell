/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_lstdelone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 09:17:31 by otait-ta          #+#    #+#             */
/*   Updated: 2023/06/10 10:34:26 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	d_lstdelone(t_doubly_lst **head, t_doubly_lst *nodeToDelete)
{
	if (*head == NULL || nodeToDelete == NULL)
		return ;
	if (*head == nodeToDelete)
		*head = nodeToDelete->next;
	if (nodeToDelete->prev != NULL)
		nodeToDelete->prev->next = nodeToDelete->next;
	if (nodeToDelete->next != NULL)
		nodeToDelete->next->prev = nodeToDelete->prev;
	free(nodeToDelete);
}
