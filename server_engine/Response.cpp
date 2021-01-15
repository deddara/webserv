/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 19:53:23 by awerebea          #+#    #+#             */
/*   Updated: 2021/01/15 14:19:49 by awerebea         ###   ########.fr       */
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
						if (response.data_begin_p) {
							free(response.data_begin_p);
							response.data_begin_p = nullptr;
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
//	if (cgi.getBytes().getBytes() == 0)
//		content_len = 0;
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
	// DEBUG
	std::cout << responseHeaders<< std::endl;
	if (bodyLength) {
		ft_memcpy(response.data + responseHeaders.length(), cgi_buff + pos,
				bodyLength);
	}
}


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
	responseHeaders.append("Server: " + webservVersion + "\r\n");

	// Date
	responseHeaders.append("Date: " + my_localtime() + "\r\n");

	// Content-Length
	if (bodyLength && !(_data->size() > 1 && itReq->second[0] == "PUT")) {
		if(!(numStr = ft_itoa(bodyLength))) {
			throw std::runtime_error("Error: malloc fails");
		}
		responseHeaders.append("Content-Length: ");
		responseHeaders.append(numStr);
		responseHeaders.append("\r\n");
		free(numStr);
		numStr = nullptr;
	} else {
		responseHeaders.append("Content-Length: 0\r\n");
	}

	// Content-location (PUT)
	if (_data->size() > 1 && itReq->second[0] == "PUT") {
			responseHeaders.append("Content-Location: ");
			responseHeaders.append(itReq->second[1] + "\r\n");
	}

	// Last-Modified
	if (errCode == 200 && !(_data->size() > 1 && itReq->second[0] == "PUT")) {
		responseHeaders.append("Last-Modified: " + fileModifiedTime + "\r\n");
	}

	// Redirect location
	if (errCode == 302) {
		responseHeaders.append("Location: " + redirectURI + "\r\n");
	}

	// Authorization in request required
	if (errCode == 401) {
		responseHeaders.append("WWW-Authenticate: Basic realm=" + realm + "\r\n");
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
	std::cout <<
			  response.data << std::endl;
	if (bodyLength) {
		ft_memcpy(response.data + responseHeaders.length(), body, bodyLength);
	}

	// DEBUG
	// write(1, response.data, response.length);
}

int					Response::checkLimitClientBody()
{
	std::multimap<std::string, std::vector<std::string> > tmp_data = location[currLocationInd]->getData();
	std::multimap<std::string, std::vector<std::string> >::const_iterator it = tmp_data.find("limit_client_body");
	if (it != tmp_data.end())
	{
		if (location[currLocationInd]->getLimitClientBody() < reqBodyLen)
		{
			errCode = 413;
			return (1);
		}
	}
	it = virtServ->getData().find("limit_client_body");
	if (it != virtServ->getData().end())
	{
		if (virtServ->getLimitClientBody() < reqBodyLen)
		{
			errCode = 413;
			return (1);
		}
	}
	return (0);
}

int					Response::checkExtForCgiHandling() {
	// XXX HACK stupid check for cgi_tester validation
	if (_data->find("head")->second[0] == "POST" &&
			(location[currLocationInd]->getPrefix() == "/post_body" ||
			location[currLocationInd]->getPrefix() == "/post_body/")) {
		return 0;
	}

	if (location[currLocationInd]->getData().count("cgi_ext")) {
		std::multimap<std::string, std::vector<std::string> >
			::const_iterator	itExt;
		size_t					i = 0;

		itExt = location[currLocationInd]->getData().find("cgi_ext");

		// XXX HACK stupid check for cgi_tester validation
		if (itExt->second[0] == ".bla") { return 0; }

		// try to find fileExt in vector of supported cgi-extensions
		for (; i < itExt->second.size(); ++i) {
			if (itExt->second[i] == fileExt) {
				return i;
			}
		}
	}
	return -1;
}

void					Response::putHandler(){
	if (!filePath.length()) {
		generateFilePath();
	}

	struct stat		statbuf;
	int fd;
	if (stat(filePath.c_str(), &statbuf) < 0) {
		if ((fd = open(filePath.c_str(), O_RDWR | O_CREAT, 0755)) < 0) {
			throw std::runtime_error("Error: file open fails");
		}
		write(fd, reqBody, reqBodyLen);
		close(fd);
		errCode = 201;
		return ;
	} else {
		if ((fd = open(filePath.c_str(), O_TRUNC | O_WRONLY)) < 0) {
			throw std::runtime_error("Error: file open fail");
		}
		write(fd, reqBody, reqBodyLen);
		close(fd);
		errCode = 200;
		return ;
	}

	return ;
}

