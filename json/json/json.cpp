#include "json.h"
#include <cctype>
#include <cmath>

namespace twistoy {
	JsonObject::JsonObject() : intValue_(0), realNumber_(false), realValue_(0), booleanValue_(false),
		type_(ObjectType::NONE), strValue_(""), arrayValue_(), objectValue_() {}
	JsonObject::JsonObject(int val) : JsonObject() {
		intValue_ = val;
		type_ = ObjectType::NUMBER;
		realNumber_ = false;
	}
	JsonObject::JsonObject(double val) : JsonObject() {
		realValue_ = val;
		type_ = ObjectType::NUMBER;
		realNumber_ = true;
	}
	JsonObject::JsonObject(bool val) : JsonObject() {
		type_ = ObjectType::BOOLEAN;
		booleanValue_ = val;
	}
	JsonObject::JsonObject(const std::string& val) : JsonObject() {
		strValue_ = val;
		type_ = ObjectType::STRING;
	}
	JsonObject::JsonObject(const std::vector<JsonObject>& val) : JsonObject() {
		arrayValue_ = val;
		type_ = ObjectType::ARRAY;
	}
	JsonObject::JsonObject(const std::map<std::string, JsonObject>& val) : JsonObject() {
		objectValue_ = val;
		type_ = ObjectType::OBJECT;
	}

	std::string JsonObject::toString() const {
		std::string rv;
		switch (type_) {
		case ObjectType::NUMBER:
			if (realNumber_)
				return std::to_string(realValue_);
			else
				return std::to_string(intValue_);
		case ObjectType::BOOLEAN:
			if (booleanValue_) {
				return "true";
			}
			else {
				return "false";
			}
		case ObjectType::NONE:
			return "null";
		case ObjectType::STRING: // wrong handle
			rv = "\"";
			for (auto item : strValue_) {
				switch (item) {
				case '\n': rv += "\\n"; break;
				case '\b': rv += "\\b"; break;
				case '\r': rv += "\\r"; break;
				case '\f': rv += "\\f"; break;
				case '\t': rv += "\\t"; break;
				default:
					rv.push_back(item);
				}
			} rv.push_back('\"');
			return rv;
		case ObjectType::ARRAY:
			rv = "[";
			for (auto item : arrayValue_) {
				rv += item.toString() + ",";
			}
			if (arrayValue_.size() != 0)
				rv.pop_back();
			rv.push_back(']');
			return rv;
		case ObjectType::OBJECT:
			rv = "{";
			for (auto item : objectValue_) {
				rv += item.first + ":" + item.second.toString() + ",";
			}
			if (objectValue_.size() != 0)
				rv.pop_back();
			rv.push_back('}');
			return rv;
		default:
			return "error";
		}
	}

	std::string JsonObject::getString() const {
		return strValue_;
	}

#ifdef DEBUG
	std::string JsonObject::debugString() const {
		return _debugString(0, false);
	}
	std::string JsonObject::_debugString(int nTabs, bool value) const {
		std::string tabs(""); for (int i = 0; i < nTabs; i++) tabs += "\t";
		std::string rv("");
		switch (type_) {
		case ObjectType::NUMBER:
			if (value)
				return "<number>: " + toString();
			return tabs + "<number>: " + toString();
		case ObjectType::BOOLEAN:
			if (value)
				return "<boolean>: " + toString();
			return tabs + "<boolean>: " + toString();
		case ObjectType::NONE:
			if (value)
				return "<null>: " + toString();
			return tabs + "<null>: " + toString();
		case ObjectType::STRING:
			if (value)
				return "<string>: " + toString();
			return tabs + "<string>: " + toString();
		case ObjectType::ARRAY:
			if (value)
				rv = "<array>:\n";
			else
				rv = tabs + "<array>:\n";
			for (auto item : arrayValue_) {
				rv += item._debugString(nTabs + 1, false) + '\n';
			}
			if (arrayValue_.size() != 0)
				rv.pop_back();
			return rv;
		case ObjectType::OBJECT:
			if (value)
				rv = "<object>:\n";
			else
				rv = tabs + "<object>:\n";

			tabs += '\t';
			for (auto item : objectValue_) {
				rv += tabs + "[key]=\"" + item.first + "\": [value]=" + item.second._debugString(nTabs + 1, true) + '\n';
			}
			if (objectValue_.size() != 0)
				rv.pop_back();
			return rv;
		default:
			return "error";
		}
	}
#endif

	void Scanner::error(const std::string& msg) {
		std::cout << "Error occur at (" << line_ << ", " << column_ << "), msg: " << std::endl;
		std::cout << msg << std::endl;
		exit(1);
	}

	Scanner::Scanner(const std::string& file) :line_(-1), column_(-1) {
		input_.open(file);
		if (input_.fail()) {
			error("Can't open file: <" + file + ">.");
		}
		else {
			line_ = 1;
			column_ = 0;
		}
	}
	char Scanner::getNextChar() {
		currentChar_ = input_.get();
		if (currentChar_ == '\n') {
			line_++;
			column_ = 0;
		}
		else {
			column_++;
		}
		return currentChar_;
	}
	char Scanner::peekChar() {
		return input_.peek();
	}

