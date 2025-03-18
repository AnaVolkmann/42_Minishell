/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution_adv.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufiguei <lufiguei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:06:46 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/18 17:23:23 by lufiguei         ###   ########.fr       */
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
 * @brief Executes a basic command without redirection or piping in a child process.
 *        Handles input/output redirection if piping is used.
 * @param cmd The command arguments.
 * @param _fd File descriptors for input and output.
 * @param env The environment variables.
 * @param pipe_data Pipe state information.
 * @return Returns 1 upon successful execution.
 */
int	execute_command_basic(char **cmd, int *_fd, char **env, int *pipe_data)
{
	pid_t				pid;
	int					fd_[2];

	pipe(fd_);
	pid = fork();
	signal(SIGINT, child_ctrl_c);
	signal(SIGQUIT, child_ctrl_c);
	if (!pid)
	{
		if (pipe_data[0] && pipe_data[0] <= pipe_data[5])
			dup2(_fd[0], 0);
		if (pipe_data[0] > 1)
			dup2(fd_[1], 1);
		else
			safe_close(_fd[0]);
		close_pipe_ends(fd_[0], fd_[1]);
		execve(cmd[0], cmd, env);
		(ft_putendl_fd(strerror(errno), 2), exit(127));
	}
	close_pipe_ends(fd_[1], _fd[0]);
	if (pipe_data[0] > 1)
		_fd[0] = fd_[0];
	else
		safe_close(fd_[0]);
	return (1);
}

/**
 * @brief Executes a command with input/output redirection in a child process.
 *        Manages file descriptors for redirection and piping.
 * @param cmd The command arguments.
 * @param _fd File descriptors for input and output.
 * @param env The environment variables.
 * @param pipe_data Pipe state information.
 * @return Returns 1 upon successful execution.
 */
int	execute_command_with_redirection(
		char **cmd, int *_fd, char **env, int *pipe_data)
{
	pid_t				pid;
	int					fd_[2];

	pipe(fd_);
	pid = fork();
	signal(SIGINT, child_ctrl_c);
	signal(SIGQUIT, child_ctrl_c);
	if (!pid)
	{
		child_fds_managment(pipe_data, _fd, fd_);
		execve(cmd[0], cmd, env);
		ft_putendl_fd(strerror(errno), 2);
		exit(127);
	}
	parent_fds_managment(pipe_data, _fd, fd_);
	return (1);
}

/**
 * @brief Prepares command arguments and executes a built-in or external command.
 *        Handles piping and redirection based on the command.
 * @param cmd The command arguments.
 * @param _fd File descriptors for input and output.
 * @param pipe_data Pipe state information.
 * @param env The environment structure.
 * @return The status of the executed command.
 */
int	prepare_and_execute_command(
		t_ast_node *head, int *_fd, int *pipe_data, t_env *env)
{
	char				**cmd_args;
	char				**f_args;
	int					status;

	f_args = prepare_cmd_arguments(head->args[0], env->original_env, 0);
	cmd_args = merge_command_args(f_args, head->args);
	if (check_if_command_is_builtin(cmd_args[0]))
	{
		if (ft_strncmp(cmd_args[0], "exit", 5) == 0)
			free_string_array(cmd_args);
		status = manage_builtin_execution(head, _fd, env, pipe_data);
	}
	else
	{
		pipe_data[10] += 1;
		if (!pipe_data[8])
			status = execute_command_basic(cmd_args, _fd, env->original_env, pipe_data);
		else
		status = execute_command_with_redirection(cmd_args, _fd, env->original_env, pipe_data);
	}
	free_string_array(cmd_args);
	if (pipe_data[0] > 1)
		pipe_data[0] -= 1;
	return (status);
}

/**
 * @brief Waits for the child processes to finish and handles signals accordingly.
 * @param status The status code of the child process.
 * @param pipe_data Pipe state information.
 * @return The exit status of the last child process or a signal value.
 */
int	wait_for_children(int status, int *pipe_data)
{
	if (status != 2 && status != 127
		&& status != 126 && pipe_data[10]
		&& pipe_data[11])
	{
		while (pipe_data[10])
		{
			wait(&status);
			pipe_data[10] -= 1;
		}
		signal(SIGINT, handle_ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		if (!g_signal)
			return (WEXITSTATUS(status));
		else
			return (g_signal);
	}
	return (status);
}
