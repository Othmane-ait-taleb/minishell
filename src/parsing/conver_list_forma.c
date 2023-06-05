/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conver_list_forma.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 18:43:12 by otait-ta          #+#    #+#             */
/*   Updated: 2023/06/04 19:46:52 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	create_node(t_doubly_lst **node, t_doubly_lst **head,
		t_doubly_lst **list)
{
	t_doubly_lst	*prev_node;

	prev_node = NULL;
	if (ft_strcmp((*list)->cmd, "|") && (!(*list)->prev || ((*list)->prev
				&& !ft_strcmp((*list)->prev->cmd, "|"))))
	{
		if (find_char_index((*list)->cmd, "><") >= 0)
			(*node) = d_lstnew("");
		else
			(*node) = d_lstnew((*list)->cmd);
		if (!*head)
			*head = (*node);
		if (prev_node)
		{
			prev_node->next = *node;
			(*node)->prev = prev_node;
		}
		prev_node = (*node);
		if (find_char_index((*list)->cmd, "<>") < 0)
			(*list) = (*list)->next;
	}
}
int	redirection_type(t_doubly_lst *list)
{
	if (!ft_strcmp(list->cmd, ">") && !ft_strcmp(list->next->cmd, ">")
		&& !list->have_quotes)
		return (APPEND);
	else if (!ft_strcmp(list->cmd, "<") && !ft_strcmp(list->next->cmd, "<")
			&& !list->have_quotes)
		return (HERE_DOC);
	else if (!ft_strcmp(list->cmd, ">") && !list->have_quotes)
		return (OUT);
	else if (!ft_strcmp(list->cmd, "<") && !list->have_quotes)
		return (IN);
	return (-1);
}

void	fill_in_out(t_doubly_lst **list, t_doubly_lst **node,
		t_exec_context *exContext)
{
	int	type;

	type = redirection_type((*list));
	if (type == APPEND)
		handle_append((*list), *node);
	else if (type == HERE_DOC)
		handle_heredoc((*list), *node, exContext);
	else if (type == OUT)
		handle_output((*list), *node);
	else if (type == IN)
		handle_input((*list), *node);
	if (type == APPEND || type == HERE_DOC)
		(*list) = (*list)->next->next->next;
	else
		(*list) = (*list)->next->next;
}

t_doubly_lst	*convert_list_format(t_doubly_lst *list,
									t_exec_context *exContext)
{
	t_doubly_lst	*node;
	t_doubly_lst	*head;

	head = NULL;
	node = NULL;
	while (list)
	{
		create_node(&node, &head, &list);
		while (list && ft_strcmp(list->cmd, "|") && node)
		{
			if (find_char_index(list->cmd, "><") >= 0)
			{
				fill_in_out(&list, &node, exContext);
				continue ;
			}
			if (node->cmd[0] == '\0')
				node->cmd = ft_strdup(list->cmd);
			else
				node->args = matrix_push_back(node->args, list->cmd);
			list = list->next;
		}
		if (list && !ft_strcmp(list->cmd, "|"))
			list = list->next;
	}
	return (head);
}