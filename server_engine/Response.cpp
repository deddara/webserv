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
		// if (ok()){
		//     return 1;
		// }
		std::map<int, std::string>::const_iterator	it;
		struct stat									errPgStatus;
		if (checkLocation()) {
			if ((it = pr_errorPage->find(404)) != pr_errorPage->end()) {
				; // TODO check if error_page is read accesable
			} else {
				// generteResponseHead(); // TODO add implementation
				return ;
			}
		}
		// DEBUG
		for (std::map<int, std::string>::const_iterator it =
				pr_errorPage->begin(); it != pr_errorPage->end(); ++it)
			std::cout << it->second << std::endl;
	}
	return ;
}

void				Response::setErrorPage(const std::map<int, std::string>
																* mapErrPg) {
	pr_errorPage = mapErrPg;
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

int					Response::checkUri() {

	return 0;
}

