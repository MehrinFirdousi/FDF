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
			transform_3d(&p[i], sq2, sq6, scale);
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

// only for testing, remove
void	transform_all(t_mlx *mlx)
{
	t_list	*node;
	t_point *p;
	int i;

	node = mlx->lst;
	while (node)
	{
		p = (t_point *)node->content;
		i = -1;
		while(p[++i].color != -1)
			transform_3d(&p[i], mlx->a, mlx->b, mlx->scale, mlx->x_offset, mlx->y_offset);
		node = node->next;
	}
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



// ------------------------------------- alternate rotation matrices -------------------------------------
// point->x_3d = (point->x_3d * cosb) - (z_scaled * sinb); // x axis 
// point->y_3d = (point->x_3d * sina * sinb) + (point->y_3d * cosa) + (z_scaled * sina * cosb); // y axis 

// point->x_3d = (point->x_3d * cosb) + (z_scaled * sinb); // x axis - counterclockwise
// point->y_3d = (point->x_3d * sina * sinb) + (point->y_3d * cosa) - (z_scaled * sina * cosb); // y axis 

// point->x_3d = (point->x_3d * cosb) - (point->y_3d * sinb); // along x axis - counterclockwise
// point->y_3d = (point->x_3d * sinb * cosa) + (point->y_3d * cosa * cosb) - (z_scaled * sina); // along z axis


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

// void	dda2(t_data *img, t_point p1, t_point p2)
// {
// 	float	steps;
// 	float	dy;
// 	float	dx;
// 	float	xi;
// 	float	yi;
// 	int		i;
	
// 	dx = p2.x_3d - p1.x_3d;
// 	dy = p2.y_3d - p1.y_3d;
// 	steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
// 	xi = dx / steps;
// 	yi = dy / steps;
// 	i = -1;
// 	while (++i <= steps)
// 	{
// 		p1.x_3d += xi;
// 		p1.y_3d += yi;
// 		my_mlx_pixel_put(img, p1.x_3d, p1.y_3d,  p1.color);
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