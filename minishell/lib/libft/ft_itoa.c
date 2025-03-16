/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:30:20 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:26:14 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** @brief converte um valor integer em uma string terminada em nulo
usando a base especificada e amarmazena o resultado na matriz fornecida
pero param str.
 @param n valor a ser convertido para uma string.
 @return um ponteiro para a string terminada em nulo.
*/
// lenofn -recebe o número inteiro n e retorna o número de dígitos
//necessários para representar n.
// linha 36- incrementa meu len para considerar o sinal negativo.
// linha 39- dividido por 10 para remover o último dígito, e
// len é incrementada para contar o número de dígitos.

#include "libft.h"

static int	count_char(long n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	else if (n < 0)
	{
		n *= -1;
		i++;
	}
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		i;
	char	*str;
	int		number;
	long	nb;

	i = 0;
	nb = n;
	number = count_char(n) - 1;
	str = (char *)ft_calloc((count_char(n) + 1), sizeof(char));
	if (!str)
		return (NULL);
	if (n < 0)
		nb *= -1;
	while (number >= 0)
	{
		str[number] = (nb % 10) + '0';
		nb = nb / 10;
		number--;
		i++;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}
