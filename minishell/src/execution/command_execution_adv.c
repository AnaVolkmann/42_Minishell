/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution_adv.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:24:03 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/24 22:01:55 by ana-lda-         ###   ########.fr       */
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
 * @brief Executes a basic command without redirection or piping
in a child process. Handles input/output redirection if piping is used.
 * @param cmd The command arguments.
 * @param _fd File descriptors for input and output.
 * @param env The environment variables.
 * @param pipe_data Pipe state information.
 * @return Returns 1 upon successful execution.
 */
int	exec_cmd_basic(char **cmd, int *_fd, t_env *env, int *pipe_data, t_ast_node *head)
{
	pid_t				pid;
	int					fd_[2];

	pipe(fd_);
	pid = fork();
	signal(SIGINT, child_ctrl_c);
	signal(SIGQUIT, child_ctrl_c);
	if (!pid)
	{
		if (!cmd[0] || cmd[0][0] == '\0')
		{
			free_string_array(cmd);
			free_ast_child(head);
			cleanup_and_exit_shell(env, 0);
		}
		if (pipe_data[0] && pipe_data[0] <= pipe_data[5])
			dup2(_fd[0], 0);
		if (pipe_data[0] > 1)
			dup2(fd_[1], 1);
		else
			safe_close(_fd[0]);
		close_pipe_ends(fd_[0], fd_[1]);
		if (execve(cmd[0], cmd, env->original_env) < 0)
		{
			ft_putendl_fd("Command not found.", 2);
			free_string_array(cmd);
			free_ast_child(head);
			cleanup_and_exit_shell(env, 127);
		}
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
int	exec_cmd_w_redir(
		char **cmd, int *_fd, t_env *env, int *pipe_data, t_ast_node *head)
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
		execve(cmd[0], cmd, env->original_env);
		ft_putendl_fd(strerror(errno), 2);
		free_ast_child(head);
		cleanup_and_exit_shell(env, 127);
	}
	parent_fds_managment(pipe_data, _fd, fd_);
	return (1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

/* int	check_if_command_is_builtin2(char *_cmd)
{
	char	*builtins[] = {"echo", "cd", "pwd",
		"export", "unset", "env", "exit", NULL};;
	int	i;

	i = 0;
	while (builtins[i] != NULL)
	{
		if (ft_strcmp(_cmd, builtins[i]) == 0)
		{
			return (1);
		}
		i++;
	}
	return (0);
} */
int	check_if_command_is_builtin2(char *_cmd)
{
	char	**builtins;
	int		i;
	int		status;

	builtins = (char *[]){"echo", "cd",
		"pwd", "export", "unset", "env", "exit", NULL};
	if (!builtins)
		return (0);
	i = 0;
	status = 0;
	while (builtins[i] != NULL)
	{
		if (ft_strcmp(_cmd, builtins[i]) == 0)
			status = 1;
		i++;
	}
	return (status);
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
	int					stat;

	f_args = prepare_cmd_arguments(head->args[0], env->original_env, 0);
	cmd_args = merge_command_args(f_args, head->args);
	if (check_if_command_is_builtin2(cmd_args[0]))
	{
		free_string_array(cmd_args);
		stat = manage_builtin_execution(head, _fd, env, pipe_data);
	}
	else
	{
		pipe_data[10] += 1;
		if (!pipe_data[8])
			stat = exec_cmd_basic(cmd_args, _fd, env, pipe_data, head);
		else
			stat = exec_cmd_w_redir(cmd_args,
					_fd, env, pipe_data, head);
		free_string_array(cmd_args);
	}
	if (pipe_data[0] > 1)
		pipe_data[0] -= 1;
	return (stat);
}
