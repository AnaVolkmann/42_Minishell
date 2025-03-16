/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 20:09:54 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:30:28 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** @brief copia a contagem de bytes de src para destino. 
Esta função permite copiar entre objetos que podem se 
sobrepor como se src fosse primeiro copiado para um 
array temporário.
 @param dest - ponteiro para a matriz de destino para onde
o conteúdo deve ser copiado.
 @param src ponteiro para a fonte de dados a ser copiada.
 @param n numero de bytes a ser copiado. 
 @return um ponteiro para o destino. 
*/

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*s1;
	unsigned char	*s2;

	s1 = (unsigned char *)src;
	s2 = (unsigned char *)dst;
	if (dst <= src)
		dst = ft_memcpy(dst, src, len);
	else
	{
		while (len--)
			s2[len] = s1[len];
	}
	return (dst);
}
