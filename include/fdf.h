/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:31:41 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/20 21:36:17 by mfirdous         ###   ########.fr       */
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
# define ROT_ANGLE 7
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

typedef struct s_point
{
	int	x;
	int	y;
	int	z;
	int	x_3d;
	int	y_3d;
	int	z_3d;
	int	color;
}				t_point;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct s_mlx
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
	int		cur_projection;
}				t_mlx;

typedef struct s_dda_dat
{
	int		dx;
	int		dy;
	int		dx_abs;
	int		dy_abs;
	int		steps;
	float	x_inc;
	float	y_inc;
}				t_dda_dat;

// parsing
t_list	*create_row(char **point_strs, int count_points, int y, int *z_max);
void	free_points(void *points);
void	get_coordinates(int fd, t_list **lst, t_mlx *mlx);
int		hex_to_dec(char *hex);

// drawing
void	dda(t_mlx *m, t_point p1, t_point p2);
void	transform_3d(t_point *p, t_mlx *m);
void	transform_2d(t_point *p, t_mlx *mlx);
void	draw_lines(t_list *node, t_mlx *mlx, void (*t)(t_point *, t_mlx *));
void	draw_image(t_mlx *mlx, void (*transform)(t_point *, t_mlx *));

// key handling
void	set_rot_angles(double *x, int angle, double *sinx, double *cosx);
int		check_rotate(int keycode, t_mlx *m);
int		key_hold_handler(int keycode, t_mlx *m);
int		key_click_handler(int keycode, t_mlx *m);
void	redraw_image(t_mlx *m);

// dvd translation
void	check_window_edge(t_point p, t_mlx *m);
void	change_direction(t_mlx *m);
void	dvd_translate(t_mlx *mlx);

void	my_mlx_pixel_put(t_data *data, int x, int y, unsigned int color);
void	mlx_set_up(t_mlx *mlx, t_data *img);

#endif