/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:07:01 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 21:46:11 by ana-lda-         ###   ########.fr       */
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

int					g_signal;

/** @brief Executes piped commands by recursively processing AST nodes.
 * Manages pipes, redirections, and commands in a pipeline.
 *
 * @param head AST node representing the current command or redirection.
 * @param piped_state Structure tracking pipe and redirection state.
 * @param env Environment variables and shell state.
 * @param fd File descriptors for piping.
 * @return Execution status of the pipeline.*/
int	handle_piped_command_execution(
		t_ast_node *head, int *pipe_data, t_env *env, int *_fd)
{
	int				status;

	if (head->file_type == EXECUTE_FILE)
	{
		pipe_data[8] = 0;
		status = prepare_and_execute_command(head->args, _fd, pipe_data, env);
	}
	if (head->type == T_REDIR_IN
		|| head->type == T_REDIR_OUT
		|| head->type == T_REDIR_APPEND
		|| head->type == T_REDIR_HEREDOC)
		return (handle_command_redirection(head, pipe_data, env, _fd));
	if (head->left)
		status = handle_piped_command_execution(head->left, pipe_data, env, _fd);
	if (head->right)
		status = handle_piped_command_execution(head->right, pipe_data, env, _fd);
	return (status);
}

/** @brief Handles redirection commands, updating pipe and file descriptors.
 * Manages redirection types (input, output, append, heredoc)
 * and executes commands if needed.
 *
 * @param head AST node representing the redirection.
 * @param piped_state Pipe state structure containing redirection information.
 * @param env Environment variables and shell state.
 * @param fd File descriptors for redirection.
 * @return Status after processing redirection. */
int	handle_command_redirection(
		t_ast_node *head, int *pipe_data, t_env *env, int *_fd)
{
	int				status;

	pipe_data[11] = 1;
	if (head->right)
	{
		status = open_file_for_redirection(head->right, pipe_data, env, 0);
		if ((status || !head->left) && pipe_data[0] > 1)
			pipe_data[0] -= 1;
	}
	if (head->left && head->left->file_type == EXECUTE_FILE
		&& pipe_data[11] && !status)
	{
		pipe_data[8] = 1;
		status = prepare_and_execute_command(
				head->left->args, _fd, pipe_data, env);
	}
	if (head->left && head->left->type == T_PIPE
		&& pipe_data[11])
		status = handle_piped_command_execution(head->left, pipe_data, env, _fd);
	if (head->left && (head->left->type == T_REDIR_IN
			|| head->left->type == T_REDIR_OUT
			|| head->left->type == T_REDIR_APPEND
			|| head->left->type == T_REDIR_HEREDOC))
		status = handle_command_redirection(head->left, pipe_data, env, _fd);
	return (status);
}

/** @brief Executes an AST node by determining
 * its type and processing commands or redirections.
 * Manages pipe and file descriptor cleanup after execution.
 *
 * @param head AST node representing the current command or operation.
 * @param piped_state Pipe state structure containing execution state.
 * @param env Environment variables and shell state.
 * @return Final status after execution.*/
int	execute_ast_node(t_ast_node *head, int *pipe_data, t_env *env)
{
	int					_fd[2];
	int					status;

	_fd[0] = -1;
	_fd[1] = -1;
	if (head->file_type == FILE_READY)
	{
		if (head->type == T_PIPE)
			status = handle_piped_command_execution(head, pipe_data, env, _fd);
		if (head->type == T_REDIR_IN
			|| head->type == T_REDIR_OUT
			|| head->type == T_REDIR_APPEND
			|| head->type == T_REDIR_HEREDOC)
			status = handle_command_redirection(head, pipe_data, env, _fd);
	}
	if (head->file_type == EXECUTE_FILE)
		status = prepare_and_execute_command(head->args, _fd, pipe_data, env);
	status = wait_for_children(status, pipe_data);
	if (pipe_data[6])
		safe_close(pipe_data[1]);
	if (pipe_data[7])
		safe_close(pipe_data[2]);
	if (_fd[0] != -1 || _fd[1] != -1)
		(safe_close(_fd[0]), safe_close(_fd[1]));
	return (g_signal = 0, status);
}

/** @brief Main entry point for executing commands in the AST.
 * Initializes pipe state, processes variables,
 * checks permissions, and executes commands.
 *
 * @param head AST node representing the root of the command structure.
 * @param env Environment variables and shell state.
 * @param status Pointer to store the final execution status.*/
void	command_executer(t_ast_node *head, t_env *env, int *status)
{
	int	pipe_data[13];
	int	_status;

	initialize_or_reset_pipe_state(pipe_data, 1);
	count_redirects_and_pipes(head, pipe_data);
	initialize_or_reset_pipe_state(pipe_data, 0);
	adjust_ast_nodes_for_execution(head);
	expand_variables_in_ast(head, env);
	_status = check_command_file_permissions(head, env->original_env);
	if (!_status)
		*status = execute_ast_node(head, pipe_data, env);
}
