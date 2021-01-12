#include <dirent.h>
#include <iostream>
#include <sys/stat.h>
#include "./includes/includes.hpp"

void generate_autoindex_off_page(const char *path)
{
	DIR 	*dir;
	std::string page;
	struct dirent	*s;
	struct stat file_inf;

	page.append("<html>\n"
			 "<head><title>Index of ");
	page.append(path);
	page.append("</title></head>\n"
				"<body>\n"
				"<h1>Index of ");
	page.append(path);
	page.append("</h1><hr><pre>\n");

	dir = opendir(path);

	while((s = readdir(dir)))
	{
		std::string date;
		std::string file_path = path;
		file_path.append(s->d_name);
		if (stat(file_path.c_str(), &file_inf) < 0)
		{
			perror("stat");
		}
		date = timeToStr(file_inf.st_mtimespec.tv_sec);
		std::cout << date << std::endl;
		page.append("<a href=\"");
		page.append(s->d_name);
		page.append("\">");
		page.append(s->d_name);
		page.append("</a>    ");
		page.append(std::to_string(file_inf.st_size));
		page.append("    ");
		page.append(date);
		page.append("\n");
	}
	page.append("</pre><hr></body>\n"
				"</html>");
	std::cout << page;
}

int main()
{
//	struct stat file_inf;
//
//	if (stat("/Users/deddara/", &file_inf) <= 0)
//	{
//		perror("stat");
//		return 1;
//	}
//	std::cout << file_inf.st_size;
	generate_autoindex_off_page("/Users/deddara/Downloads/");

}