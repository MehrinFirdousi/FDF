/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 16:55:30 by mfirdous          #+#    #+#             */
/*   Updated: 2022/11/21 22:09:41 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * @todo
 * find what error msgs are displayed when:
 * 
 * 		3. file is not given as a param
 * 			OR
 * 		   wrong number of arguments is given
 */
// hex to dec converter function
//	check if given address is null ptr 
// 	if it is null return white as the color
//	if not then first comma + 1 and convert the foll string from hex to dec 
//	and finally set the given comma address to a \0

t_list	*create_row(char **point_strs, int count_points)
{
	t_point	*p;
	char	*color_hex;
	int		i;

	i = -1;
	p = (t_point *)ft_malloc((count_points + 1) * sizeof(t_point));
	while (point_strs[++i])
	{
		color_hex = ft_strchr(point_strs[i], ',') + 1;
		// p[i].color = hex_to_dec(color_hex);
		p[i].color = 222;
		p[i].value = ft_atoi(point_strs[i]); // free this and see if it works without leaks; because we're changing the \0
	}
	return (ft_lstnew(p));
}

void	free_points(void *points)
{
	int	i;

	if (points)
	{
		i = -1;
		while (points + i)
			free(points + i);
	}
}

// each list_row will have an array of points, each point will describe x, y, z and color
t_list *get_rows(char *file_name)
{
	int		fd;
	char	*row;
	char	**point_strs;
	int		count_points;
	int		new_count;
	t_list *rows_start;
	t_list *rows_end;
	
	printf("file name %s\n", file_name);
	if (ft_strncmp(ft_strchr(file_name, '.') + 1, "fdf", 4) != 0)
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
	row = get_next_line(fd);
	point_strs = ft_split(row, ' ', &count_points);
	rows_start = create_row(point_strs, count_points);
	free(row);
	ft_free_strs(point_strs);
	rows_end = rows_start;
	row = get_next_line(fd);
	while (row)
	{
		point_strs = ft_split(row, ' ', &new_count);
		if (new_count != count_points)
		{
			ft_printf("Found wrong line length. Exiting.\n");
			close(fd);
			free(row);
			ft_free_strs(point_strs);
			ft_lstclear(&rows_start, &free_points);
			exit(EXIT_FAILURE);
		}
		rows_end->next = create_row(point_strs, new_count);
		rows_end = rows_end->next;
		free(row);
		ft_free_strs(point_strs);
		row = get_next_line(fd);
	}
	return (rows_start);
}

// int main(int argc, char **argv)
// {
// 	char *line;

// 	int fd = open("testfile", O_RDONLY);
// 	while ((line = get_next_line(fd)))
// 	{
// 		printf("-%s\n", line);
// 		printf("%d\n", ft_strlen(line));
// 	}
// 	return (0);
// }