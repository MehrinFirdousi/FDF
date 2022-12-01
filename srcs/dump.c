#include "fdf.h"

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
			transform(&p[i], sq2, sq6, scale);
		node = node->next;
	}
	
}

void	put_points(t_list *lst, t_data *img)
{
	t_list	*node;
	t_point *p;
	int i;
	
	node = lst;
	while (node)
	{
		p = (t_point *)node->content;
		i = -1;
		while(p[++i + 1].color != -1)
		{
			// printf("%d,", p[i].x_3d);
			// printf("%d\t", p[i].y_3d);
			// my_mlx_pixel_put(img, p[i].x_3d, p[i].y_3d, p[i].color);
			dda(img, p[i], p[i + 1]);
		}
		// printf("\n");
		node = node->next;
	}	
}

// weird shit

// void	draw_circle(t_data *img)
// {
// 	float x;
// 	float y;
	
// 	for (float i = 0; i <= 15; i+=0.25)
// 	{
// 		x = 200 * cos(i);
// 		y = 200 * sin(i);
// 		printf("%f, %f\n", round(500+x), round(300+y));
// 		my_mlx_pixel_put(img, round(500+x), round(300+y), 0x00FF0000);
// 		dda2(img, 501, 314, round(500+x), round(300+y));
// 	}
// }

// void	dda2(t_data *img, double x1, double x2, double y1, double y2)
// {
// 	float	steps;
// 	float	dy;
// 	float	dx;
// 	float	xi;
// 	float	yi;
// 	int		i;
	
// 	dx = x2 - x1;
// 	dy = y2 - y1;
// 	steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
// 	xi = dx / steps;
// 	yi = dy / steps;
// 	i = -1;
// 	while (++i <= steps)
// 	{
// 		x1 += xi;
// 		y1 += yi;
// 		my_mlx_pixel_put(img, x1, y1,  0x00FFC000-i);
// 	}
// }

// void	dda1(t_data *img, int x1, int y1, int x2, int y2)
// {
// 	double	m;
// 	double	dx1;
// 	double	dy1;
// 	double	dy2;
	
// 	dx1 = x1;
// 	dy1 = y1;
// 	dy2 = y2;
// 	m = (dy2 - dy1) / (x2 - x1);
// 	printf("slope = %f\n", m);
// 	if (fabs(m) <= 1)
// 		while (x1 < x2)
// 		{
// 			dy1 += m;
// 			x1++;
// 			my_mlx_pixel_put(img, x1, dy1, 0x00FFA000);
// 		}
// 	else
// 		while (y1 < y2)
// 		{
// 			dx1 += (1/m);
// 			y1++;
// 			// printf("%d, %lf\n", x1, d);
// 			my_mlx_pixel_put(img, dx1, y1, 0x00FF0000);
// 		}
// }