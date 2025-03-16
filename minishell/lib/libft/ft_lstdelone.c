/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:30:48 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 19:30:28 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** @brief Toma como parâmetro um nó e libera a memória do
o conteúdo do nó usando a função 'del' fornecida
como parâmetro e libere o nó.
 @param lst  o noh para liberar mem. 
 @param del  endereco da funcao usada para deletar o conteudo. 
 @return none.
*/
void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (!lst)
		return ;
	del(lst->content);
	free(lst);
}
