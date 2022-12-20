/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dvd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 21:19:06 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/20 21:19:55 by mfirdous         ###   ########.fr       */
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

void	dvd_translate(t_mlx *mlx)
{
	change_direction(mlx);
	if (mlx->dir == 1)
	{
		mlx->x_offset -= DVD_SPEED;
		mlx->y_offset -= DVD_SPEED;
	}
	else if (mlx->dir == 2)
	{
		mlx->x_offset += DVD_SPEED;
		mlx->y_offset -= DVD_SPEED;
	}
	else if (mlx->dir == 3)
	{
		mlx->x_offset += DVD_SPEED;
		mlx->y_offset += DVD_SPEED;
	}
	else if (mlx->dir == 4)
	{
		mlx->x_offset -= DVD_SPEED;
		mlx->y_offset += DVD_SPEED;
	}
}
