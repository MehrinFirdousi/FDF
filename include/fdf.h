/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:31:41 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/03 17:23:15 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H

# define FDF_H
# include "libft.h"
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# define WIN_WIDTH 1280
# define WIN_HEIGHT 1080
# define WHITE 16777215
# define ESC 53
typedef struct	s_point
{
	int	x;
	int	y;
	int	z;
	int	x_3d;
	int	y_3d;
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

typedef struct	s_mlx
{
	void	*mlx;
	void	*win;
}				t_mlx;

t_list	*create_row(char **point_strs, int count_points, int y);
void	free_points(void *points);
int		get_coordinates(char *file_name, t_list **lst);
int		hex_to_dec(char *hex);

#endif