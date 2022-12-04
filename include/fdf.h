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
// # define WIN_HEIGHT 1080
# define WIN_HEIGHT 920
# define WHITE 16777215
# define TRANS_RATE 10
# ifdef __APPLE__
#  define ESC 53
#  define T 17
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
# endif

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
	t_data	*img;
	t_list	*lst;
	// these 5 can be in their own struct
	unsigned int	scale; // lower scale -> zoomed out
	double	a; // rotation angle alpha
	double	b; // rotation angle beta
	int		x_offset;
	int		y_offset;
}				t_mlx;

t_list	*create_row(char **point_strs, int count_points, int y, int *z_max);
void	free_points(void *points);
int		get_coordinates(char *file_name, t_list **lst);
int		hex_to_dec(char *hex);

#endif