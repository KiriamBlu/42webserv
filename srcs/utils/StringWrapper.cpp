#include "StringWrapper.hpp"

stringWrap::stringWrap() {
	deq = std::deque<std::string>();
}

stringWrap::stringWrap(const stringWrap &other) {
	*this = other;
}

stringWrap::~stringWrap() {}

stringWrap& stringWrap::operator=(const stringWrap &other) {
	deq = other.deq;
	return *this;
}

stringWrap& stringWrap::operator+=(std::string data) {
	if (deq.empty() && data != "")
		deq.push_back("");
	while (data != "") {
		if (deq.back().size() == MAX_STRING_SIZE)
			deq.push_back("");
		std::string& last = deq.back();
		size_t sizeAppend = MAX_STRING_SIZE - last.size();
		last += data.substr(0, sizeAppend);
		data = sizeAppend >= data.size() ? "" : data.substr(sizeAppend);
	}
	return *this;
}

stringWrap& stringWrap::operator+=(const stringWrap other) {
	for (std::deque<std::string>::const_iterator it = other.deq.begin(); it != other.deq.end(); it++)
		*this += *it;
	return *this;
}

std::string stringWrap::popFirst() {
	if (deq.empty())
		return "";
	std::string ret = deq.front();
	deq.pop_front();
	return ret;
}

bool stringWrap::empty() const {
	return deq.empty();
}

std::string stringWrap::substr(size_t begin, size_t len) const {
	std::string ret;

	for (size_t i = begin; i - begin < len;) {
		if (i >= length())
			break;
		ret += deq[i / MAX_STRING_SIZE].substr(i % MAX_STRING_SIZE, begin + len - i);
		if (i == begin)
			i -= i % MAX_STRING_SIZE;
		i += len >= MAX_STRING_SIZE + i - begin ? MAX_STRING_SIZE : begin + len - i;
	}
	return ret;
}

stringWrap stringWrap::subdeque(size_t begin, size_t len) const {
	stringWrap ret;

	for (size_t i = begin; i - begin < len;) {
		if (i >= length())
			break;
		ret += deq[i / MAX_STRING_SIZE].substr(i % MAX_STRING_SIZE, begin + len - i);
		if (i == begin)
			i -= i % MAX_STRING_SIZE;
		i += len >= MAX_STRING_SIZE + i - begin ? MAX_STRING_SIZE : begin + len - i;
	}
	return ret;
}

size_t stringWrap::length() const {
	if (deq.size() == 0)
		return 0;
	return MAX_STRING_SIZE * (deq.size() - 1) + deq.back().size();
}

size_t stringWrap::find(std::string toFind, size_t pos) const {
	std::string aux;
	size_t		i;
	size_t		ret;

	i = pos / MAX_STRING_SIZE;
	if (deq.size() == 0 || i > deq.size() - 1)
		return std::string::npos;
	aux = deq[i].substr(pos % MAX_STRING_SIZE);
	if (i + 1 < deq.size())
		aux += deq[i + 1];
	ret = aux.find(toFind);
	if (ret != std::string::npos)
		return ret + pos;
	i++;

	while (i + 1 < deq.size()) {
		aux = deq[i] + deq[i + 1];
		ret = aux.find(toFind);
		if (ret != std::string::npos)
			return ret + i * MAX_STRING_SIZE;
		i++;
	}
	return std::string::npos;
}

char& stringWrap::operator[](size_t pos) {
	return deq[pos / MAX_STRING_SIZE][pos % MAX_STRING_SIZE];
}

const std::deque<std::string> &stringWrap::getDeque() const {
	return deq;
}

std::ostream &operator<<(std::ostream &o, const stringWrap &prt) {
	const std::deque<std::string>& pr = prt.getDeque();

	for (std::deque<std::string>::const_iterator it = pr.begin(); it < pr.end(); it++)
		o << *it << std::endl;
	return o;
}