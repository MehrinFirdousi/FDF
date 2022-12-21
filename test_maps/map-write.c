#include <stdio.h>
#include <fcntl.h>
#include "libft.h"

int main()
{
	int fd = open("amogus_hd.fdf", O_RDONLY);
	char *line;
	char **res;
	int count_words;

	int i = 0;
	while ((line = get_next_line(fd)))
	{
		res = ft_split2(line, " \n", &count_words);
		printf("%d: %d, ", ++i, count_words);
		ft_free_strs(res);
		free(line);
	}
	close(fd);
}