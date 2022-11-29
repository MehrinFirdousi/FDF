/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:24:28 by mfirdous          #+#    #+#             */
/*   Updated: 2022/11/29 02:08:03 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, unsigned int color)
{
	char	*dst;

	if (x < WIN_WIDTH && y < WIN_HEIGHT)
	{
		dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
		*(unsigned int*)dst = color;
	}
}

void dda(t_data *img, t_point p1, t_point p2)
{
    int dx;
    int dy;
 
	dx = p2.x - p1.x;
	dy = p2.y - p1.y;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
 
    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;
 
    float X = p1.x;
    float Y = p1.y;
    for (int i = 0; i <= steps; i++) 
	{
        my_mlx_pixel_put(img, X, Y, p1.color);
        X += Xinc; // increment in x at each step
        Y += Yinc; // increment in y at each step
    }
}

// void	draw_circle(t_data *img)
// {
// 	float x;
// 	float y;
	
// 	for (float i = 0; i <= 15; i+=0.25)
// 	{
// 		x = 200 * cos(i);
// 		y = 200 * sin(i);
// 		my_mlx_pixel_put(img, round(500+x), round(300+y), 0x00FF0000);
// 		dda(img, 501, 314, round(500+x), round(300+y), WHITE);
// 	}
// }

void	display_row(void *point)
{
	t_point *p;
	int i;
	
	p = (t_point *)point;
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

void	put_coordinates(t_data *img, t_list *lst, int scale)
{
	t_list	*node;
	t_point *p1;
	t_point	*p2;
	int i;
	
	node = lst;
	i = -1;
	p1 = (t_point *)node->content;
	while(p1[++i + 1].color != -1)
	{
		p1[i].x *= scale;
		p1[i].y *= scale;
	}
	while (node->next)
	{	
		p1 = (t_point *)node->content;
		p2 = (t_point *)node->next->content;
		i = -1;
		while(p1[++i + 1].color != -1)
		{
			p2[i].x *= scale;
			p2[i].y *= scale;
		}
		dda(img, p1[i], p2[i]);
		node = node->next;
	}
	i = -1;
	p1 = (t_point *)node->content;
	while(p1[++i + 1].color != -1)
		dda(img, p1[i], p1[i + 1]);
}
// void	put_coordinates(t_list *lst, t_data *img)
// {
// 	t_list	*node;
// 	t_point *p1;
// 	int i;
	
// 	node = lst;
// 	while (node)
// 	{	
// 		p1 = (t_point *)node->content;
// 		i = -1;
// 		while(p1[++i].color != -1)
// 		{
// 			my_mlx_pixel_put(img, p1[i].x, p1[i].y, p1[i].color);
// 		}	
// 		node = node->next;
// 	}
// }

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	
	if (argc != 2)
	{
		ft_printf("Usage : %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	// void	*mlx;
	// void	*mlx_win;
	// t_data	img;

	// // initializing a window and making it remain open 
	// mlx = mlx_init();
	// mlx_win = mlx_new_window(mlx, WIN_WIDTH, WIN_HEIGHT, "FDF");

	// img.img = mlx_new_image(mlx, WIN_WIDTH, WIN_HEIGHT);
	// img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	
	t_list *res;
	int scale;
	
	scale = get_coordinates(argv[1], &res);
	printf("scale = %d\n", scale);
	ft_lstiter(res, &display_row);
	// put_coordinates(&img, res, scale);
	printf("\n");
	ft_lstiter(res, &display_row);
	ft_lstclear(&res, &free);
	
	// mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	// mlx_loop(mlx);
	// my_mlx_pixel_put(&img, 501, 314, 0x00FFA000);	
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