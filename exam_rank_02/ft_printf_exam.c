#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <stdio.h>

int g_width;
int g_dot;
int g_result;

void	ft_putchar(char ch)
{
	write(1, &ch, 1);
	g_result++;
}

int		ft_isdigit(char ch) {return (ch >= '0' && ch <= '9');}

int		ft_strlen(char *dst)
{
	int i = 0;
	while(dst[i])
		i++;
	return (i);
}

int		ft_strlen_int(unsigned int n, int base)
{
	int len = 1;
	while ((n /= base) != 0)
		len++;
	return (len);
}

char	*ft_itoa(unsigned int n, int base)
{
	int len = ft_strlen_int(n, base);
	char *res;
	if (!(res = (char *)malloc(len + 1)))
		return (NULL);
	res[len] = '\0';
	while (len != 0)
	{
		res[len - 1] = "0123456789abcdef"[n % base];
		n /= base;
		len--;
	}
	return (res);
}

void	ft_str(char *dst)
{
	if (dst == NULL)
		dst = "(null)";
	int len = ft_strlen(dst);
	int i = 0;
	if (g_dot >= 0 && g_dot < len)
		len = g_dot;
	if (g_dot >= 0)
		while (g_width-- - len > 0)
			ft_putchar(' ');
	else
		while (g_width-- - len > 0)
			ft_putchar(' ');
	while(len--)
		ft_putchar(dst[i++]);
	
}

void	ft_int(int n)
{
	long long nbr = n;
	int i = 0;
	if (g_dot == 0 && n == 0)
	{
		while (g_width--)
			ft_putchar(' ');
		return ;
	}
	if (n < 0) {g_width--; nbr *= -1;}
	char *str = ft_itoa(nbr, 10);
	int len = ft_strlen(str);
	if (g_dot >= 0 && g_dot > len)
		while (g_width-- - g_dot > 0)
			ft_putchar(' ');
	else
		while (g_width-- - len > 0)
			ft_putchar(' ');
	if (n < 0) ft_putchar('-');
	if (g_dot >= 0)
		while (g_dot-- - len > 0)
			ft_putchar('0');
	while(len--)
		ft_putchar(str[i++]);
	free(str);
}

void	ft_hex(unsigned int n)
{
	int i = 0;
	if (g_dot == 0 && n == 0)
	{
		while (g_width-- > 0)
			ft_putchar(' ');
		return ;
	}
	char *str = ft_itoa(n, 16);
	int len = ft_strlen(str);
	if (g_dot >= 0 && g_dot > len)
		while (g_width-- - g_dot > 0)
			ft_putchar(' ');
	else
		while (g_width-- - len > 0)
			ft_putchar(' ');
	if (g_dot >= 0)
		while (g_dot-- - len > 0)
			ft_putchar('0');
	while(len--)
		ft_putchar(str[i++]);
	free(str);
}

int	ft_printf(char const *str, ...)
{
	va_list arg;
	int i = 0;
	g_result = 0;
	if (!str)
		return (0);
	va_start(arg, str);
	while(str[i])
	{
		if (str[i] == '%')
		{
			i++;
			g_dot = -1;
			g_width = 0;
			if (ft_isdigit(str[i]))
				while (ft_isdigit(str[i]) && str[i])
					g_width = g_width * 10 + str[i++] - '0';
			if (str[i] == '.')
			{
				i++;
				g_dot = 0;
				if (ft_isdigit(str[i]))
					while (ft_isdigit(str[i]) && str[i])
						g_dot = g_dot * 10 + str[i++] - '0';
			}
			if (str[i] == 's')
				ft_str(va_arg(arg, char *));
			else if (str[i] == 'd')
				ft_int(va_arg(arg, int));
			else if (str[i] == 'x')
				ft_hex(va_arg(arg, unsigned int));
			else if (str[i] == '%')
				ft_str("%");
			i++;
		}
		else
			ft_putchar(str[i++]);
	}
	va_end(arg);
	return (g_result);
}

