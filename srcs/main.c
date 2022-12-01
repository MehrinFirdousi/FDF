/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:24:28 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/01 23:11:51 by mfirdous         ###   ########.fr       */
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

void	display_row(void *points)
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

void	transform_3d(t_point *point, double sq2, double sq6, int scale)
{
	point->x_3d = point->x * scale;
	point->y_3d = point->y * scale;
	int temp = point->z * scale;
	point->y_3d = (point->y_3d - temp) / sq2;
	point->x_3d = (point->y_3d + 2 * point->x_3d + temp) / sq6;
	// point->x_3d = (point->x - point->z) / sq2;
	// point->y_3d = (point->x + 2 * point->y + point->z) / sq6;
	// printf("(%d, %d, %d) -> (%d, %d)\n", point->x, point->y, point->z, point->x_3d, point->y_3d);
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
	(void)argc;
	(void)argv;
	
	if (argc != 2)
	{
		ft_printf("Usage : %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	t_mlx	mlx;
	t_data	img;
	
	// initializing a window and making it remain open 
	mlx_set_up(&mlx, &img);
	
	t_list *res;
	int scale;
	
	scale = get_coordinates(argv[1], &res);
	printf("scale = %d\n", scale);
	// ft_lstiter(res, &display_row);
	put_coordinates(&img, res, scale);
	// ft_lstiter(res, &display_row);
	ft_lstclear(&res, &free);
	
	mlx_hook(mlx.win, 2, 1L<<0, key_handler, &mlx);
	mlx_put_image_to_window(mlx.mlx, mlx.win, img.img, 0, 0);
	mlx_loop(mlx.mlx);
}

