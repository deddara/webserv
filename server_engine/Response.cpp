#include "Response.hpp"
#include <vector>
#include <map>

void Response::connectionHandler(int & status) {
	map_type::const_iterator it = _data->find("connection");
	if (it != _data->end() && it->second[0] == "close")
		status = 3;
}

void Response::response_prepare(int & status, map_type * data) {

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
			// error404Handler(); // TODO
		}
		error403Handler(); // DEBUG
		std::cout << redirectURI << std::endl; // DEBUG
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

void				Response::generateBody() {

}

void				Response::generateRedirectURI(int err) {
	std::map<int, std::string>::const_iterator
					it = errorPage->find(err);
	size_t			pos = 0;
	size_t			i = 0;
	for (; i < location.size(); ++i) {
		if (((location[i]->getRoot()[location[i]->getRoot().length() - 1]
				== '/') && !(pos = it->second.find(location[i]->getRoot()), 0))
				|| (!(pos = it->second.find(location[i]->getRoot()), 0)
				&& (it->second[location[i]->getRoot().length()] == '/'
				|| it->second.length() == location[i]->getRoot().length()))) {
			std::cout << "ErrPage path with location match found"
				<< std::endl; // DEBUG
			break ;
		}
	}
	if (i == location.size()) {
		// errCode = 404;	// QUESTION what to do if no match error page path
						// in any location
		return ;
	}
	if (it->second.length() > location[i]->getRoot().length()) {
		redirectURI = it->second.substr(location[i]->getRoot().length());
		if (redirectURI[0] == '/' &&
				location[i]->getPrefix()[location[i]->getPrefix().length() - 1]
				== '/') {
			redirectURI.insert(0, location[i]->getPrefix().substr(0,
										location[i]->getPrefix().length() - 1));
		} else {
			redirectURI.insert(0, location[i]->getPrefix());
		}
	} else {
		redirectURI = location[i]->getPrefix();
	}
}

void				Response::error403Handler() {
	struct stat		statbuf;
	std::map<int, std::string>::const_iterator
					it;
	if (errorPage->size()) {
		if (errorPage->count(403)) {
			if ((it = errorPage->find(403)) != errorPage->end()) {
				if (!(stat(it->second.c_str(), & statbuf))) {
					if (statbuf.st_mode & S_IRUSR) {
						generateRedirectURI(403); // TODO function
						errCode = 302;
						return ;
					} else {
						errCode = 403;
						// generateBody(errCode); // TODO function
						return ;
					}
				}
				errCode = 404;
				// generateBody(errCode); // TODO function
				return ;
			}
		}
	}
	errCode = 403;
	// generateBody(errCode); // TODO function
}

int					Response::checkLocation() {
	std::map<std::string, std::vector<std::string> >::const_iterator
					it = _data->find("head");
	std::cout << it->second[1] << std::endl; // DEBUG
	size_t			pos = 0;
	size_t			i = 0;
	for (; i < location.size(); ++i) {
		if ((!(pos = it->second[1].find(location[i]->getPrefix()), 0)
				&& location[i]->getPrefix()[location[i]->getPrefix().length()
				- 1] == '/') || (!pos &&
				(it->second[1][location[i]->getPrefix().length()] == '/' ||
				it->second[1].length() == location[i]->getPrefix().length()))) {
			std::cout << "Location match found" << std::endl; // DEBUG
			std::cout << location[i]->getPrefix() << std::endl; // DEBUG
			break ;
		}
	}
	if (i == location.size()) {
		errCode = 404;
		return 1;
	}
	return 0;
}
