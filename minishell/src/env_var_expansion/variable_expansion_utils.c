/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alawrence <alawrence@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:12:35 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 16:39:13 by alawrence        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Recursively processes a string to extract words,
 *  considering quotes and spaces.
 * This function iterates through a string and extracts words 
 * separated by spaces or quotes. It accounts for both
 *  single and double quotes. 
 * The extracted words are stored in an array, and the 
 * function recursively moves through the string.
 * @param array The array to store extracted words.
 * @param index The current index in the array where the 
 * next word should be stored.
 * @param str The input string to process.
 * @param in The current index in the input string to start processing from.
 * @return The updated index in the array after all words have been processed.*/
int	simplified_refactor(char **array, int index, char *str, int in)
{
	int							size;

	if (!str[in] || in > sizeof_str(str, '\0'))
		return (index);
	while (str[in] && str[in] == ' ')
		in++;
	size = sizeof_str(str + in, ' ');
	if (size > sizeof_str(str + in, 34))
		size = sizeof_str(str + in, 34);
	if (size > sizeof_str(str + in, 39))
		size = sizeof_str(str + in, 39);
	if (str[in] == 34)
		size = sizeof_str(str + in + 1, 34) + 2;
	if (str[in] == 39)
		size = sizeof_str(str + in + 1, 39) + 2;
	if ((in + size) > sizeof_str(str, '\0'))
		return (index);
	array[index] = malloc(size + 1);
	if (!array[index])
		return (index);
	s_strcopy(array[index], str, in, in + size);
	return (simplified_refactor(array, index + 1, str, in + size));
}

/** @brief Counts the number of special characters
 *  (`"`, `'`, `$`) in the string.
 * This function counts the occurrences of double quotes
 *  (`"`), single quotes (`'`), and dollar signs (`$`) in
 *  the string. It is used to track the number of special 
 * characters for further processing.
 * @param str The input string to process.
 * @param s_q The count of single quotes (unused in this function).
 * @param d_q The count of double quotes.
 * @return The updated count of double quotes and special characters.*/
int	sus_getit_right(char *str, int s_q, int d_q)
{
	int					a;

	a = 0;
	(void)s_q;
	while (str[a])
	{
		if (str[a] == 34 || str[a] == 39
			|| str[a] == '$')
			d_q++;
		a++;
	}
	return (d_q);
}

/** @brief Checks for flaws in a string based on the presence 
 * of spaces and quotes.
 * This function scans a string for spaces that are not enclosed
 *  by quotes and returns a count of such occurrences. 
 * It tracks if a space appears outside of single or double quotes.
 * @param str The input string to check.
 * @param a The current index in the string.
 * @param b A flag to indicate if a space was encountered.
 * @param res The accumulated result from previous checks.
 * @return The number of detected flaws in the string.*/
int	is_flawed_str(char *str, int a, int b, int res)
{
	int					si_q;
	int					do_q;

	si_q = 0;
	do_q = 0;
	while (str[a])
	{
		if (str[a] == 34)
			do_q++;
		else if (str[a] == 39)
			si_q++;
		else if (!(si_q % 2) && !(do_q % 2))
		{
			if (str[a] == ' ')
			{
				if (b)
					res++;
				b = 0;
			}
			else
				b = 1;
		}
		a++;
	}
	return (res);
}

/** @brief Detects flaws in an array of strings by analyzing
 *  each string.
 * This function checks all strings in the array for flaws, 
 * such as unbalanced quotes or incorrect formatting. It counts
 *  flaws based on spaces and special characters in each string.
 * @param array The array of strings to check.
 * @return The total number of detected flaws across all strings
 *  in the array.*/
int	detected_errors(char **array)
{
	int			a;
	int			res;

	res = 0;
	a = 0;
	while (array[a])
	{
		res += is_flawed_str(array[a], 0, 0, 0);
		res += sus_getit_right(array[a], 0, 0);
		a++;
	}
	return (res);
}

char	*replace_variable_with_value(
		char *old_var, char *__new, int st, int end)
{
	int							size;
	char						*new__;
	int							unsize;

	unsize = sizeof_str(__new, '\0');
	size = st + (sizeof_str(old_var, '\0') - end) + unsize;
	new__ = malloc(size + 1);
	if (!new__)
		return (NULL);
	s_strcopy(new__, old_var, 0, st);
	s_strcopy(new__ + st, __new, 0, unsize);
	s_strcopy(new__ + st + unsize, old_var, end, sizeof_str(old_var, '\0'));
	free(old_var);
	return (new__);
}
