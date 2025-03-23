/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:12:49 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/23 17:52:14 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Removes single and double quotes from a string
 *  while preserving the content inside.
 * @param str The input string to be processed.
 * @param si_q_c The count of single quotes encountered.
 * @param do_q_c The count of double quotes encountered.
 * @param a The current index in the input string to process.
 * @return A new string with quotes removed, or NULL if memory
 *  allocation fails.
 * @details Iterates through the input string and removes quotes 
 * (single and double) while respecting the pairing of quotes. 
 * The new string is returned with quotes removed. 
 * The original string is freed after processing.*/
char	*remove_quotes_from_str(char *str, int si_q_c, int do_q_c, int a)
{
	char				*new_str;
	int					b;

	b = 0;
	new_str = malloc(sizeof_str(str, '\0') + 1);
	if (!new_str)
	{
		return (NULL);
	}
	ft_memset(new_str, 0, sizeof_str(str, '\0') + 1);
	while (str[a])
	{
		if (str[a] == 34 && !(si_q_c % 2))
			do_q_c++;
		else if (str[a] == 39 && !(do_q_c % 2))
			si_q_c++;
		if ((str[a] != 34 || si_q_c % 2)
			&& ((str[a] != 39) || do_q_c % 2))
			new_str[b++] = str[a];
		a++;
	}
	new_str[b] = '\0';
	free(str);
	return (new_str);
}

/** @brief Copies a string to a new dynamically allocated memory.
 * @param src The source string to copy.
 * @return A new dynamically allocated string that is a copy of the 
 * source string.
 * @details Creates a new string that is an exact copy of the source string, 
 * and returns the new string. Returns NULL if memory allocation fails.*/
char	*strcopy(char *src)
{
	int					a;
	int					b;
	char				*dest;

	b = 0;
	while (src[b])
		b += 1;
	dest = malloc(b + 1);
	if (!dest)
		return (NULL);
	a = 0;
	while (a < b)
	{
		dest[a] = src[a];
		a += 1;
	}
	dest[a] = '\0';
	return (dest);
}

/** @brief Copies a substring from one string to another.
 * @param s_1 The destination string to copy the substring to.
 * @param s_2 The source string to copy the substring from.
 * @param st The starting index in the source string.
 * @param en The ending index in the source string.
 * @details Copies a portion of the string `s_2` from the index
 *  `st` to `en` (exclusive) into the string `s_1`.*/
void	s_strcopy(char *s_1, char *s_2, int st, int en)
{
	int				a;

	a = 0;
	while (st < en)
		s_1[a++] = s_2[st++];
	s_1[a] = '\0';
}

void	safe_close(int fd)
{
	if (fd >= 0)
		close(fd);
	else
		return ;
}
