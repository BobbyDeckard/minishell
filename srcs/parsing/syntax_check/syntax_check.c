/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:54:09 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:41:10 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int syntax_is_valid(t_token **token_list)
{
    if (!token_list || !*token_list)
        return (1);
    if (!quote_syntax_is_valid(token_list))
    {
        printf("Error: Unmatched quotes\n");
        return (0);
    }
    if (!paren_syntax_is_valid(token_list))
    {
        printf("Error: Unmatched parentheses\n");
        return (0);
    }
    if (!redir_syntax_is_valid(token_list))
    {
        printf("Error: Invalid redirection syntax\n");
        return (0);
    }
    if (!operator_syntax_is_valid(token_list))
    {
        printf("Error: Invalid operator syntax\n");
        return (0);
    }
    return (1);
}
