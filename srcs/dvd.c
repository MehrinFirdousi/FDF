/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dvd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 21:19:06 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/21 19:48:31 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	check_window_edge(t_point p, t_mlx *m)
{
	if ((p.x_3d >= WW && m->dir == 3) || (p.y_3d <= 0 && m->dir == 1))
		m->dir = 4;
	else if ((p.x_3d <= 0 && m->dir == 4) || (p.y_3d <= 0 && m->dir == 2))
		m->dir = 3;
	else if ((p.x_3d <= 0 && m->dir == 1) || (p.y_3d >= WH && m->dir == 3))
		m->dir = 2;
	else if ((p.x_3d >= WW && m->dir == 2) || (p.y_3d >= WH && m->dir == 4))
		m->dir = 1;
}

void	change_direction(t_mlx *m)
{
	t_point	*p;
	t_list	*node;
	int		old_dir;
	int		i;

	node = m->lst;
	while (node)
	{
		p = (t_point *)node->content;
		i = -1;
		old_dir = m->dir;
		while (p[++i].color != -1)
		{
			check_window_edge(p[i], m);
			if (m->dir != old_dir)
			{
				m->color_change++;
				return ;
			}
		}
		node = node->next;
	}
}

void	change_offset(t_mlx *m, int x_speed, int y_speed)
{
	m->x_offset += x_speed;
	m->y_offset += y_speed;
}

int	dvd_translate(t_mlx *mlx)
{
	if (mlx->dvd_translate)
	{
		change_direction(mlx);
		if (mlx->dir == 1)
			change_offset(mlx, -DVD_SPEED, -DVD_SPEED);
		else if (mlx->dir == 2)
			change_offset(mlx, DVD_SPEED, -DVD_SPEED);
		else if (mlx->dir == 3)
			change_offset(mlx, DVD_SPEED, DVD_SPEED);
		else if (mlx->dir == 4)
			change_offset(mlx, -DVD_SPEED, DVD_SPEED);
		redraw_image(mlx);
	}
	return (0);
}

void	transform_cabinet(t_point *p, t_mlx *m)
{
	int	x;
	int	y;
	int	z;

	x = (p->x - (m->x_max / 2)) * m->scale;
	y = (p->y - (m->y_max / 2)) * m->scale;
	z = -(p->z - (m->z_max / 2)) * m->scale;
	p->x_3d = x + 0.5 * z * m->cosb;
	p->y_3d = y + 0.5 * z * m->sinb;
	p->z_3d = 0;
	p->x_3d += m->x_offset;
	p->y_3d += m->y_offset;
}
