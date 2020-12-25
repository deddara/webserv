/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 19:53:23 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/25 20:06:47 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

					Response::Response() {
						_data = nullptr;
						errCode = 0;
						body = nullptr;
						bodyLength = 0;
						currLocationInd = std::string::npos;
						errorPage = nullptr;
					};

void				Response::errorExit(int code, std::string const & word) {
	std::string		errors[1] = {
		"Error: malloc fails",
	};
	std::cout << errors[code] << std::endl;
	exit(1);
}

void Response::connectionHandler(int & status) {
	map_type::const_iterator it = _data->find("connection");
	if (it != _data->end() && it->second[0] == "close")
		status = 3;
}

void				Response::errorHandler() {
	if (errCode == 304) {
		// erro304handler(); // DEBUG
	} else if (errCode == 403) {
		error403Handler();
	} else if (errCode = 404) {
		// error404Handler(); // DEBUG
	}
}

void				Response::response_prepare(int & status, map_type * data) {

	_data = data;

	connectionHandler(status);

	if (errCode == 400) {
			return ;
		status = 3;
	}
	else {
		std::map<int, std::string>::const_iterator	it;
		// struct stat									errPgStatus;
		if (checkLocation()) {
			// errorHandler(); // TODO
			return ;
		}
		if (checkFile()) {
			std::cout << filePath << std::endl; // DEBUG
			// errorHandler(); // TODO
		}
			std::cout << errCode << std::endl; // DEBUG
			// errorHandler(); // TODO
		// std::cout << filePath << std::endl; // DEBUG
		error403Handler(); // DEBUG
		// std::map<int, std::string>::iterator it2 = errorPageTemplates.find(500);
		// std::cout << it2->second << std::endl; //DEBUG
		// std::cout << redirectURI << std::endl; // DEBUG
		// DEBUG
		// for (std::map<int, std::string>::const_iterator it =
		//         errorPage->begin(); it != errorPage->end(); ++it)
		//     std::cout << it->second << std::endl;
	}
	return ;
}

void				Response::setErrorPage(const std::map<int, std::string>
																* errPg) {
	errorPage = errPg;
}

void				Response::generateFilePath() {
	std::map<std::string, std::vector<std::string> >::const_iterator
					it = _data->find("head");
	std::string		cuttedURI =
		it->second[1].substr(location[currLocationInd]->getPrefix().length());

	filePath = location[currLocationInd]->getRoot();
	if (filePath[filePath.length() - 1] != '/' && cuttedURI.length() &&
			cuttedURI[0] != '/') {
		filePath.append("/");
	}
	filePath.append(cuttedURI);
}

void				Response::generateBody() {
	if (errCode == 200) {
		// TODO function to get body from file
		return ;
	}
	std::map<int, std::string>::iterator it = errorPageTemplates.find(errCode);
	if(!(body = (char*)malloc((bodyLength = it->second.length())))) {
		errorExit(0, "");
	}
	ft_memcpy(body, it->second.c_str(), bodyLength);
}

void				Response::generateRedirectURI(int err) {
	std::map<int, std::string>::const_iterator
					it = errorPage->find(err);
	size_t			pos = 0;
	size_t			i = 0;
	for (; i < location.size(); ++i) {
		pos = it->second.find(location[i]->getRoot(), 0);
		if ((!pos && (location[i]->getRoot()[location[i]->getRoot().length()
				- 1] == '/')) || (!pos && location[i]->getRoot().length() ==
				it->second.length()) || (!pos && (it->second[location[i]->
				getRoot().length()] == '/' || it->second.length() ==
				location[i]->getRoot().length()))) {
			break ;
		}
	}
	if (i == location.size()) {
		redirectURI = it->second;	// QUESTION what to do if no match error
		return ;					// page path in any location?
	}
	if (it->second.length() > location[i]->getRoot().length()) {
		redirectURI = it->second.substr(location[i]->getRoot().length());
		if (redirectURI[0] == '/' && location[i]->getPrefix()[location[i]->
				getPrefix().length() - 1] == '/') {
			redirectURI.insert(0, location[i]->getPrefix().
					substr(0, location[i]->getPrefix().length() - 1));
		} else if (redirectURI[0] != '/' && location[i]->getPrefix()
					[location[i]->getPrefix().length() - 1] != '/') {
			redirectURI.insert(0, location[i]->getPrefix() + "/");
		} else {
			redirectURI.insert(0, location[i]->getPrefix());
		}
	} else {
		redirectURI = location[i]->getPrefix();
	}
}

void				Response::error403Handler() {
	if (!errorPage->count(403)) {
		errCode = 403;
		generateBody();
		// DEBUG
		if (bodyLength) {
			write(1, body, bodyLength);
			std::cout << std::endl;
		}
		return ;
	}

	struct stat		statbuf;
	std::map<int, std::string>::const_iterator
					it;

	if ((it = errorPage->find(403)) != errorPage->end()) {
		if (!(stat(it->second.c_str(), & statbuf))) {
			if (statbuf.st_mode & S_IRUSR) {
				generateRedirectURI(403);
				errCode = 302;
				return ;
			} else {
				errCode = 403;
				generateBody();
				return ;
			}
		}
		errCode = 404;
		generateBody();
		return ;
	}
}

int					Response::checkLocation() {
	std::map<std::string, std::vector<std::string> >::const_iterator
					it = _data->find("head");
	size_t			pos = 0;
	size_t			i = 0;
	for (; i < location.size(); ++i) {
		pos = it->second[1].find(location[i]->getPrefix(), 0);
		if ((!pos &&
				location[i]->getPrefix()[location[i]->getPrefix().length() - 1]
				== '/') || (!pos &&
				(it->second[1][location[i]->getPrefix().length()] == '/' ||
				it->second[1].length() == location[i]->getPrefix().length()))) {
			break ;
		}
	}
	if (i == location.size()) {
		errCode = 404;
		return 1;
	}
	currLocationInd = i;
	return 0;
}

int					Response::checkFile() {
	generateFilePath();

	struct stat		statbuf;

	if (!(stat(filePath.c_str(), & statbuf))) {
		if (statbuf.st_mode & S_IRUSR) {
			// TODO 304 error handling (not modified)
			errCode = 200;
			return 0;
		} else {
			errCode = 403;
			return 1;
		}
	}
	errCode = 404;
	return 1;
}
