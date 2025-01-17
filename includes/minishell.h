/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otait-ta <otait-ta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 22:56:43 by hasserao          #+#    #+#             */
/*   Updated: 2023/06/19 15:51:06 by otait-ta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../ft_printf/ft_printf.h"
# include "../libft/libft.h"
# include "./doubly_lst.h"
# include "./env_var.h"
# include <stdio.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>

extern int			g_exit_status;

/*---MACROS---*/
# define FALSE 0
# define TRUE 1
# define PATH_MAX 1024 /* max bytes in pathname */

/* struct contain all cmnds in a linked list "cmds" and
all env variables in other linked list "env"*/
typedef struct s_exec_context
{
	t_doubly_lst	*cmds;
	t_env			*env;
	t_env_variable	*paths;
	int				end[2];
	pid_t			pid;
	char			**cmd_paths;
	int				is;
	t_list			*fds;
}					t_exec_context;

/*--- Redirections ---*/
typedef enum s_redir_t
{
	IN = 0,
	OUT = 1,
	APPEND = 2,
	HERE_DOC = 3,
	PIPE = 4,
}					t_redir_t;

/* fill the execContext with initial data*/
int					init_data(t_exec_context *ex_context, char **av,
						char **env_str);

/*---------------------------matrix------------------------------*/
/* iterate over a matrix and free every line */
void				free_matrix(char **matrix);
/* Counts the number of pointers in a `char **` array.
 Assumes that the array is terminated by a NULL pointer.*/
int					count_matrix(char **arr);
/* Prints out the contents of an array `matrix`,
 * with each row separated by the string `separator`*/
void				print_matrix(char **matrix, char separator);
/* Concatenates the rows of two matrices */
char				**matrix_concat(char **matrix, char **back);
/*Appends a new row to the end of the matrix*/
char				**matrix_push_back(char **matrix, char *back);
/*Appends a new row to the front of the matrix*/
char				**matrix_add_front(char *str, char **matrix);
/* Get last pointer in a matrix*/
char				*last_element_matrix(char **matrix);
/*alloc fd and return a pointer to it*/
int					*alloc_fd(int fd);
/*----------------------------------------------------------------*/

/*------------------------------STRING----------------------------*/
/*  equal == 0 || not = 1 */
int					ft_strcmp(char *str1, char *str2);

char				*ft_strcpy(char *dest, const char *src);

/*function return 0 if find in s1 the first occurence 
of any caracter in s2 else return 1*/
int					ft_strpbrk(char *s1, char *s2);
char				*ft_alpha(void);
/*Searches for the index of the first character in `str` 
that matches any character in `set`.*/
int					find_char_index(const char *str, const char *set);
/* change the value of quote depanding on its place*/
void				handle_quotes(int *s_quote, int *d_quote, char c);
/* if str start with c*/
char				start_with(char *str, char *set);
/* if str end with c*/
char				end_with(char *str, char *set);
/* put message and set exit_satatus*/
void				put_error(char *message, char *param, int status_code);
void				put_error_ex(char *message, char *param, char *suffix,
						int status_code);
void				ft_msg_error(char *message, int exit);
/* parse first element in exit*/
int					parse_arg(char *str);

char				*ft_strcat(char *dest, const char *src);

void				add_fd(t_exec_context *ex_context, int fd);
/*------------------------------------------------------------------*/

/*--------------------------PARSING-------------------------------------*/

/*parsing start here*/
int					pars_input(t_exec_context *ex_context, char *input);
/* split line given by new_line to multiple tokens*/
char				**split_space(char *line);
/*takes a string 'str' and splits it into multiple tokens 
based on the provided delimiters*/
void				line_to_tokens(char const *line, char *delimiters,
						char **tokens);
/* counts the number of words in a given string 'str'
 based on the provided 'delimiters'.*/
