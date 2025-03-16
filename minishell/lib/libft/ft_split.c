/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:33:32 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:33:19 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* count_words conta o numeor de palavras na string 's', uusando o delimitador
percorrndo a string, incrementa words toda vez que a palavra eh encontrada.
retorna o total de [alavras encontradas*/

static char	**ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free (str[i]);
		i++;
	}
	free (str);
	str = NULL;
	return (str);
}

static int	count_word(char const *s, char c)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	j = 0;
	flag = 0;
	while (s[i])
	{
		if (s[i] != c && flag == 0)
		{
			j++;
			flag = 1;
		}
		else if (s[i] == c && flag == 1)
			flag = 0;
		i++;
	}
	return (j);
}

static int	count_char(char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[j] != c)
			j++;
		i++;
	}
	return (j);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**ptr;
	int		number_word;
	int		number_char;

	i = 0;
	number_char = 0;
	if (!s)
		return (NULL);
	number_word = count_word(s, c);
	ptr = (char **)malloc(sizeof(char *) * (number_word + 1));
	if (!ptr)
		return (NULL);
	while (i < number_word)
	{
		while (*s == c)
			s++;
		number_char = count_char(s, c);
		ptr[i] = ft_substr(s, 0, number_char);
		if (!ptr[i])
			ptr = ft_free(ptr);
		s += number_char;
		i++;
	}
	return (ptr[i] = NULL, ptr);
}
