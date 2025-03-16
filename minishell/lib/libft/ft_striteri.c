/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:13:21 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:34:17 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* static void	test(unsigned int n, char *s)
{
	*s += n;
} */
/** @brief aplica a funcao 'f' a cada char da strig passada como arg
passando seu index como primeiro para, cada char passa pela funcao para
ser modificada, se necessario.
 @param s string a ser iterada
 @param f funcao a ser aplicada a cada char
 @return none 
*/
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int	i;

	i = 0;
	if (!s || !f)
		return ;
	while (s[i])
	{
		f(i, &(s[i]));
		i++;
	}
}
