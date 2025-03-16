/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:06:55 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:34:28 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** @brief concatena duas strings.
 @param s1 e s2 sao string a serem concatenadas.
 @return uma nova string resultado da juncao de s1 e s2.
*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = -1;
	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (len--)
	{
		if (s1[i])
		{
			str[i] = s1[i];
			i++;
		}
		else if (s2[++j])
			str[i + j] = s2[j];
	}
	str[i + j + 1] = '\0';
	return (str);
}
