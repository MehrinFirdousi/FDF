# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/17 15:25:59 by mfirdous          #+#    #+#              #
#    Updated: 2022/11/29 15:05:14 by mfirdous         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	fdf

SRCS	=	srcs/main.c srcs/parsing.c srcs/transform.c

OBJS	=	${SRCS:.c=.o}

HEADER	=	include

LIBFT	=	libft/libft.a

MLX		=	mlx_linux/libmlx.a

CC		=	gcc

MLX_DIR		=	mlx_linux

CFLAGS	=	 -Wall -Wextra -Werror -I ${HEADER} -I/usr/include -I ${MLX_DIR} -O3 -Ofast

MLX_FLAGS	= -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux 

.c.o:
			${CC} ${CFLAGS} -c $< -o ${<:.c=.o} 

${NAME}:	${LIBFT} ${MLX} ${OBJS}
			${CC} ${CFLAGS} ${MLX_FLAGS} ${OBJS} ${LIBFT} ${MLX} -o ${NAME} -lm -lXext -lX11 -lz
# ${NAME}:	${LIBFT} ${OBJS}
# 			${CC} ${CFLAGS} ${OBJS} ${LIBFT} -o ${NAME} -lm 

${LIBFT}:	
			${MAKE} -C libft
			${MAKE} -C libft bonus

${MLX}:		
			${MAKE} -C mlx_linux
#			mv mlx_linux/libmlx.a .

all:		${NAME}

clean:		
			rm -f ${OBJS}
			${MAKE} -C libft clean
			${MAKE} -C mlx_linux clean

fclean:		clean
			rm -f ${NAME}
			${MAKE} -C libft fclean

re:			fclean all

.PHONY:		all clean fclean re bonus