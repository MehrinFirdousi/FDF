/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 16:55:30 by mfirdous          #+#    #+#             */
/*   Updated: 2022/11/22 21:19:30 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * @todo
 * find what error msgs are displayed when:
 * 
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
	len = ft_strlen(hex);
	sum = 0;
	printf("hex: %s, ", hex);
	if (hex && hex[0])
	{
		while(hex[++i])
		{
			if (ft_isdigit(hex[i]))
				sum += (hex[i] - 48) * pow(16, --len);
			else
				sum += (hex[i] - 55) * pow(16, --len); // can remove % 16 
		}
	}
	else
		sum = WHITE;
	printf("%d\n", sum);
	return (sum);
}

t_list	*create_row(char **point_strs, int count_points)
{
	t_point	*p;
	char	*color_hex;
	int		i;

	i = -1;
	p = (t_point *)ft_malloc((count_points + 1) * sizeof(t_point));
	while (point_strs[++i])
	{
		color_hex = ft_strchr(point_strs[i], 'x') + 1;
		p[i].color = hex_to_dec(color_hex);
		p[i].value = ft_atoi(point_strs[i]); // free this and see if it works without leaks; because we're changing the \0
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

void	display_row(void *point)
{
	t_point *p;
	int i;
	
	p = (t_point *)point;
	i = -1;
	while(p[++i].color != -1)
	{
		printf("%d:", p[i].value);
		printf("%d ", p[i].color);
	}
	printf("\n");
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
	row = get_next_line(fd);
	point_strs = ft_split2(row, " \n", &count_points);
	rows_start = create_row(point_strs, count_points);
	// ft_lstadd_back(&rows_start, create_row(point_strs, count_points));
	
	rows_end = rows_start;
	// ft_lstiter(rows_start, &display_row);
	free(row);
	ft_free_strs(point_strs);
	row = get_next_line(fd);
	while (row)
	{
		point_strs = ft_split2(row, " \n", &new_count);
		if (new_count != count_points)
		{
			ft_printf("Found wrong line length. Exiting.\n");
			close(fd);
			free(row);
			ft_free_strs(point_strs);
			ft_lstclear(&rows_start, &free);
			exit(EXIT_FAILURE);
		}
		// ft_lstadd_back(&rows_start, create_row(point_strs, new_count));
		rows_end->next = create_row(point_strs, new_count);
		rows_end = rows_end->next;
		free(row);
		ft_free_strs(point_strs);
		row = get_next_line(fd);
	}
	ft_lstiter(rows_start, &display_row);
	return (rows_start);
}
