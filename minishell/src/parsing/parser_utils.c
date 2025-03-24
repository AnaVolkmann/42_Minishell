/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufiguei <lufiguei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:06:12 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/24 15:48:39 by lufiguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a new abstract syntax tree (AST) node with the specified type.
 * @param type The type of the AST node.
 * @return A pointer to the new AST node, or NULL if memory allocation fails.
 */
t_ast_node	*new_ast_node(t_token_type type)
{
	t_ast_node		*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return (node);
}

/**
 * @brief Recursively frees all nodes in the AST, including their
 *  arguments and children.
 * @param node The root AST node to free.
 */
void	free_ast(t_ast_node *node)
{
	int				i;

	i = 0;
	if (!node)
		return ;
	if (node->type == T_WORD && node->args)
	{
		while (node->args && node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

void	free_ast_child(t_ast_node *node)
{
	int				i;

	i = 0;
	if (!node)
		return ;
	while (node->parent != NULL)
		node = node->parent;
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

/**
 * @brief Creates and links a redirection AST node from the given tokens.
 * @param tokens The current token list.
 * @param tmp A temporary token to hold the redirection operator.
 * @return A pointer to the newly created redirection AST node.
 */
t_ast_node	*create_and_link_redirection(t_token **tokens, t_token *tmp)
{
	t_ast_node	*redirect_node;

	redirect_node = new_ast_node((*tokens)->type);
	*tokens = (*tokens)->next->next;
	redirect_node->left = parse_redirection(tokens);
	redirect_node->right = create_file_node(tmp->next);
	free(tmp->value);
	free(tmp);
	return (redirect_node);
}

/**
 * @brief Counts the number of command arguments in the token list.
 * @param current The current token in the list.
 * @return The number of arguments of type T_WORD in the token list.
 */
int	count_command_arguments(t_token *current)
{
	int	arg_count;

	arg_count = 0;
	while (current && current->type == T_WORD)
	{
		arg_count++;
		current = current->next;
	}
	return (arg_count);
}

/**
 * @brief Fills the arguments of a command AST node from the given token list.
 * @param command_node The command AST node to fill.
 * @param tokens The token list to extract arguments from.
 * @param arg_count The number of arguments to fill.
 */
void	fill_command_arguments(t_ast_node *command_node,
	t_token **tokens, int arg_count)
{
	int		i;
	t_token	*tmp;

	i = 0;
	while (i < arg_count)
	{
		command_node->args[i] = ft_strdup((*tokens)->value);
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free(tmp->value);
		free(tmp);
		i++;
	}
	command_node->args[arg_count] = NULL;
}
