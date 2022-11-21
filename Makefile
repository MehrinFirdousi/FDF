# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/17 15:25:59 by mfirdous          #+#    #+#              #
#    Updated: 2022/11/21 22:01:49 by mfirdous         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	fdf

SRCS	=	srcs/main.c srcs/parsing.c

OBJS	=	${SRCS:.c=.o}

HEADER	=	include

LIBFT	=	libft/libft.a

MLX		=	libmlx.dylib

CC		=	gcc

MLX_DIR		=	mlx

CFLAGS	=	-Wall -Wextra -Werror -I ${HEADER} -I ${MLX_DIR}

MLX_FLAGS	= -L. -lmlx -framework OpenGL -framework AppKit

.c.o:
			${CC} ${CFLAGS} -c $< -o ${<:.c=.o} 

${NAME}:	${LIBFT} ${MLX} ${OBJS}
			${CC} ${CFLAGS} ${MLX_FLAGS} ${OBJS} ${LIBFT} -o ${NAME}

${LIBFT}:	
			${MAKE} -C libft
			${MAKE} -C libft bonus

${MLX}:		
			${MAKE} -C mlx
			mv mlx/libmlx.dylib .

all:		${NAME}

clean:		
			rm -f ${OBJS}
			rm -f ${MLX}
			${MAKE} -C libft clean
			${MAKE} -C mlx clean

fclean:		clean
			rm -f ${NAME} 
			${MAKE} -C libft fclean

re:			fclean all

.PHONY:		all clean fclean re bonus