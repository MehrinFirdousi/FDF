/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:24:28 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/03 19:41:18 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, unsigned int color)
{
	char	*dst;

	if (x >= 0 && y >= 0 && x < WIN_WIDTH && y < WIN_HEIGHT)
	{
		dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
		*(unsigned int*)dst = color;
	}
}

// void dda(t_data *img, t_point p1, t_point p2)
// {
//     int dx;
//     int dy;
 
// 	dx = p2.x - p1.x;
// 	dy = p2.y - p1.y;
//     int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
 
//     float Xinc = dx / (float)steps;
//     float Yinc = dy / (float)steps;
 
//     float X = p1.x;
//     float Y = p1.y;
//     for (int i = 0; i <= steps; i++) 
// 	{
//         my_mlx_pixel_put(img, X, Y, p1.color);
//         X += Xinc; // increment in x at each step
//         Y += Yinc; // increment in y at each step
//     }
// }
void dda(t_data *img, t_point p1, t_point p2)
{
    int dx;
    int dy;
 
	dx = p2.x_3d - p1.x_3d;
	dy = p2.y_3d - p1.y_3d;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
 
    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;
 
    float X = p1.x_3d;
    float Y = p1.y_3d;
    for (int i = 0; i <= steps; i++) 
	{
        my_mlx_pixel_put(img, X, Y, p1.color);
        X += Xinc; // increment in x at each step
        Y += Yinc; // increment in y at each step
    }
}

void	display_row(void *points) // util only - delete later
{
	t_point *p;
	int i;
	
	p = (t_point *)points;
	i = -1;
	while(p[++i].color != -1)
	{
		printf("%d,", p[i].x);
		printf("%d,", p[i].y);
		if (p[i].color != WHITE)
			printf("\033[1;31m");
		printf("%d\t", p[i].z);
		printf("\033[0m");
		// printf("%d\t", p[i].color);
	}
	printf("\n");
}

void	display_row_3d(void *points) // util only - delete later
{
	t_point *p;
	int i;
	
	p = (t_point *)points;
	i = -1;
	while(p[++i].color != -1)
	{
		printf("%d,", p[i].x_3d);
		printf("%d\t", p[i].y_3d);
	}
	printf("\n");
}



void	transform_3d_old(t_point *point, double sq2, double sq6, int scale)
{
	int z_scaled;
	
	point->x_3d = point->x * scale;
	point->y_3d = point->y * scale;
	z_scaled = point->z * scale;
	point->y_3d = (point->y_3d - z_scaled) / sq2; 
	point->x_3d = (point->y_3d + 2 * point->x_3d + z_scaled) / sq6; // works for now but not really a fix
	// point->x_3d = (point->x_3d - z) / sq2;
	// point->y_3d = (point->x_3d + 2 * point->y_3d + z) / sq6;
}

// rotation matrices used are for right-handed coordinate system i.e., the one we have on the fdf window
void	transform_3d(t_point *point, double sq2, double sq6, int scale)
{
	(void)sq2;
	(void)sq6;
	int z_scaled;
	double a;
	double b;
	double sina, sinb, cosa, cosb;
	
	point->x_3d = (point->x * scale) + 1000;
	point->y_3d = (point->y * scale) + 200;
	z_scaled = (point->z * scale) + 200;
	
	a = asin(tan(0.523599)); // 30 in rad
	// a = 0.615472907; // 34.264 in rad
	b = 0.785398; // 45 in rad
	sina = sin(a);
	sinb = sin(b);
	cosa = cos(a);
	cosb = cos(b);
	// point->x_3d = (point->x_3d * cosb) - (z_scaled * sinb); // x axis 
	// point->y_3d = (point->x_3d * sina * sinb) + (point->y_3d * cosa) + (z_scaled * sina * cosb); // y axis 
	
	// point->x_3d = (point->x_3d * cosb) + (z_scaled * sinb); // x axis - counterclockwise
	// point->y_3d = (point->x_3d * sina * sinb) + (point->y_3d * cosa) - (z_scaled * sina * cosb); // y axis 
	
	point->x_3d = (point->x_3d * cosb) - (point->y_3d * sinb); // along x axis - counterclockwise
	point->y_3d = (point->x_3d * sinb * cosa) + (point->y_3d * cosa * cosb) - (z_scaled * sina); // along z axis

	// point->x_3d = (point->x_3d * cosa * cosb) - (point->y_3d * sinb * cosa) - (z_scaled * sina); // along y axis - counterclockwise
	// point->y_3d = (point->x_3d * sinb) + (point->y_3d * cosb); // along z axis
}

