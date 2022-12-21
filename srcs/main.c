/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:24:28 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/21 19:37:17 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, unsigned int color)
{
	char	*dst;

	if (x >= 0 && y >= 0 && x < WIN_WIDTH && y < WIN_HEIGHT)
	{
		dst = data->addr + \
				(y * data->line_length + x * (data->bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

void	mlx_set_up(t_mlx *mlx, t_data *img)
{
	mlx->mlx = mlx_init();
	mlx->win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, "FDF");
	img->img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, \
								&img->line_length, &img->endian);
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
	mlx->dir = 1;
	mlx->color_change = 1;
	mlx->cur_projection = 3;
	mlx->dvd_translate = 0;
}

int	exit_free(t_mlx *m)
{
	mlx_destroy_image(m->mlx, m->img->img);
	mlx_destroy_window(m->mlx, m->win);
	ft_lstclear(&m->lst, &free);
	exit(0);
}

int	open_file(char *file_name)
{
	int	fd;

	if (ft_strncmp(ft_strchr(file_name, '.'), ".fdf", 5) != 0)
	{
		ft_printf("File has to be in .fdf format\n");
		exit(EXIT_FAILURE);
	}
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("File does not exist\n");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

int	main(int argc, char **argv)
{
	t_mlx	mlx;
	t_data	img;
	int		fd;

	if (argc != 2)
	{
		ft_printf("Usage : %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	fd = open_file(argv[1]);
	get_3d_coordinates(fd, &mlx);
	mlx_set_up(&mlx, &img);
	draw_image(&mlx, transform_3d);
	mlx_key_hook(mlx.win, key_click_handler, &mlx);
	mlx_hook(mlx.win, 2, 1L << 0, key_hold_handler, &mlx);
	mlx_hook(mlx.win, 17, 0, exit_free, &mlx);
	mlx_loop_hook(mlx.mlx, dvd_translate, &mlx);
	mlx_put_image_to_window(mlx.mlx, mlx.win, img.img, 0, 0);
	mlx_loop(mlx.mlx);
	return (0);
}
