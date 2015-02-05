#ifndef TWISTOY_JSON_H_
#define TWISTOY_JSON_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

#define DEBUG

namespace twistoy {
	enum ObjectType {
		NUMBER, BOOLEAN, NONE, STRING, ARRAY, OBJECT
	};
	class JsonObject {
	public:
		JsonObject();											// default
		JsonObject(int);										// number integer, realNumber_ = false; eg: 1, 2, 3
		JsonObject(double);										// number real, realNumber_ = true; eg: 1.1, 1.2, 1e10
		JsonObject(bool);										// true, false
		JsonObject(const std::string&);							// "hello world"
		JsonObject(const std::vector<JsonObject>&);				// []
		JsonObject(const std::map<std::string, JsonObject>&);	// {}

#ifdef DEBUG
		std::string debugString() const;
		std::string _debugString(int, bool) const;
#endif
		std::string getString() const;
	private:
		std::string toString() const;
		int intValue_;
		double realValue_;
		bool realNumber_;
		bool booleanValue_;
		std::string strValue_;
		std::vector<JsonObject> arrayValue_;
		std::map<std::string, JsonObject> objectValue_;
		ObjectType type_;
	};
	class Scanner {
	public:
		Scanner(const std::string&);
		JsonObject load();
	private:
		char currentChar_;
		std::ifstream input_;
		int line_;
		int column_;

		char peekChar();
		char getNextChar();

		JsonObject handleValue();
		JsonObject handleString();
		JsonObject handleNumber();
		JsonObject handleArray();
		JsonObject handleObject();
		void SkipSpace();

		void error(const std::string&);
	};
}

#endif