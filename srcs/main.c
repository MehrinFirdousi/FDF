/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:24:28 by mfirdous          #+#    #+#             */
/*   Updated: 2022/11/28 00:22:33 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	dda1(t_data *img, int x1, int y1, int x2, int y2)
{
	double	m;
	double	dx1;
	double	dy1;
	double	dy2;
	
	dx1 = x1;
	dy1 = y1;
	dy2 = y2;
	m = (dy2 - dy1) / (x2 - x1);
	printf("slope = %f\n", m);
	if (fabs(m) <= 1)
		while (x1 < x2)
		{
			dy1 += m;
			x1++;
			my_mlx_pixel_put(img, x1, dy1, 0x00FFA000);
		}
	else
		while (y1 < y2)
		{
			dx1 += (1/m);
			y1++;
			// printf("%d, %lf\n", x1, d);
			my_mlx_pixel_put(img, dx1, y1, 0x00FF0000);
		}
}

void dda(t_data *img, int X0, int Y0, int X1, int Y1)
{
    // calculate dx & dy
    int dx = X1 - X0;
    int dy = Y1 - Y0;
 
    // calculate steps required for generating pixels
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
 
    // calculate increment in x & y for each steps
    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;
 
    // Put pixel for each step
    float X = X0;
    float Y = Y0;
    for (int i = 0; i <= steps; i++) 
	{
        my_mlx_pixel_put(img, X, Y, 0x00cba3ff-i);
        X += Xinc; // increment in x at each step
        Y += Yinc; // increment in y at each step
    }
}

void	dda2(t_data *img, double x1, double x2, double y1, double y2)
{
	float	steps;
	float	dy;
	float	dx;
	float	xi;
	float	yi;
	int		i;
	
	dx = x2 - x1;
	dy = y2 - y1;
	steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
	xi = dx / steps;
	yi = dy / steps;
	i = -1;
	while (++i <= steps)
	{
		x1 += xi;
		y1 += yi;
		my_mlx_pixel_put(img, x1, y1,  0x00FFC000-i);
	}
}

void	draw_circle(t_data *img)
{
	float x;
	float y;
	
	for (float i = 0; i <= 15; i+=0.25)
	{
		x = 200 * cos(i);
		y = 200 * sin(i);
		// printf("%f, %f\n", round(500+x), round(300+y));
		// my_mlx_pixel_put(img, round(500+x), round(300+y), 0x00FF0000);
		dda(img, 501, 314, round(500+x), round(300+y));
	}
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
	// void	*mlx;
	// void	*mlx_win;
	// t_data	img;

	// // initializing a window and making it remain open 
	// mlx = mlx_init();
	// mlx_win = mlx_new_window(mlx, 1280, 720, "FDF");

	// img.img = mlx_new_image(mlx, 1280, 720);
	// img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	// // dda(&img, 100, 400, 400, 100);
	// draw_circle(&img);
	// mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	// my_mlx_pixel_put(&img, 501, 314, 0x00FFA000);	
	// mlx_loop(mlx);
	
	// get_rows(argv[1]);
	t_list *res = get_rows(argv[1]);
	ft_lstclear(&res, &free);
	// printf("hextodec: %d\n", hex_to_dec("810202"));
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

