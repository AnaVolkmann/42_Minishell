/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:11:44 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 21:12:19 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** 
 * @brief Prints an environment variable to the specified file descriptor.
 * 
 * @param str_1 The environment variable name.
 * @param str_2 The environment variable value.
 * @param fd The file descriptor to print to.
 */
void	print_env_var_to_fd(char *str_1, char *str_2, int fd)
{
	if (!str_cmp(str_1, "?", NULL)
		&& !str_cmp(str_2, "F1", NULL))
	{
		ft_putstr_fd(str_1, fd);
		ft_putstr_fd("=", fd);
		if (str_2[0])
			ft_putstr_fd(str_2, fd);
		write(fd, "\n", 1);
	}
}

/** 
 * @brief Prints all environment variables in the export format to the specified file descriptor.
 * 
 * @param arr The array of environment variables.
 * @param a The number of environment variables in the array.
 * @param fd The file descriptor to print to.
 */
void	print_export_vars(char ***arr, int a, int fd)
{
	while (a--)
	{
		if (!str_cmp(arr[a][0], "?", NULL))
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(arr[a][0], fd);
			if (!str_cmp(arr[a][1], "F1", NULL))
			{
				write(fd, "=", 1);
				ft_putchar_fd('"', fd);
				if (arr[a][1][0])
					ft_putstr_fd(arr[a][1], fd);
				ft_putchar_fd('"', fd);
			}
			write(fd, "\n", 1);
		}
	}
}

/** 
 * @brief Compares two strings lexicographically based on their characters' ASCII values.
 * 
 * @param s_1 The first string.
 * @param s_2 The second string.
 * 
 * @return 0 if s_1 is greater, 1 if s_2 is greater, or 0 if they are equal.
 */
int	string_weight_compare(char *s_1, char *s_2)
{
	int					a;

	a = 0;
	while (s_1[a] && s_2[a])
	{
		if (s_1[a] > s_2[a])
			return (0);
		else if (s_2[a] > s_1[a])
			return (1);
		a++;
	}
	if (s_2[a])
		return (1);
	else if (s_1[a])
		return (0);
	return (0);
}

/** 
 * @brief Checks if the elements in the 2D array are sorted.
 * 
 * @param array The array to check.
 * @param _si The size of the array.
 * 
 * @return 1 if the array is sorted, 0 if not.
 */
int	check_array_arrangment(char ***array, int _si)
{
	int					a;
	int					b;
	int					c;

	a = 0;
	while (a < _si - 1)
	{
		b = a + 1;
		while (b < _si)
		{
			c = string_weight_compare(array[a][0], array[b][0]);
			if (c || (c < 0 && (b - a) > 1))
				return (0);
			b++;
		}
		a++;
	}
	return (1);
}


/** 
 * @brief Checks if the given string is a valid echo parameter.
 * 
 * @param s The string to check.
 * 
 * @return 1 if the string is a valid echo parameter (e.g., "-n"), 0 otherwise.
 */
int	is_valid_echo_param(char *s)
{
	int				a;

	a = 0;
	if (s && s[a] && s[a] == '-')
	{
		while (s[++a])
		{
			if (s[a] != 'n')
				return (0);
		}
	}
	else
		return (0);
	return (1);
}
