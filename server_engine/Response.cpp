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

	if (err_code == 400) {
			return ;
		status = 3;
	}
	else {
		std::map<int, std::string>::const_iterator	it;
		struct stat									errPgStatus;
		if (checkLocation()) {
			if ((it = errorPage->find(404)) != errorPage->end()) {
				; // TODO check if error_page is read accesable
			} else {
				// generteResponseHead(); // TODO add implementation
				return ;
			}
		}
		// DEBUG
		for (std::map<int, std::string>::const_iterator it =
				errorPage->begin(); it != errorPage->end(); ++it)
			std::cout << it->second << std::endl;
	}
	return ;
}

void				Response::setErrorPage(const std::map<int, std::string>
																* errPg) {
	errorPage = errPg;
}

int					Response::checkIfFileExist(std::string const & path) {
	struct stat		buf;
	int				res;
	if (!(res = stat(path.c_str(), & buf))) {
		return 1;
	}
	return 0;
}

void				Response::error403Handler() {
	std::map<int, std::string>::const_iterator it;
	if (errorPage->size()) {
		if (errorPage->count(403)) {
			if ((it = errorPage->find(403)) != errorPage->end()) {
				if (checkIfFileExist(it->second)) {
				//     if (checkIfFileReadble(it->second())) {

				//     }
				// }// TODO function
				}
			}
		}
	}
	err_code = 403;
	// generateBody(err_code); // TODO function
}

int					Response::checkLocation() {
	std::map<std::string, std::vector<std::string> >::const_iterator
					it = _data->find("head");
	std::cout << it->second[1] << std::endl; // DEBUG
	size_t			pos = 0;
	size_t			i = 0;
	for (; i < location.size(); ++i) {
		if (!(pos = it->second[1].find(location[i]->getPrefix()), 0) &&
				(it->second[1][location[i]->getPrefix().length()] == '/' || \
				it->second[1].length() == location[i]->getPrefix().length())) {
			std::cout << "Location match found" << std::endl; // DEBUG
			break ;
		}
	}
	std::cout << location[i]->getPrefix() << std::endl; // DEBUG
	if (i == location.size()) {
		err_code = 404;
		return 1;
	}
	return 0;
}
