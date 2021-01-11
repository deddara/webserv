#include <iostream>
#include <unistd.h>

void magic(char** argv) {
	int pipes[2];
	pid_t pid;
	pipe(pipes);
	pid = fork();
	if (pid == 0) {
//		char *arggv[3] = {"./cgi_bin/cgi_tester", "./cgi_bin/youpi.bla", 0};
		dup2(pipes[0], 0);
		dup2(pipes[1], 1);

		write(pipes[1], "first_name=Lebrus&last_name=Shupay", 34); // Body если метод пост

		const char *env[18] = {"AUTH_TYPE=",
		                       "GATEWAY_INTERFACE=cgi/1.1",
		                       "SCRIPT_NAME=cgi_tester",
		                       "SERVER_NAME=webserv/1.1",
		                       "SERVER_PORT=8080",
		                       "SERVER_PROTOCOL=HTTP/1.1",
		                       "SERVER_SOFTWARE=web",
		                       "REMOTE_ADDR=178.207.154.253",
		                       "REMOTE_IDENT=",
		                       "REMOTE_USER=",
		                       "PATH_TRANSLATED=/Users/jjeremia/projs/webserv/cgi_bin/cgi_tester",

		                       "QUERY_STRING=first_name=Lebrus&last_name=Shupay",
		                       "CONTENT_LENGTH=34",
		                       "CONTENT_TYPE=application/x-www-form-urlencoded",
		                       "PATH_INFO=/cgi_bin/cgi_tester",
		                       "REQUEST_METHOD=GET",
		                       "REQUEST_URI=http://127.0.0.1:8080/cgi_bin/cgi_tester?first_name=Lebrus&last_name=Shupay",

		                       nullptr
		};

		int pp = execve("cgi_bin/cgi_tester", argv, (char **)env);
		std::cout << pp << std::endl;
		exit(pp);
	} else {
		wait(nullptr);
		char line[1000];
		read(pipes[0], line, 1000);
		close(pipes[0]);
		close(pipes[1]);
		std::cout << line << std::endl;
	}
}
int main(int argc, char **argv) {
	magic(argv);

	(void) argc;
}

