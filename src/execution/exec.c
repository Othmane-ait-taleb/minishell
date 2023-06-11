/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:48:35 by hasserao          #+#    #+#             */
/*   Updated: 2023/06/11 18:44:12 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	one_cmd(t_exec_context *exContext)
{
	ft_get_path(exContext);
	exContext->pid = fork();
	if (exContext->pid == -1)
		ft_msg_error("fork", 1);
	if (exContext->pid == 0)
	{
		ft_dup(exContext->cmds);
		ft_execute_child(exContext, exContext->cmds);
	}
}

int	mutiple_cmd(t_exec_context *exContext, t_doubly_lst *commend, int *k)
{
	int	end[2];
	int	pid;

	if (pipe(end) == -1)
		ft_msg_error("pipe", 1);
	pid = fork();
	if (pid == -1)
		ft_msg_error("fork", 1);
	if (pid == 0)
		ft_child_process(exContext, commend, k, end);
	else
	{
		if (*k)
			close(*k);
		*k = dup(end[0]);
		close(end[1]);
		close(end[0]);
	}
	return (pid);
}

void	execution(t_exec_context *exContext)
{
	int				size;
	t_exec_context	*tmp;
	t_doubly_lst	*cmds;
	int				k;
	int				fdout;
	int				fdin;
	int				pid;
	struct stat		fileStat;
	t_doubly_lst	*cmd_tmp;

	k = 0;
	tmp = exContext;
	cmds = tmp->cmds;
	while (cmds)
	{
		if (ft_strchr(cmds->cmd, '/'))
		{
			stat(cmds->cmd, &fileStat);
			if (S_ISDIR(fileStat.st_mode))
			{
				(put_error_ex("minishell: ", cmds->cmd, ": is a directory\n",
							126));
				d_lstdelone(&(tmp->cmds), cmds);
			}
			else if (access(cmds->cmd, F_OK) == -1)
			{
				(put_error_ex("minishell: ", cmds->cmd,
							": No such file or directory\n", 127));
				d_lstdelone(&(tmp->cmds), cmds);
			}
		}
		cmds = cmds->next;
	}
	size = d_lstsize(exContext->cmds);
	exContext->pipe_num = size - 1;
	if (size == 1)
	{
		if (is_builtin(exContext->cmds->cmd))
		{
			fdout = dup(1);
			fdin = dup(0);
			ft_dup(exContext->cmds);
			exec_builtins(exContext, exContext->cmds);
			dup2(fdout, 1);
			dup2(fdin, 0);
		}
		else
		{
			one_cmd(exContext);
			wait(NULL);
			free_matrix(exContext->cmd_paths);
		}
	}
	else
	{
		cmd_tmp = exContext->cmds;
		while (cmd_tmp)
		{
			pid = mutiple_cmd(tmp, cmd_tmp, &k);
			cmd_tmp = cmd_tmp->next;
		}
		waitpid(pid, NULL, 0);
		while (wait(NULL) != -1)
			;
	}
	ft_close_fd(exContext);
}
