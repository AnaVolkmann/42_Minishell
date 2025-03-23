/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:12:53 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/23 14:35:55 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Returns the length of a string up to a specified character.
 * @param str The input string.
 * @param end The character to stop at (not included in the length).
 * @return The length of the string up to the specified character.
 * @details Iterates through the string and counts the
 * number of characters until the `end` character is encountered.*/
int	sizeof_str(char *str, char end)
{
	int			a;

	a = 0;
	while (str && str[a] != '\0' && str[a] != end)
		a += 1;
	return (a);
}

/** @brief Compares two strings for equality, 
 * optionally using a third string for comparison.
 * @param s_1 The first string to compare.
 * @param s_2 The second string to compare.
 * @param s_3 The third string to compare if the first two are equal
 *  (can be NULL).
 * @return 1 if the strings are equal, 0 otherwise.
 * @details Compares `s_1` and `s_2`. If they are equal, it
 * checks against `s_3` (if provided). 
 *The function returns 1 if all strings are equal, otherwise returns 0.*/
int	str_cmp(char *s_1, char *s_2, char *s_3)
{
	int					a;

	a = 0;
	while (s_1[a] && s_2[a] && s_1[a] == s_2[a])
		a += 1;
	if (a == sizeof_str(s_1, '\0')
		&& a == sizeof_str(s_2, '\0'))
		return (1);
	if (s_3)
		return (str_cmp(s_1, s_3, NULL));
	return (0);
}

/** @brief Converts a string representing a number to its integer value.
 * @param str The string representing a number.
 * @return The integer value of the string.
 * @details Converts each character in the string to its
 * corresponding integer value and constructs the resulting number.
 *  Non-numeric characters are ignored.*/
int	string_to_int(char *str)
{
	int					num;
	int					a;

	num = 0;
	a = 0;
	while (str[a])
	{
		if (str[a] <= '9' && str[a] >= '0')
			num = (num * 10) + (str[a] - 48);
		a++;
	}
	return (num);
}

/** @brief Counts the number of strings in a null-terminated array of strings.
 * @param arr The array of strings.
 * @return The number of strings in the array.
 * @details Iterates through the array until a NULL pointer is encountered 
 * and counts the number of strings in the array.*/
int	count_strings_in_array(char **arr)
{
	int			a;

	a = 0;
	while (arr[a] != 0)
		a++;
	return (a);
}

/** @brief Frees the memory allocated for an array of strings.
 * @param arr The array of strings to be freed.
 * @details Iterates through the array, freeing each string,
 * and then frees the array itself.*/
void	free_string_array(char **arr)
{
	int				a;

	a = 0;
	while (arr[a] != 0)
	{
		free(arr[a]);
		a += 1;
	}
	free(arr);
}