	void Scanner::SkipSpace() {
		while (std::isspace(peekChar())) {
			getNextChar();
		}
	}

	JsonObject Scanner::handleValue() {
		char ch = peekChar();
		if (!ch) error("Error in handling value. Expect a char but EOF.");
		if (ch == '\"') return handleString();
		if (ch == '-' || std::isdigit(ch)) return handleNumber();
		if (ch == '[') return handleArray();
		if (ch == '{') return handleObject();
		std::string tmp;
		for (int i = 0; i < 4; i++) tmp.push_back(getNextChar());
		if (tmp == "null") return JsonObject();
		if (tmp == "true") return JsonObject(true);
		tmp.push_back(getNextChar());
		if (tmp == "false") return JsonObject(false);
		error("Error in handling value. Nothing can be matched.");
	}

	JsonObject Scanner::handleString() {
		if (peekChar() != '\"') {
			error("Error in handling string. Expect '\"' but not.");
		}
		getNextChar();
		std::string rv;
		while (peekChar() != '\"' && peekChar()) {
			char ch = getNextChar();
			if (ch == '\\') {
				getNextChar();
				switch (currentChar_) {
				case 'b': rv.push_back('\b'); break;
				case 'f': rv.push_back('\f'); break;
				case 'n': rv.push_back('\n'); break;
				case 'r': rv.push_back('\r'); break;
				case 't': rv.push_back('\t'); break;
				default:
					rv.push_back(currentChar_);
				}
			}
			else {
				rv.push_back(ch);
			}
		}
		if (peekChar() == '\"') {
			getNextChar();
			return JsonObject(rv);
		}
		else {
			error("Error in handling string. Expect '\"' but EOF found.");
		}
	}

	JsonObject Scanner::handleNumber() {
		int sign = 1, nowInt = 0, len = 0;
		bool real = false;
		double realNum;
		if (peekChar() == '-') {
			sign = -1;
			getNextChar();
		}
		while (std::isdigit(peekChar())) {
			nowInt *= 10;
			nowInt += getNextChar() - '0';
		}
		if (peekChar() == '.') {
			getNextChar();
			real = true;
			realNum = nowInt;
			while (std::isdigit(peekChar())) {
				realNum *= 10;
				realNum += getNextChar() - '0';
				len--;
			}
			realNum = std::pow(realNum, len);
		}
		if (peekChar() == 'E' || peekChar() == 'e') {
			getNextChar();
			int signTmp = 1, tmp = 0;
			if (peekChar() == '-') {
				signTmp = -1;
				getNextChar();
			}
			while (std::isdigit(peekChar())) {
				tmp *= 10;
				tmp += getNextChar() - '0';
			}
			realNum = std::pow(realNum, signTmp * tmp);
		}
		if (real) {
			double tmp = nowInt;
			tmp += std::pow(realNum * sign, len);
			return JsonObject(tmp);
		}
		else {
			return JsonObject(sign * static_cast<int>(nowInt * std::pow(10, len)));
		}
	}

	JsonObject Scanner::handleArray() {
		getNextChar();
		if (currentChar_ != '[') {
			error("Error in handling array. Expect '[' but not.");
		}
		std::vector<JsonObject> rv;
		if (peekChar() == ']') {
			getNextChar();
			return JsonObject(rv);
		}

		SkipSpace();
		rv.push_back(handleValue());

		SkipSpace();
		while (peekChar() == ',') {
			getNextChar();
			SkipSpace();
			rv.push_back(handleValue());
		}

		SkipSpace();
		if (peekChar() == ']') {
			getNextChar();
			return JsonObject(rv);
		}
		else {
			error("Error in handling array. Expect ']' but not.");
		}
	}

	JsonObject Scanner::handleObject() {
		getNextChar();
		if (currentChar_ != '{') {
			error("Error in handling object. Expect '{' but not.");
		}

		std::map<std::string, JsonObject> rv;

		SkipSpace();
		auto key = handleString().getString();
		if (rv.find(key) != rv.end())
			error("Error in handing object. Already has key \"" + key + "\".");
		SkipSpace();
		getNextChar();
		if (currentChar_ != ':') {
			error("Error in handing object. Expect ':' but not.");
		}
		SkipSpace();
		auto value = handleValue();
		SkipSpace();
		rv.insert(std::make_pair(key, value));

		while (peekChar() == ',') {
			getNextChar();
			SkipSpace();
			auto key = handleString().getString();
			if (rv.find(key) != rv.end())
				error("Error in handing object. Already has key \"" + key + "\".");
			SkipSpace();
			getNextChar();
			if (currentChar_ != ':') {
				error("Error in handing object. Expect ':' but not.");
			}
			SkipSpace();
			auto value = handleValue();
			rv.insert(std::make_pair(key, value));
		}
		SkipSpace();
		if (peekChar() == '}'){
			getNextChar();
			return JsonObject(rv);
		}
		else
			error("Error in handling object. Expect '}' but not.");
	}

	JsonObject Scanner::load() {
		SkipSpace();
		return handleValue();
	}
}