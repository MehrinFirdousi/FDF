/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 16:55:30 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/01 22:24:07 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * @todo
 * leak from gnl buffer - should we fix?
 * make get_rows return row_len and col_len ?
 * 
 */

int	hex_to_dec(char *hex)
{
	int	i;
	int	len;
	int	sum;

	i = -1;
	sum = 0;
	if (hex)
	{
		hex++;
		len = ft_strlen(hex);
		while(hex[++i])
		{
			if (ft_isdigit(hex[i]))
				sum += (hex[i] - 48) * pow(16, --len);
			else
				sum += (ft_toupper(hex[i]) - 55) * pow(16, --len); // can remove % 16 
		}
		return (sum);
	}
	return (WHITE);
}

t_list	*create_row(char **point_strs, int count_points, int y)
{
	t_point	*p;
	char	*color_hex;
	int		i;

	i = -1;
	p = (t_point *)ft_malloc((count_points + 1) * sizeof(t_point));
	while (point_strs[++i])
	{
		color_hex = ft_strchr(point_strs[i], 'x');
		p[i].color = hex_to_dec(color_hex);
		p[i].z = ft_atoi(point_strs[i]);
		p[i].x = i;
		p[i].y = y;
	}
	p[count_points].color = -1;
	return (ft_lstnew(p));
}

void	display_point_strs(char **point_strs)
{
	int i;

	i = -1;
	while (point_strs[++i])
		printf("-%s- ", point_strs[i]);
	printf("\n");
}

int	calc_scale_factor(int row_count, int col_count)
{
	int w_factor;
	int h_factor;

	w_factor = WIN_WIDTH / col_count;
	h_factor = WIN_HEIGHT / row_count;
	if (w_factor < h_factor)
		return (w_factor);
	return (h_factor);
}

// each list row will have an array of points, each point will describe x, y, z and color
int	get_coordinates(char *file_name, t_list **lst)
{
	int		fd;
	char	*row;
	char	**point_strs;
	int		row_count;
	int		col_count;
	int		new_count;
	t_list *rows_end;
	
	if (ft_strncmp(ft_strchr(file_name, '.'), ".fdf", 5) != 0)
	{
		ft_printf("File has to be in .fdf format\n");
		exit(EXIT_FAILURE);
	}
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("File does not exist\n");
		exit(EXIT_FAILURE);
	}
	row_count = 0;
	row = get_next_line(fd);
	point_strs = ft_split2(row, " \n", &col_count);
	
	// ft_lstadd_back(&rows_start, create_row(point_strs, count_points, y++));
	*lst = create_row(point_strs, col_count, row_count++);
	rows_end = *lst;
	free(row);
	ft_free_strs(point_strs);
	row = get_next_line(fd);
	while (row)
	{
		point_strs = ft_split2(row, " \n", &new_count);
		if (new_count != col_count)
		{
			ft_printf("Found wrong line length. Exiting.\n");
			while (row) // to read till the end 
			{
				free(row);
				row = get_next_line(fd);	
			}
			close(fd);
			ft_free_strs(point_strs);
			ft_lstclear(lst, &free);
			exit(EXIT_FAILURE);
		}
		// ft_lstadd_back(&rows_start, create_row(point_strs, new_count, y++));
		rows_end->next = create_row(point_strs, new_count, row_count++);
		rows_end = rows_end->next;
		free(row);
		ft_free_strs(point_strs);
		row = get_next_line(fd);
	}
	close(fd);
	return (calc_scale_factor(row_count, col_count));
}
