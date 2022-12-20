/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:24:28 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/20 16:32:02 by mfirdous         ###   ########.fr       */
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

double	deg_to_rad(double deg)
{
	return (deg * (M_PI / 180.0));
}

// rotation matrices used are for right-handed coordinate system i.e., the one we have on the fdf window
void	transform_3d(t_point *p, t_mlx *mlx)
{
	// int z_scaled;
	double a;
	double b;
	
	p->x_3d = (p->x * mlx->scale)  - ((mlx->x_max / 2) * mlx->scale);
	p->y_3d = (p->y * mlx->scale)  - ((mlx->y_max / 2) * mlx->scale);
	p->z_3d = (p->z * mlx->scale)  - ((mlx->z_max / 2) * mlx->scale);
	
	a = deg_to_rad(mlx->a);
	b = deg_to_rad(mlx->b);
	
	int x = p->x_3d;
	int y = p->y_3d;
	int z = p->z_3d;
	
	p->x_3d = (x * cos(b)) + (y * sin(b)); // along x axis - clockwise
	p->y_3d = -(x * sin(b) * cos(a)) + (y * cos(a) * cos(b)) + (-z * sin(a)); // along z axis
	p->z_3d = (x * sin(a) * sin(b)) + (y * -sin(a) * cos(b)) + (-z * cos(a)); // along z axis

	p->x_3d += mlx->x_offset + ((mlx->x_max / 2) * mlx->scale);
	p->y_3d += mlx->y_offset + ((mlx->y_max / 2) * mlx->scale);
	p->z_3d += ((mlx->z_max / 2) * mlx->scale);
	// point->z_3d += mlx->y_offset + 5 * mlx->scale;
}

/*void	transform_3d_xy(t_point *point, t_mlx * mlx)
{
	int z_scaled;
	double a;
	double b;
	double sina, sinb, cosa, cosb;
	
	point->x_3d = (point->x * mlx->scale) + 100;
	point->y_3d = (point->y * mlx->scale) + 100;
	z_scaled = (point->z * mlx->scale) + 100;
	// a = asin(tan(deg_to_rad(30)));
	a = deg_to_rad(mlx->a);
	b = deg_to_rad(mlx->b);
	sina = sin(a);
	sinb = sin(b);
	cosa = cos(a);
	cosb = cos(b);

	point->x_3d = (point->x_3d * cosb) + (z_scaled * sinb); // x axis - counterclockwise
	point->y_3d = (point->x_3d * sina * sinb) + (point->y_3d * cosa) - (z_scaled * sina * cosb); // y axis 
	
	point->x_3d += mlx->x_offset - 100;
	point->y_3d += mlx->y_offset - 100;
}*/

void	transform_2d(t_point *point, t_mlx *mlx)
{
	point->x_3d = point->x * mlx->scale;
	point->y_3d = point->y * mlx->scale;
}

void	draw_image(t_mlx *mlx, void (*transform)(t_point *, t_mlx *))
{
	t_list	*node;
	t_point *p1;
	t_point	*p2;
	int		i;
	
	node = mlx->lst;
	i = -1;
	p1 = (t_point *)node->content;
	while(p1[++i].color != -1)
		transform(&p1[i], mlx);
	while (node->next)
	{
		p1 = (t_point *)node->content;
		p2 = (t_point *)node->next->content;
		i = -1;
		while(p1[++i + 1].color != -1)
		{
			transform(&p2[i], mlx);
			dda(mlx->img, p1[i], p1[i + 1]);
			dda(mlx->img, p1[i], p2[i]);
		}
		transform(&p2[i], mlx);
		dda(mlx->img, p1[i], p2[i]);
		node = node->next;
	}
	i = -1;
	p1 = (t_point *)node->content;
	while(p1[++i + 1].color != -1)
		dda(mlx->img, p1[i], p1[i + 1]);
}

void	mlx_set_up(t_mlx *mlx, t_data *img)
{
	mlx->mlx = mlx_init();
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, "FDF");
	img->img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
	mlx->img = img;
	mlx->a = 35.264;
	mlx->b = 45;
	mlx->c = 0;
	mlx->x_offset = 0;
	// mlx->y_offset = WIN_HEIGHT * 0.85;
	mlx->y_offset = 0;
	mlx->rx = 0;
	mlx->ry = 0;
	mlx->rz = 0;
}

