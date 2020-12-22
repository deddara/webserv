#include "Bytes.hpp"

Bytes::Bytes() : bytes_count(0) {}

const int & Bytes::getBytes() { return bytes_count; }

void Bytes::setBytes(const int &num) { bytes_count = num; }