/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crud_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 16:53:42 by otait-ta          #+#    #+#             */
/*   Updated: 2023/05/21 18:43:09 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	delete_env_elem(t_env *list, t_env_variable *elem)
{
	if (elem != NULL)
	{
		if (elem->prev != NULL)
			elem->prev->next = elem->next;
		if (elem->next != NULL)
			elem->next->prev = elem->prev;
		free(elem->content);
		free(elem->name);
		free(elem);
		list->size--;
		elem = NULL;
	}
}

t_env_variable	*search_env_elem(t_env *env, char *name)
{
	t_env_variable	*match;

	match = env->first;
	while (match != NULL && ft_strcmp(match->name, name) != 0)
		match = match->next;
	if (match && ft_strcmp(match->name, name) != 0)
		return (NULL);
	return (match);
}

t_env_variable	*create_env_elem(char *pair)
{
	t_env_variable	*new;
	int				equal_index;

	new = ft_calloc(sizeof(t_env_variable), 1);
	if (!new)
		return (NULL);
	equal_index = ft_strchr(pair, '=') - pair;
	new->name = ft_substr(pair, 0, equal_index);
	if (!new->name)
		return (NULL);
	new->content = ft_substr(pair, equal_index + 1, ft_strlen(pair)
			- equal_index);
	if (!new->content)
		return (free(new->name), NULL);
	return (new);
}

void	update_env_elem(t_env *env, char *name, char *new_content)
{
	t_env_variable *match;
	char *new_pair;

	match = search_env_elem(env, name);
	if (match)
		delete_env_elem(env, match);
	new_pair = str_to_env_form(name, new_content);
	match = create_env_elem(new_pair);
	set_env_elem(env, match);
	free(new_pair);
}