/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:52:21 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:22:55 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	skip_space(int *i, const char *str)
{
	int	sign;

	sign = 1;
	while ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == 32)
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign *= -1;
		(*i)++;
	}
	return (sign);
}

int	ft_atoi(const char *str)
{
	int			sign;
	int			i;
	long long	res;
	long long	prev;
	long long	tmp;

	i = 0;
	res = 0;
	sign = skip_space(&i, str);
	while (str[i] >= '0' && str[i] <= '9')
	{
		prev = res;
		res = res * 10 + str[i] - '0';
		tmp = res / 10;
		if (tmp != prev)
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
		i++;
	}
	return (sign * res);
}
