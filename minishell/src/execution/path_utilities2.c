/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utilities2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:11:30 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 21:50:34 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Finds the index of a substring within a list of strings.
 * @param haystack The array of strings to search in.
 * @param needle The substring to search for.
 * @param n_length The length of the substring.
 * @return The index of the substring if found, or -1 if not.
 */
int	find_substr_index(char **haystack, char *needle, int n_length)
{
	int		a;
	int		b;

	a = 0;
	while (haystack[a] != 0)
	{
		if (haystack[a][0] == needle[0])
		{
			b = 0;
			while (haystack[a][b]
				&& haystack[a][b] == needle[b])
			{
				if (b == n_length - 1)
					return (a);
				b++;
			}
		}
		a++;
	}
	return (-1);
}

/**
 * @brief Checks if a given path is accessible with the specified mode.
 * @param path The file path to check.
 * @param mode The mode to check for (e.g., read, write).
 * @return 1 if the path is accessible, 0 if not.
 */
int	is_path_accessible(char *path, int mode)
{
	if (access(path, mode) < 0)
		return (0);
	return (1);
}


/**
 * @brief Increments the index to the next part of a path in an environment variable.
 * @param env_var The environment variable containing the path.
 * @param indx_s The index array containing the current position and other state information.
 * @param a The current position within the environment variable string.
 */
void	increment_path_index(char *env_var, int *indx_s, int *a)
{
	if (indx_s[3])
	{
		indx_s[1] += 1;
		*a = indx_s[1];
		while (env_var[indx_s[1]] != '\0'
			&& env_var[indx_s[1]] != ':')
			indx_s[1] += 1;
	}
	else
		*a = indx_s[1];
}

/**
 * @brief Counts the number of substrings in a string, delimited by a specified character.
 * @param str The string to search in.
 * @param del The delimiter character.
 * @return The number of substrings in the string.
 */
int	count_substrings(char *str, char del)
{
	int			a;
	int			b;
	int			res;

	a = 0;
	b = 1;
	res = 0;
	while (str && str[a])
	{
		if (str[a] != del)
		{
			if (b)
				res += 1;
			b = 0;
		}
		else
			b = 1;
		a += 1;
	}
	return (res);
}
