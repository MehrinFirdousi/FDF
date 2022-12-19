/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:24:28 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/16 21:42:39 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// sorta works 

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

void dda(t_mlx *m, t_point p1, t_point p2)
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
		my_mlx_pixel_put(m->img, X, Y, (p1.color * m->color_change) % HEX_MAX);
		X += Xinc; // increment in x at each step
		Y += Yinc; // increment in y at each step
	}
}

// rotation matrices used are for right-handed coordinate system i.e., the one we have on the fdf window
void	transform_3d(t_point *p, t_mlx *mlx)
{
	int x;
	int y;
	int z;

	x = (p->x - (mlx->x_max / 2)) * mlx->scale;
	y = (p->y - (mlx->y_max / 2)) * mlx->scale;
	z = (p->z - (mlx->z_max / 2)) * mlx->scale;

	p->x_3d = (x * mlx->cosb * mlx->cosc) + (y * mlx->sinb) + (-z * -mlx->sinc * mlx->cosb); // along x axis - clockwise
	p->y_3d = (x * ((-mlx->sinb * mlx->cosa * mlx->cosc) + (mlx->sina * mlx->sinc))) + (y * mlx->cosa * mlx->cosb) + (-z * ((mlx->sinb * mlx->cosa * mlx->sinc) + (mlx->sina * mlx->cosc))); // along z axis
	p->z_3d = (x * mlx->sina * mlx->sinb * mlx->cosc) + (y * -mlx->sina * mlx->cosb) + (-z * ((-mlx->sina * mlx->sinb * mlx->sinc) + (mlx->cosa * mlx->cosc))); // along z axis

	// p->x_3d = x * mlx->cosb * mlx->cosc;
	// p->x_3d += y * mlx->sinb;
	// p->x_3d += -z * -mlx->sinc * mlx->cosb;
	// p->y_3d = x * ((-mlx->sinb * mlx->cosa * mlx->cosc) + (mlx->sina * mlx->sinc));
	// p->y_3d += y * mlx->cosa * mlx->cosb;
	// p->y_3d += -z * ((mlx->sinb * mlx->cosa * mlx->sinc) + (mlx->sina * mlx->cosc));
	// p->z_3d = x * mlx->sina * mlx->sinb * mlx->cosc;
	// p->z_3d += y * -mlx->sina * mlx->cosb;
	// p->z_3d += -z * ((-mlx->sina * mlx->sinb * mlx->sinc) + (mlx->cosa * mlx->cosc));
	
	// p->x_3d += (mlx->x_max / 2) * mlx->scale;
	// p->y_3d += (mlx->y_max / 2) * mlx->scale;
	// p->z_3d += (mlx->z_max / 2) * mlx->scale;

	p->x_3d += mlx->x_offset;
	p->y_3d += mlx->y_offset;
}

void	transform_2d(t_point *p, t_mlx *mlx)
{
	p->x_3d = p->x * mlx->scale;
	p->y_3d = p->y * mlx->scale;
}

void	put_coordinates(t_mlx *mlx, void (*transform)(t_point *, t_mlx *))
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
			// p2[mlx->z_offset].z += (mlx->z_max + 5); // for the z wave
			// p1[mlx->z_offset].z += mlx->z_max + 5;
			transform(&p2[i], mlx);
			dda(mlx, p1[i], p1[i + 1]);
			dda(mlx, p1[i], p2[i]);
			// p1[mlx->z_offset].z -= mlx->z_max + 5;
			// p2[mlx->z_offset].z -= mlx->z_max + 5;
		}
		transform(&p2[i], mlx);
		dda(mlx, p1[i], p2[i]);
		node = node->next;
	}
	i = -1;
	p1 = (t_point *)node->content;
	while(p1[++i + 1].color != -1)
		dda(mlx, p1[i], p1[i + 1]);
}

void	mlx_set_up(t_mlx *mlx, t_data *img)
{
	mlx->mlx = mlx_init();
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, "FDF");
	img->img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
	mlx->img = img;
	mlx->x_offset = (WIN_WIDTH / 2);
	mlx->y_offset = (WIN_HEIGHT / 2);
	mlx->a = 35.264;
	mlx->b = 45;
	mlx->c = 0;
	mlx->sina = sin(mlx->a * (M_PI / 180.0));
	mlx->cosa = cos(mlx->a * (M_PI / 180.0));
	mlx->sinb = sin(mlx->b * (M_PI / 180.0));
	mlx->cosb = cos(mlx->b * (M_PI / 180.0));
	mlx->sinc = sin(mlx->c * (M_PI / 180.0));
	mlx->cosc = cos(mlx->c * (M_PI / 180.0));
	mlx->direction = 1;
	mlx->color_change = 1;
}

void	redraw_image(t_mlx *mlx, void (*transform)(t_point *, t_mlx *))
{
	mlx_destroy_image(mlx->mlx, mlx->img->img);
	mlx->img->img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	mlx->img->addr = mlx_get_data_addr(mlx->img->img, &(mlx->img->bits_per_pixel), &(mlx->img->line_length), &(mlx->img->endian));
	put_coordinates(mlx, transform);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img->img, 0, 0);
}