int					words_number(const char *str, const char *delimiters);
/*splite all tokens given by split_space() to sub 
tokens taking expand in consideration */
char				**split_tokens(char **tokens, t_exec_context *ex_context);
/* checks if token have a $ if so it expnded 
if not it return the token  */
char				*var_expand(char *token, t_exec_context *ex_context);
/* checks if token have a ~ if so it expnded 
if not it return the token  */
char				*path_expand(char *str, char *home);
/* * Retrieves the value of an environment
 variable by its name, taking care of {}.*/
char				*get_env_value(char *name,
						t_exec_context *ex_context);
/* open  the file with name in old_list command and asign the fd to node */
void				handle_input(t_exec_context *ex_context,
						t_doubly_lst *old_list,
						t_doubly_lst *node);
/* open  the file with name in old_list command and asign the fd to node */
void				handle_output(t_exec_context *ex_context,
						t_doubly_lst *old_list,
						t_doubly_lst *node);
/* open  the file with name in old_list command and asign the fd to node */
void				handle_append(t_exec_context *ex_context,
						t_doubly_lst *old_list,
						t_doubly_lst *node);
/* open  the file with name in old_list command and asign the fd to node */
void				handle_heredoc(t_doubly_lst *old_list, t_doubly_lst *node,
						t_exec_context *ex_context);
/* remove quotes from str*/
char				*remove_quotes(char *str);
/* expand token*/
char				*expand_token(char *token, t_exec_context *ex_context);
/* check syntax for every node in list*/
int					check_syntax(t_doubly_lst *head);
/*the start to the end of the token to expend*/
char				*subtoken(char *str, int i, t_exec_context *ex_context);
/* convert list to have args and in , out */
t_doubly_lst		*convert_list_format(t_doubly_lst *list,
						t_exec_context *ex_context);
/*count the word delimited by delimiters*/
int					words_number_delimiters(char *str, char *delimiters);
/* split lin to tokens by delimiters*/
void				line_to_tokens_delimiters(char *line, char *delimiters,
						char **tokens);

void				sigint_handler(int sig);
void				sigquit_handler(int sig);
void				heredoc_sigint_handler(int sig);
void				ft_signals(void);
void				ft_ign_signals(void);

char				**heredoc_befor(char *token);
int					ends_with_heredoc(char **matrix);
/*-------------------------------------------------------------------------*/

/*--------------------------EXEC-------------------------------------*/
////////////////////* Builtins*///////////////////////
void				ft_env(t_exec_context *ex_context);
void				ft_export(t_exec_context *ex_context);
void				ft_unset(t_exec_context *ex_context);
void				ft_cd(char **arg, t_env *env);
int					ft_pwd(void);
void				ft_exit(char **arg);
void				ft_echo(char **arg);
int					is_builtin(char *cmd);
void				exec_builtins(t_exec_context *ex_context);
char				*go_home(t_env *env);
char				*get_env_path(t_env *env, char *name);
void				update_pwd(t_env *env, char *name);
void				update_oldpwd(t_env *env, char *name, char *path);

void				_export_variable(char *arg, t_env *env);
/* copy env list */
t_env				*copy_env_list(t_exec_context *ex_context);
/*sort env variable list*/
void				sort_env_var(t_env_variable *head);
//////////////* Execution*///////////////////

/*fuction that execute builtins*/
/*get path from enviroment and split it*/
void				ft_get_path(t_exec_context *ex_context);
/*function that join the command with her path */
char				*ft_get_cmd_path(t_exec_context *ex_context);
void				ft_wait(int pid);
void				dup_pipe(t_exec_context *ex_context, int *k, int *end);
/*function that excute command in child process*/
void				ft_execute_child(t_exec_context *ex_context);
/*excute the program*/
void				execution(t_exec_context *ex_context);
/*fuction that duplicate in and out to stdin and stdout*/
void				ft_dup(t_doubly_lst *commend);
/*fuction that close file descriptor*/
void				ft_close_fd(t_exec_context *ex_context);
/*fuction that execute  child process*/
void				ft_child_process(t_exec_context *ex_context,
						int *k,
						int *end);
#endif
