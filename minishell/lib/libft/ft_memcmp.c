/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 11:23:34 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:30:28 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** @brief compara os primeiros n bytes
da área de memória str1 e da área de memória str2.
 @param str1 e str2 ponteiros para um bloco de memoria.
  @param n numeros de bytes a serem comparados.
 */
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*obj1;
	unsigned char	*obj2;

	i = 0;
	obj1 = (unsigned char *)s1;
	obj2 = (unsigned char *)s2;
	while (i < n)
	{
		if (obj1[i] != obj2[i])
			return (obj1[i] - obj2[i]);
		i++;
	}
	return (0);
}
