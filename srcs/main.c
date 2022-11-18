/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:24:28 by mfirdous          #+#    #+#             */
/*   Updated: 2022/11/18 21:36:07 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	dda(t_data *img, int x1, int y1, int x2, int y2)
{
	double	m;
	double	dx1;
	double	dy1;
	double	dx2;
	double	dy2;
	
	dx1 = x1;
	dy1 = y1;
	dx2 = x2;
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

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	
	void	*mlx;
	void	*mlx_win;
	t_data	img;

	
	// initializing a window and making it remain open 
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "FDF");

	img.img = mlx_new_image(mlx, 1920, 1080);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, \
								&img.line_length, &img.endian);
	dda(&img, 100, 100, 400, 500);
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
	
}
