/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 19:53:23 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/26 01:47:21 by awerebea         ###   ########.fr       */
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
						response.data = nullptr;
						response.length = 0;
						errHandlersFlags = 0;
					};

void				Response::errorExit(int code, std::string const & word) {
	std::string		errors[3] = {
		"Error: file open fails",
		"Error: read fails",
		"Error: malloc fails",
	};
	std::cout << errors[code] << std::endl;
	exit(1);
}

void				Response::clearResponseData() {
	_data = nullptr;
	errCode = 0;
	if (response.data) {
		free(response.data);
		response.data = nullptr;
	}
	if (body) {
		free(body);
		body = nullptr;
	}
	bodyLength = 0;
	currLocationInd = std::string::npos;
	errorPage = nullptr;
	errHandlersFlags = 0;
}

void Response::connectionHandler(int & status) {
	map_type::const_iterator it = _data->find("connection");
	if (it != _data->end() && it->second[0] == "close")
		status = 3;
}

void				Response::buildResponse() {

}

void				Response::responsePrepare(int & status, map_type * data) {

	_data = data;

	connectionHandler(status);

	if (errCode) {
		errorHandler();
		status = 3; // QUESTION where should be set and which value
		// buildResponse(); // TODO
		return ;
	} else {
		if (checkLocation()) {
			error404Handler();
			// buildResponse(); // TODO
			return ;
		}
		if (checkFile()) {
			if (errCode == 403) {
				error403Handler();
				// buildResponse(); // TODO
			} else if (errCode = 404) {
				error404Handler();
				// buildResponse(); // TODO
			}
			return ;
		}
		generateBody();
		// buildResponse(); // TODO
		return ;
	}
	return ;
}

void				Response::errorHandler() {
	if (!errorPage->count(errCode)) {
		generateBody(); // TODO check if all possible templates are implemented
		return ;
	}

	struct stat		statbuf;
	std::map<int, std::string>::const_iterator
					it = errorPage->find(errCode);

	if (!(stat(it->second.c_str(), & statbuf))) {
		if (statbuf.st_mode & S_IRUSR || statbuf.st_mode & S_IRGRP) {
			generateRedirectURI(errCode);
			errCode = 302;
			return ;
		} else {
			if (!(errHandlersFlags & 1)) {
				error403Handler();
				return ;
			} else {
			errCode = 403;
			generateBody();
			return ;
			}
		}
	}
	if (!(errHandlersFlags & 2)) {
		error404Handler();
		return ;
	} else {
	errCode = 404;
	generateBody();
	return ;
	}
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
		int			fd;
		if ((fd = open(filePath.c_str(), O_RDONLY)) < 0) {
			errorExit(0, "");
		}
		size_t		ret = 0;
		char *		buf = nullptr;
		size_t		len = 512;
		char *		tmp = nullptr;
		size_t		oldBodyLength = 0;

		if(!(buf = (char*)malloc(len))) {
			errorExit(2, "");
		}
		while ((ret = read(fd, buf, len)) > 0) {
			if (body) {
				tmp = body;
				oldBodyLength = bodyLength;
				if(!(body = (char*)malloc((bodyLength += ret)))) {
					errorExit(2, "");
				}
				ft_memcpy(body, tmp, oldBodyLength);
				ft_memcpy(body + oldBodyLength, buf, ret);
				free(tmp);
			} else {
				if (!(body = (char*)malloc((bodyLength = ret)))) {
					errorExit(2, "");
				}
				ft_memcpy(body, buf, ret);
			}
			if (ret == len) {
				free(buf);
				if(!(buf = (char*)malloc((len *= 2)))) {
					errorExit(2, "");
				}
			} else {
				free(buf);
			}
		}
		if (ret < 0) {
			errorExit(1, "");
		}
		return ;
	}
	std::map<int, std::string>::iterator it = errorPageTemplates.find(errCode);
	if(!(body = (char*)malloc((bodyLength = it->second.length())))) {
		errorExit(2, "");
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

void				Response::error404Handler() {
	errHandlersFlags = (errHandlersFlags | 2); // set 0bXXXXXX1X
	if (!errorPage->count(404)) {
		errCode = 404;
		generateBody();
		return ;
	}

	struct stat		statbuf;
	std::map<int, std::string>::const_iterator
					it = errorPage->find(404);

	if (!(stat(it->second.c_str(), & statbuf))) {
		if (statbuf.st_mode & S_IRUSR || statbuf.st_mode & S_IRGRP) {
			generateRedirectURI(404);
			errCode = 302;
			return ;
		} else {
			if (!(errHandlersFlags & 1)) {
				error403Handler();
				return ;
			} else {
			errCode = 403;
			generateBody();
			return ;
			}
		}
	}
	errCode = 404;
	generateBody();
	return ;
}

void				Response::error403Handler() {
	errHandlersFlags = (errHandlersFlags | 1); // set 0bXXXXXXX1
	if (!errorPage->count(403)) {
		errCode = 403;
		generateBody();
		return ;
	}

	struct stat		statbuf;
	std::map<int, std::string>::const_iterator
					it = errorPage->find(403);

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
	if (!(errHandlersFlags & 2)) {
		error404Handler();
		return ;
	} else {
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
		if (statbuf.st_mode & S_IRUSR || statbuf.st_mode & S_IRGRP) {
			errCode = 200;
			// DEBUG
			std::cout << modifiedTimeToStr(statbuf.st_mtime) << std::endl;
			std::cout << my_localtime() << std::endl;
			return 0;
		} else {
			errCode = 403;
			return 1;
		}
	}
	errCode = 404;
	return 1;
}

void				Response::setLocation(std::vector<Location *> const & loc) {
	location = loc;
}

void				Response::setErrcode(int const & num) {
	errCode = num;
}

const struct s_response &
					Response::getResponseStruct() const {
	return response;
}
