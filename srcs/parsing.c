/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 16:55:30 by mfirdous          #+#    #+#             */
/*   Updated: 2022/11/20 17:19:04 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * @todo
 * find what error msgs are displayed when:
 * 		1. file doesn't exist
 * 		2. file is not a .fdf
 * 		3. file is not given as a param
 * 			OR
 * 		   wrong number of arguments is given
 */
// hex to dec converter function
//	check if given address is null ptr 
// 	if it is null return white as the color
//	if not then first comma + 1 and convert the foll string from hex to dec 
//	and finally set the given comma address to a \0

t_list	*create_row(char *row, int count_points)
{
	char	**p_strs;
	t_point	*p;
	char	*comma;
	int		new_count;
	int		i;
	
	p_strs = ft_split(row, ' ', &new_count);
	if (count_points != -1 && new_count != count_points)
	{
		ft_printf("Found wrong line length. Exiting.\n");
		ft_free_strs(p_strs);
		// close fd
		exit(EXIT_FAILURE);
	}
	i = -1;
	p = (t_point *)ft_malloc((new_count + 1) * sizeof(t_point));
	while (p_strs[++i])
	{
		comma = ft_strchr(p_strs[i], ',');
		p[i].color = hex_to_dec(comma);
		p[i].value = ft_atoi(p_strs[i]); // free this and see if it works without leaks; because we're changing the \0
	}
	ft_free_strs(p_strs);
	return (ft_lstnew(p));
}

// each list_row will have an array of points, each point will describe x, y, z and color
t_list *get_rows(char *file_name)
{
	int	fd;
	int	row;
	int	row_len;
	t_list *row_head;
	t_list *row_last;
	
	if (ft_strncmp(ft_strchr(file_name, '.') + 1, "fdf", 4) != 0)
		// print error for when file is not a .fdf 
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		// print error for when file doesnt exist
	row = get_next_line(fd);
	row_len = ft_strlen(row_len);
	row_head = create_row(row, row_len);
	while (row)
	{
		
		row = get_next_line(fd);
		row_last = create_row(row, row_len);
	}
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