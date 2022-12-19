/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:31:41 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/19 20:46:57 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H

# define FDF_H
# include "libft.h"
# include <mlx.h>
# include <math.h>
# include <fcntl.h>
# define WIN_WIDTH 1580
// # define WIN_HEIGHT 720
# define WIN_HEIGHT 980
# define WW WIN_WIDTH
# define WH WIN_HEIGHT
# define WHITE 16777215
# define SPEED 35
# define DVD_SPEED 10
# define ROT_ANGLE 10
# define ZOOM 0.5
# define HEX_MAX 16777216

# ifdef __APPLE__
#  define ESC 53
#  define TWO	19
#  define THREE	20
#  define LEFT	123
#  define UP	126
#  define RIGHT	124
#  define DOWN	125
#  define PLUS	24
#  define MINUS	27
#  define W		13
#  define A		0
#  define S		1
#  define D		2
#  define Q		12
#  define E		14
#  define ENTER 36

# else
#  define ESC	65307
#  define TWO	50
#  define THREE	51
#  define LEFT	65361
#  define UP	65362
#  define RIGHT	65363
#  define DOWN	65364
#  define PLUS	61
#  define MINUS	45
#  define W		119
#  define A		97
#  define S		115
#  define D		100
#  define Q		113
#  define E		101
#  define ENTER 65293
# endif

typedef struct	s_point
{
	int	x;
	int	y;
	int	z;
	int	x_3d;
	int	y_3d;
	int	z_3d;
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
	t_data	*img;
	t_list	*lst;
	double	scale;
	double	a;
	double	b;
	double	c;
	int		x_offset;
	int		y_offset;
	int		z_max;
	int		x_max;
	int		y_max;
	double	sina;
	double	sinb;
	double	sinc;
	double	cosa;
	double	cosb;
	double	cosc;
	int		dir;
	int		color_change;
}				t_mlx;

t_list	*create_row(char **point_strs, int count_points, int y, int *z_max);
void	free_points(void *points);
void	get_coordinates(char *file_name, t_list **lst, t_mlx *mlx);
int		hex_to_dec(char *hex);
void	transform_3d(t_point *point, t_mlx *mlx);
double	deg_to_rad(double deg);
void	matrix_mul(t_mlx *mlx);
void	rotate(t_point *point, t_mlx *mlx);
void	rotate_x(t_point *point, t_mlx *mlx);
void	rotate_y(t_point *p, t_mlx *mlx);
void	rotate_z(t_point *p, t_mlx *mlx);

#endif