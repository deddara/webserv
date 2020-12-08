#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

#include <iostream>
#include "string"
#include "unistd.h"
int main()
{
	std::string query =
			"HEAD / HTTP/1.0\r\n"
			"Host: www.google.com\r\n"
			"\r\n";
	std::string hostname = "www.google.com";

	struct sockaddr_in	sin;
	struct hostent		*h;
	int fd;
	ssize_t 	n_written, remaining;
	char buf[1024];
	if (!(h = gethostbyname(hostname.c_str())))
	{
		std::cout << "Could not lookup" << hostname << " " << hstrerror(h_errno) << std::endl;
		return (1);
	}
	if (h->h_addrtype != AF_INET)
	{
		std::cout << stderr << "No ipv6 support, sorry.\n";
	}

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		perror("socket");
		close(fd);
		return (1);
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(80);
	sin.sin_addr = *(reinterpret_cast<in_addr*>(h->h_addr));

	std::cout << sin.sin_addr << std::endl;

	if (connect(fd, (struct sockaddr*) &sin, sizeof(sin)))
	{
		perror("connect");
		close(fd);
		return 1;
	}

	remaining = query.length();
	while(remaining)
	{
		n_written = send(fd, query.c_str(), remaining, 0);
		if (n_written <= 0)
		{
			perror("send");
			return 1;
		}
		remaining -= n_written;
	}

	while (1){
		ssize_t result = recv(fd, buf, sizeof(buf), 0);
		if (!result)
			break;
		else if (result < 0)
		{
			perror("recv");
			close(fd);
			return 1;
		}
		std::cout << buf;
	}

	std::string str = "ASdsdsadASDdsasd";
	std::cout << s

	return (0);
}