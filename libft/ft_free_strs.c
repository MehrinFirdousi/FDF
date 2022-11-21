/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_strs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 20:16:30 by mfirdous          #+#    #+#             */
/*   Updated: 2022/11/21 12:45:27 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_strs(char **args)
{
	int	i;

	i = -1;
	if (args)
	{
		while (args[++i])
			free(args[i]);
		free(args);
	}
}
