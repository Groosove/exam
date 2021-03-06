#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int		width, height;
char	*draw;
char	type, color, back;
float	x, y, w, h;

int		check_init(float i, float j)
{
	if (i < x || i > x + w || j < y || j > y + h)
		return (0);
	if (i - x < 1 || x + w - i < 1 || j - y < 1 || y + h - j < 1)
		return (2); // Border
	return (1); // Inside
}
void	drawing(void)
{
	int	i = -1, j, hit;

	while (++i < width)
	{
		j = -1;
		while (++j < height)
		{
			hit = check_init((float)i, (float)j);
			if (hit == 2 || (hit == 1 && type == 'R'))
				draw[i + j * width] = color;
		}
	}
}

int	parser(FILE *file)
{
	int i = -1, result;
	if (fscanf(file, "%d %d %c\n", &width, &height, &back) != 3)
		return (1);
	if (width < 1 || width > 300 || height < 1 || height > 300)
		return (1);
	if (!(draw = (char *)malloc(width * height)))
		return (1);
	while (++i < width * height)
		draw[i] = back;
	while ((result = fscanf(file, "%c %f %f %f %f %c\n", &type, &x, &y, &w, &h, &color)) == 6)
	{
		if (w <= 0 || h <= 0 || (type != 'r' && type != 'R'))
			return(1);
		drawing();
	}
	if (result != -1)
		return (1);
	return (0);
}

void output(void)
{
	int	i = -1, c = 10;
	while (++i < height) 
	{
		write(1, draw + i * width, width);
		write(1, &c, 1);
	}
}

int main(int argc, char *argv[])
{
	FILE *file;
	if (argc != 2) {write (1, "Error: argument\n", 16); return (1);}
	if (!(file = fopen(argv[1], "r")) || parser(file)) { write(1, "Error: Operation file corrupted\n", 32); free(draw); fclose(file); return (1); }
	output();
	fclose(file);
	free(draw);
	return (0);
}