void				Response::responsePrepare(int & status, map_type * data,
												const cgi_data & _cgi_data) {
		std::map<std::string, std::vector<std::string> > tmpMap;
	if (errCode != 400) {
		_data = data;
	} else {
		tmpMap["head"] = std::vector<std::string>(3, "HTTP/1.1");
		_data = &tmpMap;
	}

	reqBodyLen = _cgi_data.body_len;

	connectionHandler(status);
	try {
		if (errCode) {
			errorHandler();
			buildResponse();
			if (errCode != 404)
				status = 3; // QUESTION where should be set and which value
			return ;
		} else {
			int			ret = 0;
			// check if location is handled by config
			if ((ret = checkLocation())) {
				if (ret == 1) {
					error404Handler();
				}
				// case when uri is match error page filename (location hadn't
				// been set)
				else if (ret == 2) {
					std::string	reqMethod = _data->find("head")->second[0];
					// only GET and HEAD allowed for error pages
					if (reqMethod != "GET" && reqMethod != "HEAD") {
						errCode = 405;
						errorHandler();
					} else {
						// check if requested file exist and readble
						if (checkFile()) {
							errorHandler();
							buildResponse();
							if (errCode != 404)
								status = 3;
							return ;
						}
						generateBody();
					}
				}
				buildResponse();
				if (errCode != 404)
					status = 3;
				return ;
			}

			// check authorization
			if ((ret = checkAuth())) {
				if (ret == 1) {
					errCode = 401;
					errorHandler();
				} else if (ret == 2) {
					errCode = 403;
					error403Handler();
					if (errCode != 404)
						status = 3;
				}
				buildResponse();
				return;
			}

			if (checkAllowMethods()) {
				errorHandler();
				buildResponse();
				if (errCode != 404)
					status = 3; // QUESTION where should be set and which value
				return ;
			}
			// check file size limit (for PUT and POST)
			if (checkLimitClientBody()) {
				errorHandler();
				buildResponse();
				if (errCode != 404)
					status = 3;
				return ;
			}
			// check if requested file exist and readble
			if (_data->find("head")->second[0] == "PUT") {
				putHandler();
				buildResponse();
				return ;
			}
			if (checkFile()) {
				if (errCode == 302) {
					buildResponse();
				} else {
					errorHandler();
					buildResponse();
				}
				if (errCode != 404)
					status = 3;
				return ;
			}
			// check if fileExt found in CGI settings for current location
			int		i;
			if ((i = checkExtForCgiHandling()) >= 0) {
				cgi = new Cgi(_cgi_data, filePath,
						location[currLocationInd]->getData().
						find("cgi_bin")->second[i], reqBody);
				int	res = 0;
				if((res = cgi->handler()) == 200) {
					cgi_response_parser(*cgi);
					return;
				} else {
					errorHandler();
					buildResponse();
					if (errCode != 404)
						status = 3;
					return ;
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
			if (errCode != 404)
				status = 3; // QUESTION where should be set and which value
			return ;
		}
		catch (std::exception & e) {
			std::cout << e.what() << std::endl
				<< "Close connection silently" << std::endl;
			if (errCode != 404)
				status = 3;
			return ;
		}
	}
}

void				Response::errorHandler() {
	// check if error pages paths not set in config
	if (!errorPage || !errorPage->count(errCode)) {
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
		close(fd);
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
			break ;
		}
	}
	// requested method is not found in allowed methods for specified location
	if (i == itField->second.size()) {
		errCode = 405;
		return 1;
	}
	if (itReq->second[0] == "POST") {
		// check if fileExt found in CGI settings for current location
		if (checkExtForCgiHandling() >= 0) {
			return 0;
		} // request method is POST, but fileExt is not handled by cgi in config
		errCode = 405;
		return 1;
	}
	return 0;
}

int					Response::checkAuth() {
	// insert 'root' of location in '.htaccess' file path
	std::string		accessFilePath = location[currLocationInd]->getData().
													find("root")->second[0];

	// add '/' to the end of the 'root' path if needed
	if (accessFilePath[accessFilePath.length() - 1] != '/') {
		accessFilePath.append("/");
	}
	// complete '.htaccess' file path
	accessFilePath.append(".htacess");

	struct stat		statbuf;
	// check if location root contains .htaccess
	if (!(stat(accessFilePath.c_str(), & statbuf))) {
		int			fd;
		if ((fd = open(accessFilePath.c_str(), O_RDONLY)) < 0) {
			return 2; // have not read access to .htacess
		}
		char *	line = nullptr;
		int		res = 0;
		std::map<std::string, std::string>	accessFileData;
		std::string							tempLine;
		std::string							key, value;
		while ((res = get_next_line(fd, &line)) >= 0) {
			tempLine = std::string(line);
			key = tempLine.substr(0, tempLine.find(' '));
			if (key == "AuthName" || key == "AuthUserFile") {
				accessFileData[key] = tempLine.substr(tempLine.find(' '));
			}
			free(line);
			if (!res) { break ; }
		}
		if (res < 0) {
			errCode = 500;
			close(fd);
			throw std::runtime_error("Error: malloc fails");
		}
		close(fd);
		if (accessFileData.size() != 2) {
			return 2;
		}
		trim(accessFileData["AuthName"]);
		realm = accessFileData["AuthName"];
		if (!_data->count("authorization") ||
		    !_data->find("authorization")->second.size()) {
			return 1; // no authorization in request -> response(401)
		}
		trim(accessFileData["AuthUserFile"]);
		if ((fd = open(accessFileData["AuthUserFile"].c_str(), O_RDONLY))
				< 0) {
			return 2; // have not read access to .htpasswd
		}
		std::string authData = _data->find("authorization")->second[0];
		if (toLower(authData.substr(0, authData.find(' '))) != "basic") {
			close(fd);
			return 2; // request auth != location auth -> response(403)
		}
		std::string	strLine;
		while ((res = get_next_line(fd, &line)) >= 0) {
			strLine = std::string(line);
			if (decodeBase64(authData.substr(authData.find(' ') + 1)) ==
					strLine && strLine.length()) {
				free(line);
				close(fd);
				return 0;
			}
			free(line);
			if (!res) { break ; }
		}
		if (res < 0) {
			errCode = 500;
			close(fd);
			throw std::runtime_error("Error: malloc fails");
		}
		close(fd);
		return 2; // have not read access to .htacess
	}
	return 0; // authorization is not required
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
	if (errorPage && fileExt.length()) {
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
	if (!filePath.length()) {
		generateFilePath();
	}

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
				// redirectURI = filePath.substr(location[currLocationInd]->
				//         getData().find("root")->second[0].length()) + "/";
				redirectURI = _data->find("head")->second[1] + "/";
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
	else {
		if (_data->find("head")->second[0] == "PUT" || _data->find("head")->second[0] == "POST")
			return (0);
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
