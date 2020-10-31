/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flavon <flavon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 13:34:11 by flavon            #+#    #+#             */
/*   Updated: 2020/10/31 13:42:38 by flavon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"
#include "stdlib.h"

char    *ft_func(char *dst, char c, int *size)
{
    int i = -1;
    char *tmp;
    if(!(tmp = malloc(sizeof(char) * (*size + 1))))
        return (NULL);
    while (++i < *size && dst[i])
        tmp[i] = dst[i];
    free(dst);
    dst = NULL;
    tmp[*size] = c;
    *size += 1;
    return (tmp);
}

int     get_next_line(char **line)
{
    char	buff[1];
    int		bytes;
	char	*tmp = NULL;
	int		size = 0;

	while ((bytes = read(0, buff, 1)) > 0)
	{
		if (buff[0] == '\n')
		{
			if (!(ft_func(tmp, '\0', &size)))
				return (-1);
			*line = tmp;
			return (1);
		}
		if (!(ft_func(tmp, buff[0], &size)))
			return (-1);
	}
	if (bytes == -1)
	{
		if (tmp)
			free(tmp);
		return (-1);
	}
	else if (bytes == 0)
	{
		if (!tmp)
		{
			line = malloc(1);
			*line[0] = 0;
			return (0);
		}
		else 
		{
			if (!(ft_func(tmp, '\0', &size)))
				return (-1);
			*line = tmp;
			return (0);
		}
	}
	return (-1);
}