void	transform_2d(t_point *point, int scale)
{
	point->x_3d = point->x * scale;
	point->y_3d = point->y * scale;
}

void	put_coordinates(t_data *img, t_list *lst, int scale)
{
	t_list	*node;
	t_point *p1;
	t_point	*p2;
	double	sq2;
	double	sq6;
	int		i;
	
	node = lst;
	i = -1;
	sq2 = sqrt(2);
	sq6 = sqrt(6);
	p1 = (t_point *)node->content;
	while(p1[++i].color != -1)
		transform_3d(&p1[i], sq2, sq6, scale);
	while (node->next)
	{
		p1 = (t_point *)node->content;
		p2 = (t_point *)node->next->content;
		i = -1;
		while(p1[++i + 1].color != -1)
		{
			transform_3d(&p2[i], sq2, sq6, scale);
			dda(img, p1[i], p1[i + 1]);
			dda(img, p1[i], p2[i]);
		}
		transform_3d(&p2[i], sq2, sq6, scale);
		dda(img, p1[i], p2[i]);
		node = node->next;
	}
	i = -1;
	p1 = (t_point *)node->content;
	while(p1[++i + 1].color != -1)
		dda(img, p1[i], p1[i + 1]);
}

void	put_coordinates_2d(t_data *img, t_list *lst, int scale)
{
	t_list	*node;
	t_point *p1;
	t_point	*p2;
	int		i;
	
	node = lst;
	i = -1;
	p1 = (t_point *)node->content;
	while(p1[++i].color != -1)
		transform_2d(&p1[i], scale);
	while (node->next)
	{
		p1 = (t_point *)node->content;
		p2 = (t_point *)node->next->content;
		i = -1;
		while(p1[++i + 1].color != -1)
		{
			transform_2d(&p2[i], scale);
			dda(img, p1[i], p1[i + 1]);
			dda(img, p1[i], p2[i]);
		}
		transform_2d(&p2[i], scale);
		dda(img, p1[i], p2[i]);
		node = node->next;
	}
	i = -1;
	p1 = (t_point *)node->content;
	while(p1[++i + 1].color != -1)
		dda(img, p1[i], p1[i + 1]);
}

void	transform_all(t_list *lst, int scale)
{
	t_list	*node;
	t_point *p;
	double	sq2;
	double	sq6;
	int i;

	node = lst;
	sq2 = sqrt(2);
	sq6 = sqrt(6);
	while (node)
	{
		p = (t_point *)node->content;
		i = -1;
		while(p[++i].color != -1)
			transform_3d(&p[i], sq2, sq6, scale);
		node = node->next;
	}
}

void	mlx_set_up(t_mlx *mlx, t_data *img)
{
	mlx->mlx = mlx_init();
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, "FDF");
	img->img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
}

int	key_handler(int keycode, t_mlx *vars, t_data *img, t_list *res, int scale)
{
	if (keycode == ESC)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	if (keycode == 17)
	{
		put_coordinates_2d(img, res, scale);
		// doesnt work, how to pass parameters to key handler??????????????
	}
	(void)img;
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_printf("Usage : %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	t_mlx	mlx;
	t_data	img;
	
	mlx_set_up(&mlx, &img);
	
	t_list *lst;
	int		scale;
	
	scale = get_coordinates(argv[1], &lst);
	printf("scale = %d\n", scale);
	// ft_lstiter(lst, &display_row);
	put_coordinates(&img, lst, scale);
	// transform_all(lst, scale);
	// printf("\n");
	// ft_lstiter(lst, &display_row_3d);
	ft_lstclear(&lst, &free);
	
	mlx_hook(mlx.win, 2, 1L<<0, key_handler, &mlx);
	mlx_put_image_to_window(mlx.mlx, mlx.win, img.img, 0, 0);
	mlx_loop(mlx.mlx);
}