void	clear_image(t_data *img)
{
	int		x;
	int		y;
	char	*dst;
	x = -1;
	while (++x < WIN_WIDTH)
	{
		y = -1;
		while (++y < WIN_HEIGHT)
		{
			dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
			*(unsigned int*)dst = 0;
		}
	}
}

void	redraw_image(t_mlx *mlx, void (*transform)(t_point *, t_mlx *), char axis)
{
	mlx_destroy_image(mlx->mlx, mlx->img->img);
	mlx->img->img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	mlx->img->addr = mlx_get_data_addr(mlx->img->img, &(mlx->img->bits_per_pixel), &(mlx->img->line_length), &(mlx->img->endian));
	// clear_image(mlx->img);
	(void)axis;
	draw_image(mlx, transform);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img->img, 0, 0);
}

int	key_click_handler(int keycode, t_mlx *vars)
{
	if (keycode == ESC)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		ft_lstclear(&vars->lst, &free);
		exit(0);
	}
	if (keycode == TWO)
		redraw_image(vars, transform_2d, 'n');
	if (keycode == THREE)
		redraw_image(vars, transform_3d, 'n');
	// printf("keycode = %d\n", keycode);
	return (0);
}

int	key_hold_handler(int keycode, t_mlx *vars)
{
	// void (*func)(t_point *, t_mlx *);
	if (keycode == RIGHT)
	{
		vars->ry += ROT_ANGLE;
		// vars->b = fmod(vars->b + ROT_ANGLE, 360);
		redraw_image(vars, rotate_y, 'y');
		return (0);
	}
	else if (keycode == LEFT)
	{
		vars->ry -= ROT_ANGLE;
		// vars->b = fmod(vars->b - ROT_ANGLE, 360);
		redraw_image(vars, rotate_y, 'y');
		return (0);
	}
	else if (keycode == DOWN)
	{
		vars->rx += ROT_ANGLE;
		// vars->a = fmod(vars->a - ROT_ANGLE, 360);
		redraw_image(vars, rotate_x, 'x');
		return (0);
	}
	else if (keycode == UP)
	{
		vars->rx -= ROT_ANGLE;
		// vars->a = fmod(vars->a + ROT_ANGLE, 360);
		redraw_image(vars, rotate_x, 'x');
		return (0);
	}
	else if (keycode == E)
	{
		vars->rz += ROT_ANGLE;
		// vars->c = fmod(vars->c + ROT_ANGLE, 360);
		redraw_image(vars, rotate_z, 'z');
		return (0);
	}
	else if (keycode == Q)
	{
		vars->rz -= ROT_ANGLE;
		// vars->c = fmod(vars->c + ROT_ANGLE, 360);
		redraw_image(vars, rotate_z, 'z');
		return (0);
	}
	else if (keycode == PLUS)
		vars->scale++;
	else if (keycode == MINUS && vars->scale > 1)
		vars->scale--;
	else if (keycode == W)
		vars->y_offset -= SPEED;
	else if (keycode == A)
		vars->x_offset -= SPEED;
	else if (keycode == S)
		vars->y_offset += SPEED;
	else if (keycode == D)
		vars->x_offset += SPEED;
	else
		return (1);
	redraw_image(vars, transform_3d, 'n');
	// printf("keycode2 = %d\n", keycode);
	return (0);
}

void display_matrix(double m[3][3])
{
	int i;
	int j;

	i = -1;
	while (++i < 3)
	{
		j = -1;
		while (++j < 3)
			printf("%lf ", m[i][j]);
		printf("\n");
	}
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
	
	get_coordinates(argv[1], &mlx.lst, &mlx);
	mlx_set_up(&mlx, &img);
	printf("scale = %d\n", mlx.scale);
	
	draw_image(&mlx, transform_3d);
	mlx_key_hook(mlx.win, key_click_handler, &mlx);
	mlx_hook(mlx.win, 2, 1L<<0, key_hold_handler, &mlx);
	mlx_put_image_to_window(mlx.mlx, mlx.win, img.img, 0, 0);
	mlx_loop(mlx.mlx);
	return (0);	
}
