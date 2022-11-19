/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 16:55:30 by mfirdous          #+#    #+#             */
/*   Updated: 2022/11/19 20:46:43 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// hex to dec converter function

t_list	*create_row(char *row, int len)
{
	char	**points;
	int		new_len;
	int		i;
	int		temp;
	
	points = ft_split(row, ' ', &new_len);
	if (new_len != len)
	{
		ft_printf("Found wrong line length. Exiting.\n");
		ft_free_strs(points);
		exit(EXIT_FAILURE);
	}
	i = -1;
	while (points[++i])
	{
		temp = convert_to_hex(ft_strchr(points[i], ',') + 1);
		
	}
	ft_free_strs(points);
}

// each list_row will have an array of points, each point will describe x, y, z and color
s_list *get_rows(char *file_name)
{
	int	fd;
	int	row;
	t_list *last_row;
	t_list *rows;
	
	fd = open(file_name, O_RDONLY);
	row = get_next_line(fd);
	last_row = create_row(row, t_list *last_row);
	while (row)
	{
		
		row = get_next_line(fd);
	}
}