// check for leaks
int exit_free(t_mlx *m)
{
    ft_lstclear(&m->lst, &free);
    exit(0);
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
		redraw_image(vars, transform_2d);
	if (keycode == THREE)
		redraw_image(vars, transform_3d);
	// printf("keycode = %d\n", keycode);
	return (0);
}

void	set_rot_angles(double *x, int angle, double *sinx, double *cosx)
{
	double rad;
	
	*x = fmod(*x + angle, 360);
	rad = *x * (M_PI / 180.0);
	*sinx = sin(rad);
	*cosx = cos(rad);
}

/*
- - top-left     \. 1
- + bottom-left  /  4
+ - top-right   ./  2
+ + bottom-right \  3

x y    	   x'y'

		x	   y
- - => + - or - +		001 => 2 or 4
+ - => - - or + +		102 => 1 or 3
+ + => - + or + -		113 => 4 or 2
- + => + + or - -		014 => 3 or 1

*/

void	change_direction(t_mlx *mlx)
{
	t_list	*node;
	t_point *p;
	int i;
	int edge_reached;

	node = mlx->lst;
	edge_reached = 0;
	while (node)
	{
		p = (t_point *)node->content;
		i = -1;
		while(p[++i].color != -1)
			if (p[i].x_3d <= 0 || p[i].y_3d <= 0 || p[i].x_3d >= WIN_WIDTH || p[i].y_3d >= WIN_HEIGHT)
			{
				if ((p[i].x_3d >=  WIN_WIDTH && mlx->direction == 3) || (p[i].y_3d <= 0 && mlx->direction == 1))
					mlx->direction = 4;
				else if ((p[i].x_3d <= 0 && mlx->direction == 4) || (p[i].y_3d <= 0 && mlx->direction == 2))
					mlx->direction = 3;
				else if ((p[i].x_3d <= 0 && mlx->direction == 1) || (p[i].y_3d >= WIN_HEIGHT && mlx->direction == 3))
					mlx->direction = 2;
				else if ((p[i].x_3d >= WIN_WIDTH && mlx->direction == 2) || (p[i].y_3d >= WIN_HEIGHT && mlx->direction == 4))
					mlx->direction = 1;
				// mlx->direction = (mlx->direction % 4) + 1;
				edge_reached = 1;
				mlx->color_change++;
				break;
			}
		if (edge_reached)
			break;
		node = node->next;
	}
	if (mlx->direction == 1)
	{
		mlx->x_offset -= DVD_SPEED;
		mlx->y_offset -= DVD_SPEED;
	}
	else if (mlx->direction == 2)
	{
		mlx->x_offset += DVD_SPEED;
		mlx->y_offset -= DVD_SPEED;
	}
	else if (mlx->direction == 3)
	{
		mlx->x_offset += DVD_SPEED;
		mlx->y_offset += DVD_SPEED;
	}
	else if (mlx->direction == 4)
	{
		mlx->x_offset -= DVD_SPEED;
		mlx->y_offset += DVD_SPEED;
	}
}

int	key_hold_handler(int keycode, t_mlx *m)
{
	if (keycode == E)
		set_rot_angles(&m->b, -ROT_ANGLE, &m->sinb, &m->cosb);
	else if (keycode == Q)
		set_rot_angles(&m->b, ROT_ANGLE, &m->sinb, &m->cosb);
	else if (keycode == S)
		set_rot_angles(&m->a, -ROT_ANGLE, &m->sina, &m->cosa);
	else if (keycode == W)
		set_rot_angles(&m->a, ROT_ANGLE, &m->sina, &m->cosa);
	else if (keycode == A)
		set_rot_angles(&m->c, -ROT_ANGLE, &m->sinc, &m->cosc);
	else if (keycode == D)
		set_rot_angles(&m->c, ROT_ANGLE, &m->sinc, &m->cosc);
	else if (keycode == PLUS)
		m->scale += ZOOM;
	else if (keycode == MINUS && m->scale > 1)
		m->scale -= ZOOM;
	else if (keycode == UP)
		m->y_offset -= SPEED;
	else if (keycode == LEFT)
		m->x_offset -= SPEED;
	else if (keycode == DOWN)
		m->y_offset += SPEED;
	else if (keycode == RIGHT)
		m->x_offset += SPEED;
	else if (keycode == ENTER)
		change_direction(m);
	else
		return (1);
	redraw_image(m, transform_3d);
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
	
	get_coordinates(argv[1], &mlx.lst, &mlx);
	mlx_set_up(&mlx, &img);
	put_coordinates(&mlx, transform_3d);
	mlx_key_hook(mlx.win, key_click_handler, &mlx);
	mlx_hook(mlx.win, 2, 1L<<0, key_hold_handler, &mlx);
	mlx_hook(mlx.win, 17, 0, exit_free, &mlx);
	mlx_put_image_to_window(mlx.mlx, mlx.win, img.img, 0, 0);
	mlx_loop(mlx.mlx);
	return (0);	
}
