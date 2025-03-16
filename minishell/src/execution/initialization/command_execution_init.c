/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution_init.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:06:26 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 21:56:22 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** 
 * @brief Checks for directory errors and sets the status accordingly.
 * 
 * This function checks the given path and file for potential directory errors. It ensures that the path does not point to an invalid directory or a file when the file is expected to be something else. It also handles specific error messages for common issues like attempting to work with directories when files are expected.
 * 
 * @param path_ The path of the file or directory to check.
 * @param file The name of the file or directory to verify.
 * @param status A pointer to the status variable which is updated based on the check.
 */
void	check_directory_status(char *path_, char *file, int *status)
{
	struct stat		s;

	if (file && str_cmp(file, ".", NULL))
		*status = 2;
	else if (str_cmp(file, "..", NULL)
		|| str_cmp(file, ",", ""))
	{
		*status = 1;
		errno = 2;
	}
	else if (!stat(path_, &s)
		&& s.st_mode & __S_IFDIR)
	{
		*status = 2;
		ft_putstr_fd("   err: this \'", 2);
		ft_putstr_fd(path_, 2);
		ft_putendl_fd("\' Is a directory", 2);
		errno = 13;
	}
}

/** 
 * @brief Handles error messages for file-related errors.
 * 
 * This function processes and prints error messages for file handling. It displays appropriate error messages based on the status code provided, helping to identify issues with files and directories.
 * 
 * @param file The name of the file associated with the error.
 * @param _status The status code indicating the type of error encountered.
 * 
 * @return The status code after processing the error.
 */
int	handle_file_error_message(char *file, int _status)
{
	if (_status == 1)
	{
		_status = get_shell_exit_status(errno);
		ft_putstr_fd("   Error: Unable to process file '", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd("\' ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (_status);
	}
	else if (_status)
	{
		ft_putstr_fd("   minishell(\'", 2);
		ft_putstr_fd(file, 2);
		ft_putendl_fd("': Operation failed, please check input or environment.", 2);
	}
	return (_status);
}

/** 
 * @brief cheks the file permissions for a command.
 * 
 * This function checks if the file associated with a command has the appropriate read or write permissions based on its type. It handles error checking and ensures that files can be accessed or processed as expected.
 * 
 * @param head The AST node representing the command being executed.
 * @param env The environment variables available for the command.
 * 
 * @return The status code indicating whether the file permissions are valid or not.
 */
int	check_command_file_permissions(t_ast_node *head, char **env)
{
	int				status;
	char			*path_;

	status = 0;
	if (head->args
		&& !check_if_command_is_builtin(head->args[0])
		&& head->file_type == READ_FILE)
	{
		path_ = get_file_path(head->args[0], env, "PWD", R_OK);
		if (!path_)
			status = 0;
		else
		{
			check_directory_status(path_, head->args[0], &status);
			free(path_);
		}
		status = handle_file_error_message(head->args[0], status);
	}
	if (!status && head->left)
		status = check_command_file_permissions(head->left, env);
	if (!status && head->right)
		status = check_command_file_permissions(head->right, env);
	return (status);
}

/** 
 * @brief Adjusts AST nodes for proper execution, based on their type.
 * 
 * This function iterates over an Abstract Syntax Tree (AST) and adjusts the file type of each node based on the redirection or piping that is specified. It sets flags for redirections, pipes, and file types for commands to be executed.
 * 
 * @param head The root node of the AST to adjust.
 */
void	adjust_ast_nodes_for_execution(t_ast_node *head)
{
	if (head->type != T_WORD)
	{
		head->file_type = FILE_READY;
		if (head->type == T_REDIR_OUT && head->right)
			head->right->file_type = WRITE_FILE;
		if (head->type == T_REDIR_APPEND && head->right)
			head->right->file_type = WRITE_FILE_APPEND;
		if (head->type == T_REDIR_IN && head->right)
			head->right->file_type = READ_FILE;
		if (head->type == T_REDIR_HEREDOC && head->right)
			head->right->file_type = READ_FROM_APPEND;
		if (head->type == T_PIPE)
		{
			if (head->right)
				head->right->file_type = EXECUTE_FILE;
			if (head->left)
				head->left->file_type = EXECUTE_FILE;
		}
	}
	if (!head->file_type)
		head->file_type = EXECUTE_FILE;
	if (head->left)
		adjust_ast_nodes_for_execution(head->left);
	if (head->right)
		adjust_ast_nodes_for_execution(head->right);
}


/** 
 * @brief Counts the number of redirections and pipes in an AST.
 * 
 * This function counts how many redirection and pipe operations are present in the AST. It updates an array of counters that track the number of output redirections, input redirections, pipes, and other relevant details for the command execution.
 * 
 * @param head The root node of the AST to process.
 * @param pipe_data An array that holds the counts for redirections and pipes.
 */
void	count_redirects_and_pipes(t_ast_node *head, int *pipe_data)
{
	head->file_type = 0;
	if (head->type == T_REDIR_OUT
		|| head->type == T_REDIR_APPEND)
		pipe_data[4] += 1;
	else if (head->type == T_REDIR_IN
		|| head->type == T_REDIR_HEREDOC)
		pipe_data[3] += 1;
	else if (head->type == T_PIPE)
		pipe_data[5] += 1;
	if (head->left)
		count_redirects_and_pipes(head->left, pipe_data);
	if (head->right)
		count_redirects_and_pipes(head->right, pipe_data);
}
