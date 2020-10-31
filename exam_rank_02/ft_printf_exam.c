/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_exam.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flavon <flavon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 13:43:55 by flavon            #+#    #+#             */
/*   Updated: 2020/10/31 15:42:45 by flavon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int g_width;
int g_result;
int g_presicion;

void	ft_putchar(char ch)
{
	write(1, &ch, 1);
	g_result++;
}

int		ft_isdigit(char ch)
{
	return (ch >= '0' && ch <= '9');
}

int		ft_width(char *str, int i)
{
	int len = 0;
	while (str[i] && ft_isdigit(str[i]))
	{
		len++;
		g_width = (g_width * 10) + str[i++] - '0';
	}
	return (len);
}

int		ft_presicion(char *str, int i)
{
	int len = 1;
	g_presicion = 0;
	i++;
	while (str[i] && ft_isdigit(str[i]))
	{
		len++;
		g_presicion = (g_presicion * 10) + str[i++] - '0';
	}
	return (len);
}

int		ft_strlen(char *dst)
{
	int i = 0;
	while (dst[i])
		i++;
	return(i);
}

int		ft_strlen_int(long long n, int base)
{
	int i = 0;
	while ((n /= base) != 0)
		i++;
	return (++i);
}

char	*ft_itoa(long long nbr, int base)
{
	char *res;
	int len = ft_strlen_int(nbr, base);
	if (!(res = malloc(sizeof(char) * len + 1)))
		return (0);
	res[len] = '\0';
	while (len != 0)
	{
		res[len - 1] = "0123456789abcdef"[nbr % base];
		nbr /= base;
		len--;
	}
	return (res);
}
void	ft_int(int n)
{
	int i = 0;
	long long nbr = n;
	if (n == 0 && g_presicion >= 0)
		while (g_width-- > 0)
		{
			ft_putchar(' ');
			return ;
		}
	if (g_presicion >= 0 && n < 0) {g_width--; nbr *= -1;}
	char *str = ft_itoa(nbr, 10);
	int len = ft_strlen(str);
	if (g_presicion >= 0 && g_presicion > len)
		while (g_width-- - g_presicion > 0)
			ft_putchar(' ');
	else
		while (g_width-- - len > 0)
			ft_putchar(' ');
	if (n < 0)
		ft_putchar('-');
	if (g_presicion >= 0 && g_presicion > len)
		while (g_presicion-- - len > 0)
			ft_putchar('0');
	while (len--)
		ft_putchar(str[i++]);
}

void	ft_hex(unsigned int n)
{
	int i = 0;
	long long nbr = n;
	if (n == 0 && g_presicion >= 0)
		while (g_width-- > 0)
		{
			ft_putchar(' ');
			return ;
		}
	char *str = ft_itoa(nbr, 16);
	int len = ft_strlen(str);
	if (g_presicion >= 0 && g_presicion > len)
		while (g_width-- - g_presicion > 0)
			ft_putchar(' ');
	else
		while (g_width-- - len > 0)
			ft_putchar(' ');
	if (g_presicion >= 0 && g_presicion > len)
		while (g_presicion-- - len > 0)
			ft_putchar('0');
	while (len--)
		ft_putchar(str[i++]);
}

void	ft_str(char *dst)
{
	int i = 0;
	int length = ft_strlen(dst);
	if (g_presicion >= 0 && g_presicion < length)
		length = g_presicion;
	if (g_presicion >= 0)
		while (g_width-- - g_presicion > 0)
			ft_putchar(' ');
	else
		while (g_width-- - length > 0)
			ft_putchar(' ');
	while (length--)
		ft_putchar(dst[i++]);
}

int ft_printf(char const *format, ...)
{
	int i = 0;
	va_list arg;
	g_result = 0;
	if (!format)
		return (0);
	va_start(arg, format);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			g_presicion = -1;
			g_width = 0;
			i++;
			if (ft_isdigit(format[i]))
				i += ft_width((char *)format, i);
			if (format[i] == '.')
				i += ft_presicion((char *)format, i);
			if (format[i] == 'd')
				ft_int(va_arg(arg, int));
			else if (format[i] == 'x')
				ft_hex(va_arg(arg, unsigned int));
			else if (format[i] == 's')
				ft_str(va_arg(arg, char *));
			else if (format[i] == '%')
				ft_str("%");
			i++;
		}
		if (format[i] != '%')
			ft_putchar(format[i++]);
	}
	va_end(arg);
	return (g_result);
}

int main(void)
{
	ft_printf("Hello, %123d\n", -0);
	printf("Hello, %123d\n", -0);
	return(0);
}