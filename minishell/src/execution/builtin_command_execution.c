/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_command_execution.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufiguei <lufiguei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:23:56 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/25 15:31:30 by lufiguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 		ARRAY GUIDE
		piped[0]: executed_pipes_index
		piped[1]: current_input_fd/		
		piped[2]: current_output_fd
		piped[3]: input_files_count
		piped[4]: output_files count
		piped[5]: pipe_count
		piped[6]: has_input_file
		piped[7]: has_output_file		
		piped[8]: is_redirect_or_pipe
		piped[9]: heredoc_status
		piped[10]: children_count
		piped[11]: second_heredoc_status */

void	handle_pipe_redirection(int *_fd, int *pipe_data, int fd_[2])
{
	if (pipe_data[0] && pipe_data[0] <= pipe_data[5])
	{
		dup2(_fd[0], 0);
	}
	if (pipe_data[0] > 1)
	{
		dup2(fd_[1], 1);
	}
	else
	{
		safe_close(_fd[0]);
	}
}

/** @brief Handles child process execution for built-in 
 * commands with optional piping.
 * @param cmd The command arguments.
 * @param _fd File descriptors for input and output.
 * @param env The environment structure.
 * @param pipe_data Pipe state information.
 * @return 1 if the child process was executed successfully.*/
int	simple_child_for_builtins(t_ast_node *head,
			int *_fd, t_env *env, int *pipe_data)
{
	pid_t					pid;
	int						fd_[2];
	int						_out_fd_[2];
	int						status;

	_out_fd_[1] = 1;
	pipe(fd_);
	pid = fork();
	if (!pid)
	{
		handle_pipe_redirection(&_fd[0], pipe_data, fd_);
		close_pipe_ends(fd_[0], fd_[1]);
		dup2(_out_fd_[1], 1);
		status = execute_builtin_command_in_child(head,
				env, _out_fd_, pipe_data);
		free_ast_child(head);
		cleanup_and_exit_shell(env, status);
	}
	close_pipe_ends(fd_[1], _fd[0]);
	if (pipe_data[0] > 1)
		_fd[0] = fd_[0];
	else
		safe_close(fd_[0]);
	return (1);
}

/** @brief Executes a built-in command in a child process with
 *  redirection support.
 * @param cmd The command arguments.
 * @param _fd File descriptors for input and output.
 * @param env The environment structure.
 * @param pipe_data Pipe state information.
 * @return 1 if the child process was executed successfully.*/
int	execute_child_with_redirections(t_ast_node *head,
		int *_fd, t_env *env, int *pipe_data)
{
	pid_t			pid;
	int				_out_fd[2];

	_out_fd[1] = 1;
	if (pipe_data[8] && pipe_data[7])
		_out_fd[1] = pipe_data[2];
	if (pipe_data[0] > 1 && (!pipe_data[8] || !pipe_data[7]))
		pipe(_out_fd);
	pid = fork();
	if (!pid)
		exec_builtin_and_exit(head, env, _out_fd, pipe_data);
	if (pipe_data[8] && pipe_data[7])
	{
		safe_close(_out_fd[1]);
		pipe_data[7] = 0;
	}
	if (!pipe_data[6] && !pipe_data[7])
		pipe_data[8] = 0;
	if (pipe_data[0] > 1 && (!pipe_data[8] || !pipe_data[7]))
	{
		safe_close(_out_fd[1]);
		_fd[0] = _out_fd[0];
	}
	return (1);
}

/** @brief Checks if a string consists of numeric characters only.
 * @param s_1 The string to check.
 * @return 1 if the string is numeric, 0 otherwise.*/
int	is_string_numeric(char *s_1)
{
	int				a;
	int				len;

	a = 0;
	len = ft_strlen(s_1);
	if (len > 17)
		return (0);
	while (s_1[a] == ' ')
		a++;
	if (s_1[0] == '-' || s_1[0] == '+')
		a++;
	while (s_1[a])
	{
		if (!ft_isdigit(s_1[a]))
			return (0);
		a++;
	}
	return (1);
}

/** @brief Manages the execution of built-in commands, including 
 * handling piping and redirection.
 * @param _cmd_ The command arguments.
 * @param _fd File descriptors for input and output.
 * @param env The environment structure.
 * @param pipe_data Pipe state information.
 * @return The status code of the executed command.*/
int	manage_builtin_execution(t_ast_node *head,
		int *_fd, t_env *env, int *pipe_data)
{
	int				status;

	status = 0;
	pipe_data[10] += 1;
	if (pipe_data[0])
	{
		if (!pipe_data[8])
			status = simple_child_for_builtins(head, _fd, env, pipe_data);
		else
			status = execute_child_with_redirections(head, _fd, env, pipe_data);
	}
	else
		status = manage_single_builtin_execution(head, _fd, env, pipe_data);
	return (status);
}