int	main(void)
{
	int a;
	int b;

	a = ft_printf("Hello, %s\n", "World!");
	b = printf("Hello, %s\n", "World!");
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15s\n", "World!");
	b = printf("Hello, %15s\n", "World!");
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15.5s\n", "World!");
	b = printf("Hello, %15.5s\n", "World!");
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.10s\n", "World!");
	b = printf("Hello, %5.10s\n", "World!");
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.10s\n", "World!");
	b = printf("Hello, %.10s\n", "World!");
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.3s\n", "World!");
	b = printf("Hello, %.3s\n", "World!");
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.s\n", "World!");
	b = printf("Hello, %.s\n", "World!");
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %s\n", NULL);
	b = printf("Hello, %s\n", NULL);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15s\n", NULL);
	b = printf("Hello, %15s\n", NULL);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15.10s\n", NULL);
	b = printf("Hello, %15.10s\n", NULL);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.10s\n", NULL);
	b = printf("Hello, %5.10s\n", NULL);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.10s\n", NULL);
	b = printf("Hello, %.10s\n", NULL);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.s\n", NULL);
	b = printf("Hello, %.s\n", NULL);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.2s\n", NULL);
	b = printf("Hello, %.2s\n", NULL);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %d\n", 0);
	b = printf("Hello, %d\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15d\n", 0);
	b = printf("Hello, %15d\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15.5d\n", 0);
	b = printf("Hello, %15.5d\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.10d\n", 0);
	b = printf("Hello, %5.10d\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.1d\n", 0);
	b = printf("Hello, %5.1d\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.10d\n", 0);
	b = printf("Hello, %.10d\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.1d\n", 0);
	b = printf("Hello, %.1d\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.d\n", 0);
	b = printf("Hello, %.d\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %d\n", INT_MAX);
	b = printf("Hello, %d\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15d\n", INT_MAX);
	b = printf("Hello, %15d\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15.10d\n", INT_MAX);
	b = printf("Hello, %15.10d\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.15d\n", INT_MAX);
	b = printf("Hello, %5.15d\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.d\n", INT_MAX);
	b = printf("Hello, %5.d\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.2d\n", INT_MAX);
	b = printf("Hello, %5.2d\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.20d\n", INT_MAX);
	b = printf("Hello, %.20d\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);
	
	a = ft_printf("Hello, %.2d\n", INT_MAX);
	b = printf("Hello, %.2d\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.d\n", INT_MAX);
	b = printf("Hello, %d\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %d\n", INT_MIN);
	b = printf("Hello, %d\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15d\n", INT_MIN);
	b = printf("Hello, %15d\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15.10d\n", INT_MIN);
	b = printf("Hello, %15.10d\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.15d\n", INT_MIN);
	b = printf("Hello, %5.15d\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.d\n", INT_MIN);
	b = printf("Hello, %5.d\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.2d\n", INT_MIN);
	b = printf("Hello, %5.2d\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.20d\n", INT_MIN);
	b = printf("Hello, %.20d\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);
	
	a = ft_printf("Hello, %.2d\n", INT_MIN);
	b = printf("Hello, %.2d\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.d\n", INT_MIN);
	b = printf("Hello, %.d\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %d\n", INT_MIN);
	b = printf("Hello, %d\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %d\n", INT_MIN);
	b = printf("Hello, %d\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15d\n", INT_MAX + 1);
	b = printf("Hello, %15d\n", INT_MAX + 1);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.5d\n", INT_MIN - 1);
	b = printf("Hello, %.5d\n", INT_MIN - 1);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %10d\n", INT_MIN - 1);
	b = printf("Hello, %10d\n", INT_MIN - 1);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15.5d\n", INT_MIN - 1);
	b = printf("Hello, %15.5d\n", INT_MIN - 1);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %x\n", 0);
	b = printf("Hello, %x\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15x\n", 0);
	b = printf("Hello, %15x\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15.5x\n", 0);
	b = printf("Hello, %15.5x\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.10x\n", 0);
	b = printf("Hello, %5.10x\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.1x\n", 0);
	b = printf("Hello, %5.1x\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.10x\n", 0);
	b = printf("Hello, %.10x\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.1x\n", 0);
	b = printf("Hello, %.1x\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.x\n", 0);
	b = printf("Hello, %.x\n", 0);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %x\n", INT_MAX);
	b = printf("Hello, %x\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15x\n", INT_MAX);
	b = printf("Hello, %15x\n", INT_MAX);
	printf("a = %x\n", a);
	printf("b = %x\n", a);

	a = ft_printf("Hello, %15.10x\n", INT_MAX);
	b = printf("Hello, %15.10x\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.15x\n", INT_MAX);
	b = printf("Hello, %5.15x\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.x\n", INT_MAX);
	b = printf("Hello, %5.x\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.2x\n", INT_MAX);
	b = printf("Hello, %5.2x\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.20x\n", INT_MAX);
	b = printf("Hello, %.20x\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);
	
	a = ft_printf("Hello, %.2x\n", INT_MAX);
	b = printf("Hello, %.2x\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.x\n", INT_MAX);
	b = printf("Hello, %x\n", INT_MAX);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %x\n", INT_MIN);
	b = printf("Hello, %x\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15x\n", INT_MIN);
	b = printf("Hello, %15x\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15.10x\n", INT_MIN);
	b = printf("Hello, %15.10x\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.15x\n", INT_MIN);
	b = printf("Hello, %5.15x\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.x\n", INT_MIN);
	b = printf("Hello, %5.x\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %5.2x\n", INT_MIN);
	b = printf("Hello, %5.2x\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.20x\n", INT_MIN);
	b = printf("Hello, %.20x\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);
	
	a = ft_printf("Hello, %.2x\n", INT_MIN);
	b = printf("Hello, %.2x\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.x\n", INT_MIN);
	b = printf("Hello, %.x\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %x\n", INT_MIN);
	b = printf("Hello, %x\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %x\n", INT_MIN);
	b = printf("Hello, %x\n", INT_MIN);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15x\n", INT_MAX + 1);
	b = printf("Hello, %15x\n", INT_MAX + 1);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %.5x\n", INT_MIN - 1);
	b = printf("Hello, %.5x\n", INT_MIN - 1);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %10x\n", INT_MIN - 1);
	b = printf("Hello, %10x\n", INT_MIN - 1);
	printf("a = %d\n", a);
	printf("b = %d\n", a);

	a = ft_printf("Hello, %15.5x\n", INT_MIN - 1);
	b = printf("Hello, %15.5x\n", INT_MIN - 1);
	printf("a = %d\n", a);
	printf("b = %d\n", a);
}