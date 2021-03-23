#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

int g_id = 0;

typedef struct s_cli {
    int fd;
    int id;
    char*   write_buf;
    char*   read_buf;
    struct s_cli*   next;
}               t_cli;

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void    fatal_error() {
    write(2, "Fatal error\n", strlen("Fatal error\n"));
    exit(1);
}

int main(int ac, char** av) {
    if (ac != 2) {
        write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
        exit(1);
    }

    int sockfd, connfd, max_fd, ret, id_tmp;
	struct sockaddr_in servaddr, cli; 
    socklen_t   len = sizeof(cli);
    fd_set fd_read, fd_write, cp_fd_read, cp_fd_write;
    char*   buf = NULL;
    char*   buf_one = (char*)calloc(101000, 1);
    char*   msg = NULL;
    t_cli*  client = NULL;
    uint16_t    port = atoi(av[1]);

	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = 127 | (1 << 24); //127.0.0.1
	servaddr.sin_port = port >> 8 | port << 8;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        fatal_error();
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
        fatal_error();
	if (listen(sockfd, 10) != 0)
        fatal_error();

    max_fd = sockfd;

    FD_ZERO(&fd_read);
    FD_ZERO(&fd_write);

    FD_SET(max_fd, &fd_read);
    FD_SET(max_fd, &fd_write);

    while (1) {
        cp_fd_read = fd_read;
        cp_fd_write = fd_write;
        select(max_fd + 1, &cp_fd_read, &cp_fd_write, NULL, 0);
        if (FD_ISSET(sockfd, &cp_fd_read)) {
            connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
            if (connfd) {
                t_cli*  tmp = client;
                t_cli*  new;

                new = (t_cli*)calloc(sizeof(t_cli), 1);
                if (!new)
                    fatal_error();
                new->fd = connfd;
                new->id = g_id++;
                new->write_buf = NULL;
                new->read_buf = NULL;
                new->next = NULL;

                if (!client)
                    client = new;
                else {
                    while (tmp->next)
                        tmp = tmp->next;
                    tmp->next = new;
                }
                FD_SET(connfd, &fd_read);
                FD_SET(connfd, &fd_write);
                max_fd = (connfd > max_fd) ? connfd : max_fd;
                sprintf(buf_one, "server: client %d just arrived\n", g_id - 1);
                for (t_cli* it = client; it; it = it->next) {
                    if (it->fd != connfd)
                        it->write_buf = str_join(it->write_buf, buf_one);
                }
                bzero(buf_one, 100);
            }
        }
        for (t_cli* it = client; it; it = it->next) {
            if (FD_ISSET(it->fd, &cp_fd_read) && it->fd != sockfd) {
                buf = (char*)calloc(4097, 1);
                if (!buf)
                    fatal_error();
                ret = recv(it->fd, buf, 4096, 0);
                if (ret <= 0) {
                    close(it->fd);
                    sprintf(buf_one, "server: client %d just left\n", it->id);
                    for (t_cli* tmp = client; tmp; tmp = tmp->next) {
                        if (tmp->fd != it->fd)
                            tmp->write_buf = str_join(tmp->write_buf, buf_one);
                    }
                    if (it->write_buf)
                        free(it->write_buf);
                    if (it->read_buf)
                        free(it->read_buf);
                    t_cli*  tmp = client;
                    t_cli*  del;

                    if (tmp->fd == it->fd) {
                        client = tmp->next;
                        free(tmp);
                    }
                    else {
                        while (tmp->next && tmp->next->fd != it->fd)
                            tmp = tmp->next;
                        del = tmp->next;
                        tmp->next = tmp->next->next;
                        free(del);
                    }
                    FD_CLR(it->fd, &fd_write);
                    FD_CLR(it->fd, &fd_read);
                    bzero(buf_one, 100);
                }
                else {
                    it->read_buf = str_join(it->read_buf, buf);
                    while (extract_message(&(it->read_buf), &msg)) {
                        sprintf(buf_one, "client %d: %s", it->id, msg);
                        for (t_cli* tmp = client; tmp; tmp = tmp->next) {
                            if (tmp->fd != it->fd)
                                tmp->write_buf = str_join(tmp->write_buf, buf_one);
                        }
                        bzero(buf_one, 101000);
                        free(msg);
                        msg = NULL;
                    }
                }
                free(buf);
            }
        }
        for (t_cli* it = client; it; it = it->next) {
            if (FD_ISSET(it->fd, &cp_fd_write) && it->write_buf) {
                ret = send(it->fd, it->write_buf, strlen(it->write_buf), 0);
                if (ret == -1) {
                    close(it->fd);
                    sprintf(buf_one, "server: client %d just left\n", it->id);
                    for (t_cli* tmp = client; tmp; tmp = tmp->next) {
                        if (tmp->fd != it->fd)
                            tmp->write_buf = str_join(tmp->write_buf, buf_one);
                    }
                    if (it->write_buf)
                        free(it->write_buf);
                    if (it->read_buf)
                        free(it->read_buf);
                    t_cli*  tmp = client;
                    t_cli*  del;

                    if (tmp->fd == it->fd) {
                        client = tmp->next;
                        free(it);
                    }
                    else {
                        while (tmp->next && tmp->next->fd != it->fd)
                            tmp = tmp->next;
                        del = tmp->next;
                        tmp->next = tmp->next->next;
                        free(del);
                    }
                    FD_CLR(it->fd, &fd_write);
                    FD_CLR(it->fd, &fd_read);
                    bzero(buf_one, 100);
                }
                else if (ret != strlen(it->write_buf)) {
                    msg = str_join(msg, it->write_buf + ret);
                    free(it->write_buf);
                    it->write_buf = msg;
                    free(msg);
                    msg = NULL;
                }
                else {
                    free(it->write_buf);
                    it->write_buf = NULL;
                }
            }
        }
    }
    return 0;
}