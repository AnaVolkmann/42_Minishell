/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 11:41:51 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:34:06 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** @brief cria uma nova string, duplicando o imput.
 @param s string a ser duplicada
 @return nova string copia do imput
*/
char	*ft_strdup(const char *src)
{
	size_t	i;
	size_t	size;
	char	*str;

	i = 0;
	size = ft_strlen(src);
	str = malloc(size + 1);
	if (str == NULL)
		return (NULL);
	while (src[i])
	{
		str[i] = src[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
