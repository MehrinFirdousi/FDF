#include <stdio.h>
#include <fcntl.h>
#include "libft.h"

int main()
{
	int fd = open("dvd-small.fdf", O_RDONLY);
	char *line;
	char *new_line;
	char *new_space = "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
	t_list *lst;
	t_list *node;
	char *nl;

	int i = 0;
	while ((line = get_next_line(fd)))
	{
		// if (i == 0)
		// {
		// 	printf("%s", line);
		// 	printf("%zu\n", ft_strlen(line));
		// }
		// i++;
		nl = ft_strchr(line, '\n');
		*nl = ' ';
		// line[ft_strlen(line) - 1] = '0';
		new_line = ft_strjoin(line, new_space);
		ft_lstadd_back(&lst, ft_lstnew(new_line));
		free(line);
	}
	close(fd);
	fd = open("dvd-small.fdf", O_WRONLY);
	node = lst;
	while (node)
	{
		write(fd, node->content, ft_strlen((char *)(node->content)));
		node = node->next;
	}
}