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

//Auxiliary functions

Request::value_type split(std::string const &raw, char delim) {
	Request::value_type res;
	std::string k;
	for( size_t i = 0; i != raw.size(); ++i) {
		if (raw[i] == delim && k.size() >= 1) {
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

// Private functions

void Request::check_common() {
	if ((_data["head"][0] != "GET" &&
	     _data["head"][0] != "HEAD" &&
	     _data["head"][0] != "POST") ||
	     _data["head"][2] != "HTTP/1.1")
	     _isbadrequest = true;
	if (!_data["host"].size() || _data["host"][0].empty())
		_isbadrequest = true;
}

// Public functions

Request::Request(std::string const &raw_data)
											: _isbadrequest(false), _body("") {
	std::string tmp = raw_data;
	trim(tmp);
	if (tmp.empty() || (tmp[0] == '\r' && tmp[1] == '\n')) {
		_isbadrequest = true;
		return;
	}
	bool first_str = true;
	std::size_t pos = 0;
	for(std::size_t i = 0; i != raw_data.size(); i = pos) {
		if (pos >= raw_data.size())
			break;
		pos += 2;
		if (raw_data[pos] == '\n') {
			_body = raw_data.substr(pos + 1, pos + raw_data.size());
			check_common();
			return;
		}
		pos = raw_data.find('\n', pos);
		if (first_str) {
			std::string fstr = raw_data.substr(0, pos - 1);
			if (std::count(fstr.begin(), fstr.end(),' ') > 2)
				_isbadrequest = true;
			_data["head"] = split(fstr, ' ');
			if (_data["head"].size() != 3)
				_isbadrequest = true;
			first_str = false;
			continue;
		}
		value_type header = split(raw_data.substr(i, pos - i - 1), ':');
		std::string key(++header[0].begin(), header[0].end());
		if (*--key.end() == ' ')
			_isbadrequest = true;
		value_type value(++header.begin(), header.end());
		for(size_t i = 0; i != value.size(); ++i)
			trim(value[i]);
		_data[str_to_lower(key)] = value;
	}
	check_common();
}

void	Request::req_init(std::string const &raw_data){
	std::string tmp = raw_data;
	trim(tmp);
	if (tmp.empty() || (tmp[0] == '\r' && tmp[1] == '\n')) {
		_isbadrequest = true;
		return;
	}
	bool first_str = true;
	std::size_t pos = 0;
	for(std::size_t i = 0; i != raw_data.size(); i = pos) {
		if (pos >= raw_data.size())
			break;
		pos += 2;
		if (raw_data[pos] == '\n') {
			_body = raw_data.substr(pos + 1, pos + raw_data.size());
			check_common();
			return;
		}
		pos = raw_data.find('\n', pos);
		if (first_str) {
			std::string fstr = raw_data.substr(0, pos - 1);
			if (std::count(fstr.begin(), fstr.end(),' ') > 2)
				_isbadrequest = true;
			_data["head"] = split(fstr, ' ');
			if (_data["head"].size() != 3)
				_isbadrequest = true;
			first_str = false;
			continue;
		}
		value_type header = split(raw_data.substr(i, pos - i - 1), ':');
		std::string key(++header[0].begin(), header[0].end());
		if (*--key.end() == ' ')
			_isbadrequest = true;
		value_type value(++header.begin(), header.end());
		for(size_t i = 0; i != value.size(); ++i)
			trim(value[i]);
		_data[str_to_lower(key)] = value;
	}
	check_common();
}

Request::Request(Request const &obj)
					: _isbadrequest(obj._isbadrequest), _body(obj._body) {
	map_type::iterator ite = _data.end();
	for(map_type::iterator it = _data.begin(); it != ite; ++it)
		it->second.clear();
	_data.clear();
	_data.insert(obj._data.begin(), obj._data.end());
}

Request &Request::operator=(Request const &obj) {
	if (this != &obj) {
		_isbadrequest = obj._isbadrequest;
		_body = obj._body;
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

Request::map_type::const_iterator Request::begin() const {
	return _data.begin();
}

Request::map_type::const_iterator Request::end() const {
	return _data.end();
}

bool Request::error() const {
	return _isbadrequest;
}

bool Request::is_valid_value(const std::string &key) const {
	std::string low_key = static_cast<std::string>(key);
	value_type value = _data.find(str_to_lower(low_key))->second;
	if (!value.empty() && !value[0].empty())
		return true;
	return false;
}

const Request::value_type &Request::find(std::string const &key) const {
	std::string low_key = static_cast<std::string>(key);
	return _data.find(str_to_lower(low_key))->second;
}

const std::string & Request::get_body() const {
	return _body;
}