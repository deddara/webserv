/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeremia <jjeremia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 10:45:13 by jjeremia          #+#    #+#             */
/*   Updated: 2020/12/14 10:45:14 by jjeremia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::value_type split(std::string const &raw, char delim) {
	Request::value_type res;
	std::string k;
	for( size_t i = 0; i != raw.size(); ++i) {
		if (raw[i] == delim && k.size() > 1) {
			res.push_back(k);
			k = "";
		} else if (raw[i] != delim)
			k += raw[i];
	}
	if (k != "")
		res.push_back(k);
	return res;
}

void    trim(std::string &str) {
	str.erase(str.find_last_not_of(' ') + 1);
	str.erase(0, str.find_first_not_of(' '));
}

std::string str_to_lower(std::string &str) {
	for (size_t i = 0; i != str.size(); ++i)
		str[i] = tolower(str[i]);
	return str;
}

void Request::check_common() {
	if ((_data["head"][0] != "GET" && _data["head"][0] != "HEAD") ||
		_data["head"][2] != "HTTP/1.1")
		_isbadrequest = true;
	if (!_data["host"].size() || _data["host"][0] == "")
		_isbadrequest = true;
}

Request::Request(std::string const &raw_data) : _isbadrequest(false) {
	bool first_str = true;
	std::size_t pos = 0;
	for(std::size_t i = 0; i != raw_data.size(); i = pos) {
		if (pos >= raw_data.size())
			break;
		pos++;
		pos = raw_data.find("\n", pos);
		if (first_str) {
			std::string fstr = raw_data.substr(0, pos);
			if (std::count(fstr.begin(), fstr.end(),' ') > 2)
				_isbadrequest = true;
			_data["head"] = split(fstr, ' ');
			if (_data["head"].size() != 3)
				_isbadrequest = true;
			first_str = false;
			continue;
		}
		value_type header = split(raw_data.substr(i, pos - i), ':');
		std::string key(++header[0].begin(), header[0].end());
		if (*--key.end() == ' ')
			_isbadrequest = true;
		value_type value(++header.begin(), header.end());
		for(size_t i = 0; i != value.size(); ++i)
			trim(value[i]);
		_data[str_to_lower(key)] = value;
		_data.erase("");
	}
	check_common();
}

Request::Request(Request const &obj) {
	map_type::iterator ite = _data.end();
	for(map_type::iterator it = _data.begin(); it != ite; ++it)
		it->second.clear();
	_data.clear();
	_data.insert(obj._data.begin(), obj._data.end());
}

Request &Request::operator=(Request const &obj) {
	if (this != &obj) {
		map_type::iterator ite = _data.end();
		for(map_type::iterator it = _data.begin(); it != ite; ++it)
			it->second.clear();
		_data.clear();
		_data.insert(obj._data.begin(), obj._data.end());
	}
	return *this;
}

Request::~Request() {
	map_type::iterator ite = _data.end();
	for(map_type::iterator it = _data.begin(); it != ite; ++it)
		it->second.clear();
	_data.clear();
}

bool Request::error() const {
	return _isbadrequest;
}

const Request::value_type &Request::find(std::string const &key) const {
	std::string low_key = static_cast<std::string>(key);
	return _data.find(str_to_lower(low_key))->second;
}