/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:11:22 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/25 13:41:24 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Compares two strings up to a specified maximum length.
 *Returns 1 if the strings are equal within the given limit, 0 otherwise.
 * @param s_1 The first string.
 * @param s_2 The second string.
 * @param max The maximum number of characters to compare.
 * @return 1 if strings are equal up to `max` characters, 0 otherwise.*/
int	str_compare(char *s_1, char *s_2, int max)
{
	int								a;

	a = 0;
	while (a < max && *s_1 && *s_2
		&& s_1[a] == s_2[a])
		a++;
	if (sizeof_str(s_1, '\0') == a
		&& a == max)
		return (1);
	return (0);
}

/** @brief Exits the process after writing a newline to stdout.
 * This function is used as a handler for quitting a here-doc process.
 * @param a Unused parameter (kept for consistency).*/
void	quite_heredoc(int a)
{
	(void)a;
	write(1, "\n", 1);
	exit(0);
}

/** @brief Checks if a string contains any quote characters 
 * (single or double quotes).
 * @param s The string to check.
 * @return 0 if the string contains any quote characters, 1 otherwise.*/
int	is_there_any_quotes(char *s)
{
	int					a;

	a = 0;
	while (s && s[a])
	{
		if (s[a] == 34 || s[a] == 39)
			return (0);
		a++;
	}
	return (1);
}

/** @brief Reads lines from the user, expanding variables if necessary, 
 * and writes them to a specified output.
 * @param std_out The file descriptor to write the lines to.
 * @param limiter The delimiter to end the input with.
 * @param env The environment structure used for variable expansion.
 * @param is_expandable Flag indicating whether variable expansion 
 * should occur.*/
void	read_and_write(int std_out, char *limiter, t_env *env,
					int is_expandable)
{
	char							*buf;
	int								f_arr[3];

	limiter = remove_quotes_from_str(limiter, 0, 0, 0);
	while (1)
	{
		buf = readline(">> ");
		if (!buf || str_compare(limiter, buf, sizeof_str(buf, '\n')))
		{
			free(buf);
			break ;
		}
		if (is_expandable)
		{
			buf[sizeof_str(buf, '\n')] = '\0';
			ft_memset(f_arr, 0, 3 * sizeof(int));
			buf = recursively_expand_variables(buf, env, 0, f_arr);
			ft_memset(f_arr, 0, 3 * sizeof(int));
			buf = recursively_expand_variables(buf, env, 1, f_arr);
		}
		write(std_out, buf, sizeof_str(buf, '\0'));
		write(std_out, "\n", 1);
		free(buf);
	}
	free(limiter);
}

/** @brief Handles reading input for a here-document and writes
 *  the result to a pipe.
 * @param limiter The delimiter that marks the end of the here-doc input.
 * @param pipe_data Array used for managing file descriptors for piping.
 * @param env The environment structure for variable expansion.
 * @return The status of the here-doc process.*/
int	exec_here_doc(t_ast_node *limiter, int *pipe_data, t_env *env)
{
	int							_out_fd_[2];
	pid_t						pid;
	int							status;

	pipe(_out_fd_);
	pid = fork();
	signal(SIGINT, SIG_IGN);
	if (!pid)
	{
		signal(SIGINT, quite_heredoc);
		safe_close(_out_fd_[0]);
		read_and_write(_out_fd_[1], limiter->args[0], env,
			is_there_any_quotes(limiter->args[0]));
		cleanup_and_exit_shell(env, 1);
	}
	waitpid(pid, &status, 0);
	safe_close(_out_fd_[1]);
	pipe_data[1] = _out_fd_[0];
	pipe_data[9] = (WEXITSTATUS(status)) - 1;
	if (pipe_data[9] < 0)
		pipe_data[9] += 1;
	pipe_data[11] = status;
	return (pipe_data[9]);
}
