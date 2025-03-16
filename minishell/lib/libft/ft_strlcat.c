/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 19:04:27 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:34:40 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** @brief anexa a string scr ao final da dest e conta o tamanho 
do resultado da concatenacao de ambas
*/
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < dstsize && *dst)
	{
		dst++;
		i++;
	}
	if (i == dstsize)
		return (i + ft_strlen(src));
	j = 0;
	while (src[j])
	{
		if (j < dstsize - i - 1)
			*dst ++ = src[j];
		j++;
	}
	*dst = '\0';
	return (i + j);
}
