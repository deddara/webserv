/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 19:53:23 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/28 16:28:54 by awerebea         ###   ########.fr       */
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
						webservVersion = "webserv/0.1a";
						response.data = nullptr;
						response.length = 0;
						errHandlersFlags = 0;
					};

					Response::~Response() {
						// FIXME segfault after hardcoded body
						if (response.data) {
							free(response.data);
							response.data = nullptr;
						}
						if (body) {
							free(body);
							body = nullptr;
						}
					}

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
	location.clear();
	responseHeaders.clear();
	redirectURI.clear();
	filePath.clear();
	fileModifiedTime.clear();
	webservVersion.clear();
	errHandlersFlags = 0;
}

void Response::connectionHandler(int & status) {
	map_type::const_iterator it = _data->find("connection");
	if (it != _data->end() && it->second[0] == "close")
		status = 3;
}

void				Response::buildResponse() {
	std::map<std::string, std::vector<std::string> >::const_iterator	itReq;
	std::map<int, std::vector<std::string> >::const_iterator			itErr;
	char *	numStr = nullptr;

	itReq = _data->find("head");

	// HTTP/1.X
	responseHeaders = itReq->second[2] + " ";

	// errCode
	if(!(numStr = ft_itoa(errCode))) {
		errorExit(2, "");
	}
	responseHeaders.append(numStr);
	free(numStr);
	numStr = nullptr;

	// Message
	itErr = errorPageTempl->find(errCode);
	responseHeaders.append(" " + itErr->second[1] + "\r\n");

	// Server
	responseHeaders.append("Server: " + webservVersion + "\r\n");

	// Date
	responseHeaders.append("Date: " + my_localtime() + "\r\n");

	// Content-Length
	if (bodyLength) {
		if(!(numStr = ft_itoa(bodyLength))) {
			errorExit(2, "");
		}
		responseHeaders.append("Content-Length: ");
		responseHeaders.append(numStr);
		responseHeaders.append("\r\n");
		free(numStr);
		numStr = nullptr;
	}

	// Last-Modified
	if (errCode == 200) {
		responseHeaders.append("Last-Modified: " + fileModifiedTime + "\r\n");
	}

	// Redirect location
	if (errCode == 302) {
		responseHeaders.append("Location: " + redirectURI + "\r\n");
	}

	// Connection
	if (_data->count("connection")) {
		itReq = _data->find("connection");
		responseHeaders.append("Connection: " + itReq->second[0] + "\r\n");
	} else if (errCode > 302) {
		responseHeaders.append("Connection: close\r\n");
	} else {
		responseHeaders.append("Connection: keep-alive\r\n");
	}

	// Append body and make response struct
	responseHeaders.append("\r\n");
	response.length = responseHeaders.length() + bodyLength;
	if(!(response.data = (char*)malloc(responseHeaders.length()))) {
		errorExit(2, "");
	}
	ft_memcpy(response.data, responseHeaders.c_str(), responseHeaders.length());
	if (bodyLength) {
		ft_memcpy(response.data + responseHeaders.length(), body, bodyLength);
	}

	// DEBUG
	write(1, response.data, response.length);
}

void				Response::responsePrepare(int & status, map_type * data) {
	_data = data;

	connectionHandler(status);

	if (errCode) {
		errorHandler();
		status = 3; // QUESTION where should be set and which value
		buildResponse();
		return ;
	} else {
		if (checkLocation()) {
			error404Handler();
			buildResponse();
			return ;
		}
		if (checkFile()) {
			if (errCode == 403) {
				error403Handler();
				buildResponse();
			} else if (errCode == 404) {
				error404Handler();
				buildResponse();
			}
			return ;
		}
		generateBody();
		buildResponse();
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

void				Response::setErrorPagePath(const std::map<int, std::string>
															* errPgPathMap) {
	errorPage = errPgPathMap;
}

void				Response::setErrorPageTempl(const std::map<int,
								std::vector<std::string> > * errPgTemplMap) {
	errorPageTempl = errPgTemplMap;
}

void				Response::generateFilePath() {
	std::map<std::string, std::vector<std::string> >::const_iterator
					it = _data->find("head");
	std::string		cuttedURI =
		it->second[1].substr(location[currLocationInd]->getPrefix().length());

	filePath = location[currLocationInd]->getData().find("root")->second[0];
	if (filePath[filePath.length() - 1] != '/' && cuttedURI.length() &&
			cuttedURI[0] != '/') {
		filePath.append("/");
	}
	filePath.append(cuttedURI);
}

void				Response::generateBody() {
	std::map<std::string, std::vector<std::string> >::const_iterator	itReq;
	std::map<int, std::vector<std::string> >::const_iterator			itErr;

	itReq = _data->find("head");
	if (errCode == 200 && itReq->second[0] == "GET") {
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
				tmp = nullptr;
			} else {
				if (!(body = (char*)malloc((bodyLength = ret)))) {
					errorExit(2, "");
				}
				ft_memcpy(body, buf, ret);
			}
			if (ret == len) {
				free(buf);
				buf = nullptr;
				if(!(buf = (char*)malloc((len *= 2)))) {
					errorExit(2, "");
				}
			}
		}
		if (ret < 0) {
			errorExit(1, "");
		}
		free(buf);
		return ;
	}
	itErr = errorPageTempl->find(errCode);
	if(!(body = (char*)malloc((bodyLength = itErr->second[0].length())))) {
		errorExit(2, "");
	}
	ft_memcpy(body, itErr->second[0].c_str(), bodyLength);
}

void				Response::generateRedirectURI(int err) {
	std::map<int, std::string>::const_iterator
					it = errorPage->find(err);
	size_t			pos = 0;
	size_t			i = 0;
	std::string		root;
	std::string		prefix;
	for (; i < location.size(); ++i) {
		root = location[i]->getData().find("root")->second[0];
		pos = it->second.find(root, 0);
		if ((!pos && (root[root.length() - 1] == '/'))
				|| (!pos && root.length() == it->second.length())
				|| (!pos && (it->second[root.length()] == '/'
				|| it->second.length() == root.length()))) {
			break ;
		}
	}
	if (i == location.size()) {
		redirectURI = it->second;	// QUESTION what to do if no match error
		return ;					// page path in any location?
	}
	prefix = location[i]->getPrefix();
	if (it->second.length() > root.length()) {
		redirectURI = it->second.substr(root.length());
		if (redirectURI[0] == '/' && prefix[prefix.length() - 1] == '/') {
			redirectURI.insert(0, prefix.substr(0, prefix.length() - 1));
		} else if (redirectURI[0] != '/'
				&& prefix[prefix.length() - 1] != '/') {
			redirectURI.insert(0, prefix + "/");
		} else {
			redirectURI.insert(0, prefix);
		}
	} else {
		redirectURI = prefix;
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
			fileModifiedTime = timeToStr(statbuf.st_mtime); // FIXME -1y
			// DEBUG
			// std::cout << timeToStr(statbuf.st_mtime) << std::endl;
			// std::cout << my_localtime() << std::endl;
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

struct s_response &	Response::getResponseStruct() {
	return response;
}
