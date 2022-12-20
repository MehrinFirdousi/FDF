/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 16:48:31 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/20 21:17:07 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	set_rot_angles(double *x, int angle, double *sinx, double *cosx)
{
	double	rad;

	*x = fmod(*x + angle, 360);
	rad = *x * (M_PI / 180.0);
	*sinx = sin(rad);
	*cosx = cos(rad);
}

int	check_rotate(int keycode, t_mlx *m)
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
	else
		return (1);
	redraw_image(m);
	return (0);
}

int	key_hold_handler(int keycode, t_mlx *m)
{
	if (check_rotate(keycode, m))
	{
		if (keycode == UP)
			m->y_offset -= SPEED;
		else if (keycode == LEFT)
			m->x_offset -= SPEED;
		else if (keycode == DOWN)
			m->y_offset += SPEED;
		else if (keycode == RIGHT)
			m->x_offset += SPEED;
		else if (keycode == PLUS)
			m->scale += ZOOM;
		else if (keycode == MINUS && m->scale > 1)
			m->scale -= ZOOM;
		else if (keycode == ENTER)
			dvd_translate(m);
		else
			return (1);
		redraw_image(m);
	}
	return (0);
}

int	key_click_handler(int keycode, t_mlx *m)
{
	if (keycode == ESC)
	{
		mlx_destroy_window(m->mlx, m->win);
		ft_lstclear(&m->lst, &free);
		exit(0);
	}
	if (keycode == TWO)
	{
		m->cur_projection = 2;
		redraw_image(m);
	}
	if (keycode == THREE)
	{
		m->cur_projection = 3;
		redraw_image(m);
	}
	return (0);
}

void	redraw_image(t_mlx *m)
{
	mlx_destroy_image(m->mlx, m->img->img);
	m->img->img = mlx_new_image(m->mlx, WIN_WIDTH, WIN_HEIGHT);
	m->img->addr = mlx_get_data_addr(m->img->img, &(m->img->bits_per_pixel), \
									&(m->img->line_length), &(m->img->endian));
	if (m->cur_projection == 3)
		draw_image(m, transform_3d);
	else if (m->cur_projection == 2)
		draw_image(m, transform_2d);
	mlx_put_image_to_window(m->mlx, m->win, m->img->img, 0, 0);
}
