/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 17:00:57 by otait-ta          #+#    #+#             */
/*   Updated: 2023/04/08 17:50:01 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_start_vars(t_exec_context *exContext)
{
	char	*tmp;

	// char	*tmp2;
	tmp = getcwd(NULL, 0);
	update_env_elem(exContext->env, "PWD", tmp);
}

int	init_data(t_exec_context *exContext, char **av, char **env_str)
{
	(void)av;
	exContext->cmds = NULL;
	exContext->env = env_to_list(env_str);
	set_start_vars(exContext);
	return (0);
}