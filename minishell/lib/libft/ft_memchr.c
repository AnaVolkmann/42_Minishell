/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 11:59:11 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:30:28 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** @brief procura a primeira ocorrência do caractere c 
nos primeiros n bytes da string apontada, pelo argumento str.
 @param str ponteiro para o bloco de memoria onde
a busca pelo character deve ser feita.
 @param c esse eh o valor passado como int, a funcao executa um pesquisa
 byte por byte usando a conversao unsiged int desse valor.
 @param n numero de bytes a sererm analizados.
 */
void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*obj;

	i = 0;
	obj = (unsigned char *)s;
	while (i < n)
	{
		if (obj[i] == (unsigned char)c)
			return (&obj[i]);
		i++;
	}
	return (0);
}
