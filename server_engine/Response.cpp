/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 19:53:23 by awerebea          #+#    #+#             */
/*   Updated: 2021/01/09 20:00:30 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

					Response::Response() {
						_data = nullptr;
						cgi = NULL;
						errCode = 0;
						body = nullptr;
						reqBody = nullptr;
						bodyLength = 0;
						currLocationInd = std::string::npos;
						errorPage = nullptr;
						webservVersion = "webserv/0.1a";
						response.data = nullptr;
						response.length = 0;
						errHandlersFlags = 0;
					};

					Response::~Response() {
						if (response.data) {
							free(response.data);
							response.data = nullptr;
						}
						if (body) {
							free(body);
							body = nullptr;
						}
						if (cgi)
							delete cgi;
					}

void				Response::clearResponseData() {
	if (response.data) {
		free(response.data);
		response.data = nullptr;
		response.length = 0;
	}
	if (body) {
		free(body);
		body = nullptr;
		bodyLength = 0;
	}
	responseHeaders.clear();
	redirectURI.clear();
	filePath.clear();
	fileModifiedTime.clear();
}

void Response::connectionHandler(int & status) {
	map_type::const_iterator it = _data->find("connection");
	if (it != _data->end() && it->second[0] == "close")
		status = 3;
}

void Response::cgi_response_parser(Cgi const &cgi){
	std::map<std::string, std::vector<std::string> >::const_iterator	itReq;
	std::map<int, std::vector<std::string> >::const_iterator			itErr;
	char const *	cgi_buff = cgi.getResponse();
	std::string		cgi_buff_str = std::string(cgi_buff);
	char *			numStr = nullptr;
	std::string		cgi_headers;
	size_t			pos = 0;

	cgi_headers = cgi_buff_str.substr(0, cgi_buff_str.find("\r\n\r\n") + 2);

	itReq = _data->find("head");
	// HTTP/1.X
	responseHeaders = itReq->second[2] + " ";
	pos = cgi_headers.find("Status");
	if (pos != std::string::npos){
		pos += 8;
		while (cgi_headers[pos] != '\r')
		{
			responseHeaders.push_back(cgi_headers[pos]);
			pos++;
		}
		pos += 2;
		responseHeaders.append("\r\n");
	}
	// errCode
	else {
		pos = 0;
		if (!(numStr = ft_itoa(errCode))) {
			throw std::runtime_error("Error: malloc fails");
		}
		responseHeaders.append(numStr);
		free(numStr);
		numStr = nullptr;

		// Message
		itErr = errorPageTempl->find(errCode);
		responseHeaders.append(" " + itErr->second[1] + "\r\n");
	}
	while (cgi_headers[pos]){
		responseHeaders.push_back(cgi_headers[pos]);
		pos++;
	}
	// Server
	responseHeaders.append("Server: " + webservVersion + "\r\n");

	// Date
	responseHeaders.append("Date: " + my_localtime() + "\r\n");

	pos = cgi_buff_str.find("\r\n\r\n") + 4;
	int content_len = cgi.getBytes().getBytes() - pos;
	responseHeaders.append("Content-Length: ");
	responseHeaders.append(std::to_string(content_len));
	responseHeaders.append("\r\n");
	responseHeaders.append("Connection: close\r\n\r\n");
	bodyLength = content_len;


	response.length = responseHeaders.size() + bodyLength;
	if(!(response.data = (char*)malloc(response.length))) {
		throw std::runtime_error("Error: malloc fails");
	}
	ft_memcpy(response.data, responseHeaders.c_str(), responseHeaders.length());
	if (bodyLength) {
		ft_memcpy(response.data + responseHeaders.length(), cgi_buff + pos,
				bodyLength);
	}
};


void				Response::buildResponse() {
	std::map<std::string, std::vector<std::string> >::const_iterator	itReq;
	std::map<int, std::vector<std::string> >::const_iterator			itErr;
	char *	numStr = nullptr;

	// HTTP/1.X
	if (errCode == 400) {
		responseHeaders = "HTTP/1.1 ";
	} else {
		itReq = _data->find("head");
		responseHeaders = itReq->second[2] + " ";
	}

	// errCode
	if(!(numStr = ft_itoa(errCode))) {
		throw std::runtime_error("Error: malloc fails");
	}
	responseHeaders.append(numStr);
	free(numStr);
	numStr = nullptr;

	// Message
	itErr = errorPageTempl->find(errCode);
	responseHeaders.append(" " + itErr->second[1] + "\r\n");

	// Server
	webservVersion = "webserv0.1";
	responseHeaders.append("Server: " + webservVersion + "\r\n");

	// Date
	responseHeaders.append("Date: " + my_localtime() + "\r\n");

	// Content-Length
	if (bodyLength) {
		if(!(numStr = ft_itoa(bodyLength))) {
			throw std::runtime_error("Error: malloc fails");
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

	// Authorization in request required
	if (errCode == 401) {
		responseHeaders.append("WWW-Authenticate: Basic\r\n");
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

//	responseHeaders.append("Content-Type: image/svg+xml\r\n");

	// Append body and make response struct
	responseHeaders.append("\r\n");
	response.length = responseHeaders.length() + bodyLength;
	if(!(response.data = (char*)malloc(response.length))) {
		throw std::runtime_error("Error: malloc fails");
	}
	ft_memcpy(response.data, responseHeaders.c_str(), responseHeaders.length());
	if (bodyLength) {
		ft_memcpy(response.data + responseHeaders.length(), body, bodyLength);
	}

	// DEBUG
	// write(1, response.data, response.length);
}

void				Response::responsePrepare(int & status, map_type * data,
												const cgi_data & _cgi_data) {
	_data = data;

	connectionHandler(status);

	try {
		if (errCode) {
			errorHandler();
			buildResponse();
			status = 3; // QUESTION where should be set and which value
			return ;
		} else {
			int			ret = 0;
			if ((ret = checkLocation())) {
				if (ret == 1) {
					error404Handler();
				}
				else if (ret == 2) {
					generateBody();
				}
				buildResponse();
				status = 3; // QUESTION where should be set and which value
				return ;
			}

			// check authorization
			if ((ret = checkAuth())) {
				if (ret == 1) {
					errCode = 401;
					errorHandler();
					buildResponse();
				} else if (ret == 2) {
					error403Handler();
					buildResponse();
					status = 3; // QUESTION where should be set and which value
				}
				return;
			}

			if (checkAllowMethods()) {
				errorHandler();
				buildResponse();
				status = 3; // QUESTION where should be set and which value
				return ;
			}
			if (checkFile()) {
				if (errCode == 302) {
					buildResponse();
				} else if (errCode == 403) {
					error403Handler();
					buildResponse();
				} else if (errCode == 404) {
					error404Handler();
					buildResponse();
				}
				status = 3; // QUESTION where should be set and which value
				return ;
			}
			// check if CGI settings is present in current location
			if (location[currLocationInd]->getData().count("cgi_ext")) {
				std::multimap<std::string, std::vector<std::string> >
					::const_iterator	itExt;
				size_t					i = 0;

				itExt = location[currLocationInd]->getData().find("cgi_ext");
				// try to find fileExt in vector of supported cgi-extensions
				for (; i < itExt->second.size(); ++i) {
					if (itExt->second[i] == fileExt) {
						break ;
					}
				}
				// check if fileExt found in supported by config
				if (i < itExt->second.size()) {
					cgi = new Cgi(_cgi_data, filePath,
							location[currLocationInd]->getData().
							find("cgi_bin")->second[i], reqBody);
					if(!cgi->handler()) {
						cgi_response_parser(*cgi);
						return;
					}
				}
			}
			generateBody();
			buildResponse();
		}
		return ;
	}
	catch (std::exception & e) {
		std::cout << e.what() << std::endl
			<< "Generation \"error 500\" response" << std::endl;
		try {
			clearResponseData();
			errCode = 500;
			errorHandler();
			buildResponse();
			status = 3; // QUESTION where should be set and which value
			return ;
		}
		catch (std::exception & e) {
			std::cout << e.what() << std::endl
				<< "Close connection silently" << std::endl;
			status = 3;
			return ;
		}
	}
}

void				Response::errorHandler() {
	if (!errorPage || !errorPage || !errorPage->count(errCode)) {
		generateBody(); // TODO check if all possible templates are implemented
		return ;
	}

	struct stat		statbuf;
	std::map<int, std::string>::const_iterator
					it = errorPage->find(errCode);

	if (!(stat(it->second.c_str(), & statbuf))) {
		if (statbuf.st_mode & S_IRUSR || statbuf.st_mode & S_IRGRP) {
			redirectURI = it->second.substr(it->second.rfind("/"));
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

void				Response::generateFilePath() {
	std::map<std::string, std::vector<std::string> >::const_iterator
					it = _data->find("head");
	std::string		appendixFromURI;

	filePath = location[currLocationInd]->getData().find("root")->second[0];

	// case if the specified location is the file extension (.ext)
	if (location[currLocationInd]->getPrefix()[0] == '.') {
		size_t		pos = 0;
		// get from the URI part from the start to the '?' if exist
		while (pos < it->second[1].length() && it->second[1][pos] != '?') {
			appendixFromURI.push_back(it->second[1][pos++]);
		}
		// append part of path from URI
		filePath.append(appendixFromURI);
		return ;
	}

	// case if the specified location is the path (/path/to/file_or_dir)
	if (location[currLocationInd]->getPrefix()[0] == '/') {
		// get from the URI part from the index position equal length of the
		// location prefix to the end
		appendixFromURI = it->second[1].substr(location[currLocationInd]->
														getPrefix().length());
		// append '/' between root and appendix from URI if needed
		if (filePath[filePath.length() - 1] != '/' && appendixFromURI.length()
				&& appendixFromURI[0] != '/') {
			filePath.append("/");
		}
		filePath.append(appendixFromURI);
		return ;
	}
}

void				Response::generateBody() {
	std::map<std::string, std::vector<std::string> >::const_iterator	itReq;
	std::map<int, std::vector<std::string> >::const_iterator			itErr;

	itReq = _data->find("head");
	if (errCode == 200 && itReq->second[0] == "GET") {
		// check if body is generated directory listing (autoindex)
		if (dirListing.length()) {
			if(!(body = (char*)malloc((bodyLength = dirListing.length())))) {
				throw std::runtime_error("Error: malloc fails");
			}
			ft_memcpy(body, dirListing.c_str(), bodyLength);
			return ;
		// case if body in file
		} else {
		int			fd;
		if ((fd = open(filePath.c_str(), O_RDONLY)) < 0) {
			throw std::runtime_error("Error: file open fails");
		}
		int			ret = 0;
		char *		buf = nullptr;
		size_t		len = 512;
		char *		tmp = nullptr;
		size_t		oldBodyLength = 0;

		if(!(buf = (char*)malloc(len))) {
			throw std::runtime_error("Error: malloc fails");
		}
		while ((ret = read(fd, buf, len)) > 0) {
			if (body) {
				tmp = body;
				oldBodyLength = bodyLength;
				if(!(body = (char*)malloc((bodyLength += ret)))) {
					throw std::runtime_error("Error: malloc fails");
				}
				ft_memcpy(body, tmp, oldBodyLength);
				ft_memcpy(body + oldBodyLength, buf, ret);
				free(tmp);
				tmp = nullptr;
			} else {
				if (!(body = (char*)malloc((bodyLength = ret)))) {
					throw std::runtime_error("Error: malloc fails");
				}
				ft_memcpy(body, buf, ret);
			}
			if (ret == static_cast<int>(len)) {
				free(buf);
				buf = nullptr;
				if(!(buf = (char*)malloc((len *= 2)))) {
					throw std::runtime_error("Error: malloc fails");
				}
			}
		}
		if (ret < 0) {
			throw std::runtime_error("Error: read fails");
		}
		free(buf);
		return ;
		}
	}
	// case if body from error pages template
	itErr = errorPageTempl->find(errCode);
	if(!(body = (char*)malloc((bodyLength = itErr->second[0].length())))) {
		throw std::runtime_error("Error: malloc fails");
	}
	ft_memcpy(body, itErr->second[0].c_str(), bodyLength);
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
			redirectURI = it->second.substr(it->second.rfind("/"));
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
			redirectURI = it->second.substr(it->second.rfind("/"));
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

int					Response::checkAllowMethods() {
	std::map<std::string, std::vector<std::string> >::const_iterator	itReq;
	std::map<std::string, std::vector<std::string> >::const_iterator	itField;

	// find requested method on allowed
	itReq = _data->find("head");
	itField = location[currLocationInd]->getData().find("allow_methods");
	size_t i = 0;
	for (; i < itField->second.size(); ++i) {
		if (itReq->second[0] == itField->second[i]) {
			return 0;
		}
	}
	// requested method is not found in allowed methods for specified location
	if (i == itField->second.size()) {
		errCode = 405;
	}
	return 1;
}

int                 Response::checkAuth() const {
	std::map<std::string, std::vector<std::string>>::const_iterator conf_auth_it =
			location[currLocationInd]->getData().find("auth");
	if (!conf_auth_it->second.empty() && !conf_auth_it->second[0].empty()) {
		std::map<std::string, std::vector<std::string> >::const_iterator it =
				_data->find("authorization");
		if (!it->second.empty() && !it->second[0].empty()) {
			std::string type_creds = it->second[0];
			std::string req_auth = type_creds.substr(type_creds.find(' ') + 1);
			if (decodeBase64(req_auth) == conf_auth_it->second[0]) {
				return 0;
			} else
				return 2; // request auth != location auth -> response(403)
		} else
			return 1; // no authorization in request -> response(401)
	}
	return 0;
}

int					Response::checkLocation() {
	std::map<std::string, std::vector<std::string> >::const_iterator
					it = _data->find("head");
	size_t			pos = 0;
	size_t			i = 0;
	std::string		uri;

	uri = it->second[1];

	// check if URI contains file extension
	if ((pos = uri.find(".", 0)) != std::string::npos) {
		// cut out file extension from URI
		while (pos < uri.length() && uri[pos] != '?') {
			fileExt.push_back(uri[pos++]);
		}
	}

	// check if URI is redirection to error page
	std::map<int, std::string>::const_iterator
					itErrPg;
	if (errorPage) {
		itErrPg = errorPage->begin();
		for (; itErrPg != errorPage->end(); ++itErrPg) {
			if (itErrPg->second.substr(itErrPg->second.rfind("/")) == uri) {
				break ;
			}
		}
		if (itErrPg != errorPage->end()) {
			filePath = itErrPg->second;
			errCode = 200;
			return 2;
		}
	}

	// check if any 'path' location corresponds to the URI
	for (i = 0; i < location.size(); ++i) {
		pos = it->second[1].find(location[i]->getPrefix(), 0);
		if ((!pos &&
				location[i]->getPrefix()[location[i]->getPrefix().length() - 1]
				== '/') || (!pos &&
				(it->second[1][location[i]->getPrefix().length()] == '/' ||
				it->second[1].length() == location[i]->getPrefix().length()))) {
			currLocationInd = i;
			return 0;
		}
	}
	errCode = 404;
	return 1;
}

int					Response::checkFile() {
	generateFilePath();

	std::multimap<std::string, std::vector<std::string> >::const_iterator
					itLocationData;
	std::string		indexPagePath;
	struct stat		statbuf;
	// check if generated path is exist in filesystem
	if (!(stat(filePath.c_str(), & statbuf))) {
		// check if 'filePath' is directory
		if (statbuf.st_mode & S_IFDIR) {
			// if path to dir not ended by '/' init redirect
			if (filePath[filePath.length() - 1] != '/') {
				redirectURI = filePath.substr(location[currLocationInd]->
						getData().find("root")->second[0].length()) + "/";
				errCode = 302;
				return 1;
			}
			// check if owner or group has execute access to directory
			if (statbuf.st_mode & S_IXUSR || statbuf.st_mode & S_IXGRP) {
				// get vector with names of index pages from specified location
					itLocationData = location[currLocationInd]->getData().
																find("index");
				// check if there is such vector
				if (itLocationData !=
						location[currLocationInd]->getData().end()) {
					indexPagePath = filePath;
					struct stat		statbufIndexPage;
					// append '/' to the directory path if needed
					if (indexPagePath[indexPagePath.length() - 1] != '/') {
						indexPagePath.append("/");
					}
					size_t i = 0;
					for (; i < itLocationData->second.size(); ++i) {
						// append filename from vector to directory path
						indexPagePath.append(itLocationData->second[i]);
						// check files from vector existance
						if (!(stat(indexPagePath.c_str(), &statbufIndexPage))) {
							// check if owner or group has read access to file
							if (statbufIndexPage.st_mode & S_IRUSR ||
									statbufIndexPage.st_mode & S_IRGRP) {
								errCode = 200;
								fileModifiedTime =
									timeToStr(statbufIndexPage.st_mtime);
								filePath = indexPagePath;
								return 0;
							} else {
								// no read access to existed file
								errCode = 403;
								return 1;
							}
						}
					}
					// file of index pages vector not found QUESTION
					// errCode = 404;
					// return ;
				}
				// check autoindex status if index pages are not set in location
				// case if autoindex off (return 404)
				if ((itLocationData = location[currLocationInd]->getData().
						find("autoindex")) == location[currLocationInd]->
						getData().end() || itLocationData->second[0] == "off") {
					errCode = 404;
					return 1;
				} else {
					// case if autoindex on
					errCode = 200;
					generateDirListing(); // TODO
					return 0;
				}
			}
			errCode = 404;
			return 1;
		}
		// case if filePath is not directory
		// check if owner or group has read access to file
		else if (statbuf.st_mode & S_IRUSR || statbuf.st_mode & S_IRGRP) {
			errCode = 200;
			fileModifiedTime = timeToStr(statbuf.st_mtime); // FIXME +/- 1 year
			return 0;
		// file is not readble
		} else {
			errCode = 403;
			return 1;
		}
	}
	// file is not found
	errCode = 404;
	return 1;
}

void				Response::generateDirListing() {
	DIR *			dir;
	struct dirent	*s;
	struct stat		statbuf;

	dirListing.append("<html>\n<head><title>Index of ");
	dirListing.append(filePath.substr(location[currLocationInd]->
			getData().find("root")->second[0].length()));
	dirListing.append("</title></head>\n<body>\n<h1>Index of ");
	dirListing.append(filePath.substr(location[currLocationInd]->
			getData().find("root")->second[0].length()));
	dirListing.append("</h1><hr><pre>\n");

	dir = opendir(filePath.c_str());

	int				nameWidth = 70;
	int				sizeWidth = 16;
	char *			numStr = nullptr;
	std::string	date;
	std::string	directoryItem;
	while((s = readdir(dir))) {
		if (!ft_memcmp(s->d_name, ".", 2))
			continue;
		directoryItem = filePath;
		directoryItem.append(s->d_name);
		if (stat(directoryItem.c_str(), &statbuf) < 0) {
			continue;
		}
		date = timeToStr(statbuf.st_mtime);
		dirListing.append("<a href=\"");
		dirListing.append(s->d_name);
		dirListing.append("\">");
		dirListing.append(s->d_name);
		dirListing.append("</a>");
		std::string	spacesName (nameWidth - ft_strlen(s->d_name), ' ');
		dirListing.append(spacesName);
		if (!(numStr = ft_itoa(statbuf.st_size))) {
			throw std::runtime_error("Error: malloc fails");
		}
		dirListing.append(numStr);
		std::string	spacesSize (sizeWidth - ft_strlen(numStr), ' ');
		dirListing.append(spacesSize);
		free(numStr);
		numStr = nullptr;
		dirListing.append(date);
		dirListing.append("\n");
	}
	dirListing.append("</pre><hr></body>\n</html>");
}

void				Response::setErrcode(int const & num) {
	errCode = num;
}

struct s_response &	Response::getResponseStruct() {
	return response;
}

void				Response::setServerData(VirtServer const & obj) {
	virtServ = & obj;
	errorPage = & obj.getErrorPagePath();
	location = obj.getLocation();
	limitClientBody = obj.getLimitClientBody();
}

void				Response::setErrorPageTempl(const std::map<int,
								std::vector<std::string> > * errPgTemplMap) {
	errorPageTempl = errPgTemplMap;
}

VirtServer const &	Response::getVirtServer() const {
	return * virtServ;
}

void Response::setReqBody(const char *str) {
	if (str)
		reqBody = str;
}
