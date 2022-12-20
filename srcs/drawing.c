/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 16:53:41 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/20 22:12:15 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	dda(t_mlx *m, t_point p1, t_point p2)
{
	int			i;
	float		x;
	float		y;
	t_dda_dat	d;

	d.dx = p2.x_3d - p1.x_3d;
	d.dy = p2.y_3d - p1.y_3d;
	d.dx_abs = abs(d.dx);
	d.dy_abs = abs(d.dy);
	if (d.dx_abs > d.dy_abs)
		d.steps = d.dx_abs;
	else
		d.steps = d.dy_abs;
	d.x_inc = d.dx / (float)d.steps;
	d.y_inc = d.dy / (float)d.steps;
	x = p1.x_3d;
	y = p1.y_3d;
	i = -1;
	while (++i <= d.steps)
	{
		my_mlx_pixel_put(m->img, x, y, (p1.color * m->color_change) % HEX_MAX);
		x += d.x_inc;
		y += d.y_inc;
	}
}

/**
 * @brief transforms the 3d coordinates (x,y,z) to 2d coordinates (x,y)
 * 		  rotation matrices used are for right-handed coordinate system
 * @param p 3d point being transformed
 * @param m mlx object
 */
void	transform_3d(t_point *p, t_mlx *m)
{
	int	x;
	int	y;
	int	z;

	x = (p->x - (m->x_max / 2)) * m->scale;
	y = (p->y - (m->y_max / 2)) * m->scale;
	z = (p->z - (m->z_max / 2)) * m->scale;
	p->x_3d = (x * m->cosb * m->cosc) + \
				(y * m->sinb) + \
				(-z * -m->sinc * m->cosb);
	p->y_3d = (x * ((-m->sinb * m->cosa * m->cosc) + \
				(m->sina * m->sinc))) + \
				(y * m->cosa * m->cosb) + \
				(-z * ((m->sinb * m->cosa * m->sinc) + \
				(m->sina * m->cosc)));
	p->z_3d = (x * m->sina * m->sinb * m->cosc) + \
				(y * -m->sina * m->cosb) + \
				(-z * ((-m->sina * m->sinb * m->sinc) + \
				(m->cosa * m->cosc)));
	// p->x_3d += (m->x_max / 2) * m->scale;
	// p->y_3d += (m->y_max / 2) * m->scale;
	// p->z_3d += (m->z_max / 2) * m->scale;
	p->x_3d += m->x_offset;
	p->y_3d += m->y_offset;
}

void	transform_2d(t_point *p, t_mlx *m)
{
	p->x_3d = p->x * m->scale;
	p->y_3d = p->y * m->scale;
	p->x_3d += (m->x_offset - WIN_WIDTH / 4);
	p->y_3d += (m->y_offset - WIN_HEIGHT / 4);
}

void	draw_lines(t_list *node, t_mlx *mlx, void (*t)(t_point *, t_mlx *))
{
	int		i;
	t_point	*p1;
	t_point	*p2;

	p1 = (t_point *)node->content;
	p2 = (t_point *)node->next->content;
	i = -1;
	while (p1[++i + 1].color != -1)
	{
		t(&p2[i], mlx);
		dda(mlx, p1[i], p1[i + 1]);
		dda(mlx, p1[i], p2[i]);
	}
	t(&p2[i], mlx);
	dda(mlx, p1[i], p2[i]);
}

void	draw_image(t_mlx *mlx, void (*transform)(t_point *, t_mlx *))
{
	t_list	*node;
	t_point	*p1;
	int		i;

	node = mlx->lst;
	i = -1;
	p1 = (t_point *)node->content;
	while (p1[++i].color != -1)
		transform(&p1[i], mlx);
	while (node->next)
	{
		draw_lines(node, mlx, transform);
		node = node->next;
	}
	i = -1;
	p1 = (t_point *)node->content;
	while (p1[++i + 1].color != -1)
		dda(mlx, p1[i], p1[i + 1]);
}
