/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution_control.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:06:49 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 21:46:11 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes or resets the pipe state based on the value of `f`.
 *        Resets specific pipe states to 0 or increments the pipe state.
 * @param pipe_data The pipe state array.
 * @param f Flag indicating whether to reset or increment pipe state.
 */
void	initialize_or_reset_pipe_state(int *pipe_data, int f)
{
	int			a;

	a = 0;
	pipe_data[0] = pipe_data[5];
	if (f)
		while (a < 12)
			pipe_data[a++] = 0;
	else if (pipe_data[5])
		pipe_data[0] += 1;
	pipe_data[11] = 1;
}

/**
 * @brief Switches or closes file descriptors based on conditions.
 *        Updates the pipe state to reflect the changes in file descriptors.
 * @param pipe_data The pipe state array.
 * @param index The index of the pipe state to modify.
 * @param index_2 The index of the pipe state to close.
 * @param con A condition to either open or close the file descriptors.
 * @return Returns 1 on successful execution.
 */
int	switch_fds_identifier(
		int *pipe_data, int index, int index_2, int con)
{
	if (con)
	{
		if (pipe_data[index])
			close(pipe_data[index_2]);
		pipe_data[index] = 1;
	}
	else
	{
		ft_putendl_fd("err: file not found", 2);
		pipe_data[6] = 0;
	}
	return (1);
}

/**
 * @brief Opens a file for redirection based on the file type.
 *        Handles reading from a file, appending, and writing to a file.
 * @param head AST node containing redirection information.
 * @param pipe_data The pipe state array.
 * @param env The environment variables.
 * @param status The current execution status.
 * @return The updated status after attempting to open the file.
 */
int	open_file_for_redirection(
		t_ast_node *head, int *pipe_data, t_env *env, int status)
{
	int			mode;

	if (head->file_type == READ_FILE)
	{
		switch_fds_identifier(pipe_data, 6, 1, 1);
		pipe_data[1] = open(head->args[0], O_RDONLY);
		if (pipe_data[1] < 0)
			status = switch_fds_identifier(pipe_data, 0, 0, 0);
	}
	else if (head->file_type == READ_FROM_APPEND)
	{
		switch_fds_identifier(pipe_data, 6, 1, 1);
		status = exec_here_doc(head->args[0], pipe_data, env);
		signal(SIGINT, handle_ctrl_c);
	}
	else
	{
		switch_fds_identifier(pipe_data, 7, 2, 1);
		mode = O_TRUNC;
		if (head->file_type == WRITE_FILE_APPEND)
			mode = O_APPEND;
		pipe_data[2] = open(head->args[0], O_WRONLY | O_CREAT | mode, 0666);
	}
	return (status);
}

/**
 * @brief Checks if the given command is a built-in shell command.
 * @param _cmd The command to check.
 * @return 1 if it's a built-in command, 0 otherwise.
 */
int	check_if_command_is_builtin(char *_cmd)
{
	char		*tmp_cmd;
	int			status;

	status = 0;
	tmp_cmd = malloc(sizeof_str(_cmd, ' ') + 1);
	if (!tmp_cmd)
		return (0);
	s_strcopy(tmp_cmd, _cmd, 0, sizeof_str(_cmd, ' '));
	if (str_cmp(tmp_cmd, "echo", "cd")
		|| str_cmp(tmp_cmd, "pwd", "export")
		|| str_cmp(tmp_cmd, "unset", "env")
		|| str_cmp(tmp_cmd, "exit", NULL))
		status = 1;
	free(tmp_cmd);
	return (status);
}

/**
 * @brief Executes a built-in shell command in a child process.
 *        Handles commands like echo, cd, pwd, export, unset, and exit.
 * @param cmd The command arguments.
 * @param env The environment variables.
 * @param _out_fd The output file descriptor.
 * @param pipe_data Pipe state information.
 * @return The status of the executed command.
 */
int	execute_builtin_command_in_child(
		char **cmd, t_env *env, int *_out_fd, int *pipe_data)
{
	int			status;

	status = 0;
	(void)pipe_data;
	if (str_cmp(cmd[0], "echo", NULL))
		status = ft_echo(cmd, _out_fd);
	else if (str_cmp(cmd[0], "pwd", "env"))
		status = env_or_pwd_cmd(cmd[0], env, 0, _out_fd);
	else if (str_cmp(cmd[0], "export", "unset"))
		cmd = unset_or_export_cmd(cmd, env, _out_fd, &status);
	else if (str_cmp(cmd[0], "cd", NULL))
		status = ft_cd(cmd, env, _out_fd);
	else if (str_cmp(cmd[0], "exit", NULL))
		ft_exit(cmd);
	free_string_array(cmd);
	return (status);
}
