/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasserao <hasserao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 02:32:00 by otait-ta          #+#    #+#             */
/*   Updated: 2023/06/08 21:55:42 by hasserao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status;

int	main(int ac, char **av, char **env)
{
	t_exec_context	exContext;
	char			*input;

	(void)ac;
	if (init_data(&exContext, av, env))
		exit(1);
	while (1)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, sigquit_handler);
		input = readline("minishell $ ");
		if (!input)
			exit(g_exit_status);
		if (input[0] != '\0')
			add_history(input);
		if (pars_input(&exContext, input))
			continue ;
		execution(&exContext);
		d_lstclear(&exContext.cmds);
	}
	return (0);
}
