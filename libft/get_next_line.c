/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:14:28 by imeulema          #+#    #+#             */
/*   Updated: 2024/11/29 11:14:29 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*concat(char *line, char *buffer)
{
	char	*new;
	int		i;

	new = (char *) malloc((gnl_strlen(line) + BUFFER_SIZE + 1) * sizeof(char));
	if (!new)
	{
		free(line);
		return (NULL);
	}
	i = 0;
	while (line && line[i])
	{
		new[i] = line[i];
		i++;
	}
	while (*buffer && *buffer != '\n')
		new[i++] = *(buffer++);
	if (*buffer == '\n')
		new[i++] = '\n';
	new[i] = 0;
	free(line);
	return (new);
}

char	*trim(char **buffer)
{
	char	*copy;
	int		i;
	int		j;

	copy = (char *) malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!copy)
		return (free_null(buffer));
	i = 0;
	j = 0;
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	if ((*buffer)[i] == '\n')
		i++;
	while ((*buffer)[i])
		copy[j++] = (*buffer)[i++];
	if (j == 0)
		free_null(&copy);
	else
		copy[j] = 0;
	free_null(buffer);
	return (copy);
}

char	*make_line(char **line, int fd, char **buffer)
{
	int	read_bytes;

	while (!line_complete(*line))
	{
		read_bytes = read(fd, *buffer, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(*line);
			return (free_null(buffer));
		}
		else if (read_bytes == 0)
		{
			free_null(buffer);
			break ;
		}
		else
			(*buffer)[read_bytes] = 0;
		*line = concat(*line, (*buffer));
		if (!*line)
			return (free_null(buffer));
	}
	return (*line);
}

char	*get_next_line(int fd)
{
	static char	*buffer[4095];
	char		*line;

	line = NULL;
	if (fd < 0 || fd > 4095 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer[fd])
	{
		buffer[fd] = (char *) malloc((BUFFER_SIZE + 1) * sizeof(char));
		if (!buffer[fd])
			return (NULL);
	}
	else
	{
		line = concat(line, buffer[fd]);
		if (!line)
			return (free_null(&buffer[fd]));
	}
	line = make_line(&line, fd, &buffer[fd]);
	if (buffer[fd])
		buffer[fd] = trim(&buffer[fd]);
	return (line);
}
/*
#include <stdio.h>
#include <fcntl.h>

int main()
{
	int		fd1;
	int		fd2;
	char	*line;

	fd1 = open("file.txt", O_RDONLY);
	fd2 = open("file2.txt", O_RDONLY);
	line = get_next_line(fd1);
	printf("%s", line);
	line = get_next_line(fd2);
	printf("%s", line);
	line = get_next_line(fd1);
	printf("%s", line);
	line = get_next_line(fd2);
	printf("%s", line);
	line = get_next_line(fd1);
	printf("%s", line);
	line = get_next_line(fd2);
	printf("%s", line);
	line = get_next_line(fd1);
	printf("%s", line);
	line = get_next_line(fd2);
	printf("%s", line);
	free(line);
	close(fd1);
	return 0;
}
*/
