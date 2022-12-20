/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 16:55:30 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/20 22:40:06 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	hex_to_dec(char *hex)
{
	int	i;
	int	len;
	int	sum;

	i = -1;
	sum = 0;
	if (hex[0])
	{
		hex++;
		len = ft_strlen(hex);
		while (hex[++i])
		{
			if (ft_isdigit(hex[i]))
				sum += (hex[i] - 48) * pow(16, --len);
			else
				sum += (ft_toupper(hex[i]) - 55) * pow(16, --len);
		}
		return (sum);
	}
	return (WHITE);
}

t_list	*create_row(char **point_strs, int count_points, int y, int *z_max)
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
		if (abs(p[i].z) > *z_max)
			*z_max = abs(p[i].z);
	}
	p[count_points].color = -1;
	return (ft_lstnew(p));
}

int	calc_scale_factor(int row_count, int col_count, int z_max)
{
	int	w;
	int	h;
	int	z;

	w = WIN_WIDTH / (col_count * 1.4);
	h = WIN_HEIGHT / (row_count * 1.4);
	z = __INT_MAX__;
	if (z_max > 0)
		z = WIN_HEIGHT / ((float)z_max * 1.4);
	printf("w %d, h %d, z %d\n", w, h, z);
	if (w <= h && w <= z)
		return (w);
	else if (h <= w && h <= z)
		return (h);
	return (z);
}

/**
 * @brief Gets the (x,y,z) coordinate values from the fdf map file
 * 
 * 
 * @param fd file descriptor of open .fdf file
 * @param lst Each list row will have an array of points,
 * 			  each point will describe x, y, z and color
 * @param m mlx object
 */
void	get_coordinates(int fd, t_list **lst, t_mlx *m)
{
	char	*row;
	char	**point_strs;
	int		new_count;
	t_list	*rows_end;

	m->y_max = 0;
	m->z_max = 0;
	row = get_next_line(fd);
	point_strs = ft_split2(row, " \n", &m->x_max);
	// ft_lstadd_back(&rows_start, create_row(point_strs, count_points, y++));
	*lst = create_row(point_strs, m->x_max, (m->y_max)++, &(m->z_max));
	rows_end = *lst;
	free(row);
	ft_free_strs(point_strs);
	row = get_next_line(fd);
	while (row)
	{
		point_strs = ft_split2(row, " \n", &new_count);
		if (new_count != m->x_max)
		{
			ft_printf("Found wrong line length. Exiting.\n");
			while (row)
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
		rows_end->next = create_row(point_strs, new_count, (m->y_max)++, &(m->z_max));
		rows_end = rows_end->next;
		free(row);
		ft_free_strs(point_strs);
		row = get_next_line(fd);
	}
	close(fd);
	m->scale = calc_scale_factor(m->y_max, m->x_max, m->z_max);
}
