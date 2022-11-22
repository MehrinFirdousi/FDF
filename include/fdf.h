/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:31:41 by mfirdous          #+#    #+#             */
/*   Updated: 2022/11/22 20:55:20 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H

# define FDF_H
# include "libft.h"
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# define WHITE 16777215

typedef struct	s_point
{
	int	x;
	int	y;
	int	z;
	int	value;
	int	color;
}				t_point;

typedef struct	s_data 
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

t_list	*create_row(char **point_strs, int count_points);
void	free_points(void *points);
t_list	*get_rows(char *file_name);
int		hex_to_dec(char *hex);

#endif