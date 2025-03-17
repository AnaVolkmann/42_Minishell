/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utilities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:12:57 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 22:00:09 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Converts a shell error code to an appropriate exit status.
 * @param err The error code.
 * @return The corresponding shell exit status.
 * @details Maps specific error codes (e.g., 2 and 13) to the respective exit statuses (127 and 126). 
 *          For other error codes, returns the original error code.
 */
int	get_shell_exit_status(int err)
{
	if (err == 2)
		return (127);
	else if (err == 13)
		return (126);
	return (err);
}

/**
 * @brief Counts the number of digits in an integer.
 * @param num The integer to count digits for.
 * @return The number of digits in the integer.
 * @details Continuously divides the number by 10 to count the number of digits. 
 *          Handles the case where the number is 0 by returning 1.
 */
int	count_digits_in_int(int num)
{
	int			a;

	a = 0;
	if (!num)
		return (1);
	while (num)
	{
		num /= 10;
		a++;
	}
	return (a);
}

/**
 * @brief Frees the memory allocated for the environment variables.
 * @param array The array of environment variables to be freed.
 * @details Iterates through the array of environment variables, freeing each key-value pair and the array itself.
 */
void	free_environment_variables(char ***array)
{
	int				a;

	a = 0;
	while (array[a])
	{
		free(array[a][0]);
		free(array[a][1]);
		free(array[a]);
		a += 1;
	}
	free(array);
}

/**
 * @brief Cleans up and exits the shell, freeing the allocated resources.
 * @param env The environment structure to free.
 * @param status The exit status to return on shell exit.
 * @details Frees memory associated with the environment, then exits the shell with the provided exit status.
 */
void	cleanup_and_exit_shell(t_env *env, int status)
{
	if (env)
	{
		free_string_array(env->original_env);
		free_environment_variables(env->parsed_env);
		free(env);
	}
	exit(status);
}

/**
 * @brief Closes the specified pipe ends.
 * @param fd1 The first file descriptor to close.
 * @param fd2 The second file descriptor to close.
 * @details Closes both the file descriptors provided, used for cleaning up after piping operations.
 */
void	close_pipe_ends(int fd1, int fd2)
{
	safe_close(fd1);
	safe_close(fd2);
}
