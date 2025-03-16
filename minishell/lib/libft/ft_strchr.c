/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:09:43 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:33:34 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** @brief procura pela primeira ocorencia do char c na string
 apontada por str.
 @param str string a ser percorida
  @param c charcarter a ser procurado.
  @return ponteiro para a primeira ocorrencia do char c na string str
  ou null, se o char nao for encontrado.*/

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (0);
}
