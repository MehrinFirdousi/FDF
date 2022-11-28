/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 16:55:30 by mfirdous          #+#    #+#             */
/*   Updated: 2022/11/28 22:10:40 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * @todo
 * leak from gnl buffer - should we fix?
 * make get_rows return row_len and col_len ?
 * 
 */

// hex to dec converter function
//	check if given address is null ptr 
// 	if it is null return white as the color
//	if not then first comma + 1 and convert the foll string from hex to dec 
//	and finally set the given comma address to a \0

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
		p[i].x = i * 85;
		p[i].y = y * 85;
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

// each list_row will have an array of points, each point will describe x, y, z and color
t_list *get_rows(char *file_name)
{
	int		fd;
	int		y;
	char	*row;
	char	**point_strs;
	int		count_points;
	int		new_count;
	t_list *rows_start;
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
	y = 0;
	row = get_next_line(fd);
	point_strs = ft_split2(row, " \n", &count_points);
	printf("width divided: %d\n", WIN_WIDTH / count_points);
	rows_start = create_row(point_strs, count_points, y++);
	// ft_lstadd_back(&rows_start, create_row(point_strs, count_points, y++));
	rows_end = rows_start;
	free(row);
	ft_free_strs(point_strs);
	row = get_next_line(fd);
	while (row)
	{
		point_strs = ft_split2(row, " \n", &new_count);
		if (new_count != count_points)
		{
			ft_printf("Found wrong line length. Exiting.\n");
			while (row) // to read till the end 
			{
				free(row);
				row = get_next_line(fd);	
			}
			close(fd);
			ft_free_strs(point_strs);
			ft_lstclear(&rows_start, &free);
			exit(EXIT_FAILURE);
		}
		// ft_lstadd_back(&rows_start, create_row(point_strs, new_count, y++));
		rows_end->next = create_row(point_strs, new_count, y++);
		rows_end = rows_end->next;
		free(row);
		ft_free_strs(point_strs);
		row = get_next_line(fd);
	}
	printf("height divided: %d\n", WIN_HEIGHT/ (y + 1));
	close(fd);
	return (rows_start);
}
