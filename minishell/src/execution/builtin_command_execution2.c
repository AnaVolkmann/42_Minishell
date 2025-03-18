/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_command_execution2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufiguei <lufiguei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:06:39 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/18 14:14:47 by lufiguei         ###   ########.fr       */
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

/**
 * @brief Executes a built-in command with piping support in a child process.
 * @param _cmd_ The command arguments.
 * @param _fd File descriptors for input and output.
 * @param env The environment structure.
 * @param pipe_data Pipe state information.
 * @return The status code of the executed command.
 */
int	execute_builtin_with_piping(t_ast_node *head, int *_fd, t_env *env, int *pipe_data)
{
	int				status;
	int				_out_fd[2];

	_out_fd[1] = 1;
	if (pipe_data[0] > 1)
		pipe(_out_fd);
	status = execute_builtin_command_in_child(head, env, _out_fd, pipe_data);
	if (pipe_data[0] > 1)
	{
		safe_close(_out_fd[1]);
		_fd[0] = _out_fd[0];
	}
	return (status);
}

/**
 * @brief Executes a built-in command with simple piping support in a child process.
 * @param _cmd_ The command arguments.
 * @param _fd File descriptors for input and output.
 * @param env The environment structure.
 * @param pipe_data Pipe state information.
 * @return The status code of the executed command.
 */
int	execute_builtin_with_simple_piping(
		t_ast_node *head, int *_fd, t_env *env, int *pipe_data)
{
	int				status;
	int				_out_fd[2];

	_out_fd[1] = 1;
	if (pipe_data[8] && pipe_data[7])
		_out_fd[1] = pipe_data[2];
	if (pipe_data[0] > 1 && (!pipe_data[8] || !pipe_data[7]))
		pipe(_out_fd);
	status = execute_builtin_command_in_child(head, env, _out_fd, pipe_data);
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
	return (status);
}

/**
 * @brief Manages the execution of a single built-in command with or without piping.
 *        Handles special cases like `exit` and dispatches to other execution functions.
 * @param cmd The command arguments.
 * @param _fd File descriptors for input and output.
 * @param env The environment structure.
 * @param pipe_data Pipe state information.
 * @return The status code of the executed command.
 */
int	manage_single_builtin_execution(
		t_ast_node *head, int *_fd, t_env *env, int *pipe_data)
{
	int				status;
	//int				ex_status;

	status = 0;
	// if (str_cmp(cmd[0], "exit", NULL))
	// {
	// 	ex_status = 0;
	// 	if (cmd[1] && cmd[2])
	// 		return (ft_putendl_fd("Minishell: exit: too many arguments.", 1), 1);
	// 	if (cmd[1] && !is_string_numeric(cmd[1]))
	// 	{
	// 		ft_putendl_fd("Minishell: exit: numeric argument required.", 1);
	// 		ex_status = 255;
	// 	}
	// 	else if (cmd[1])
	// 		ex_status = (ft_atoi(cmd[1]) % 256);
	// 	free_string_array(cmd);
	// 	ft_putendl_fd("EXIT", 1);
	// 	cleanup_and_exit_shell(env, ex_status);
	// }
	if (!pipe_data[8])
		status = execute_builtin_with_piping(head, _fd, env, pipe_data);
	else
		status = execute_builtin_with_simple_piping(head, _fd, env, pipe_data);
	return (status);
}
