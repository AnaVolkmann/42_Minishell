/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 17:19:32 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:31:02 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** @brief copia o caractere c para os primeiros n caracteres 
da string apontada, pelo argumento str.
 @param s ponteiro para o bloco de memoria a ser preenchido.
 @param c o valor a ser definido, (passado como um int,
a funcao converte utilizanco cast para unsiged char).
 @param n (limitador) numero de bytes a serem definidos para o valor.
 @return um pontriro para a area de memoria 's'.
 */
void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*obj;

	i = 0;
	obj = (unsigned char *)s;
	while (i < n)
	{
		obj[i] = (unsigned char)c;
		i++;
	}
	return (obj);
}
