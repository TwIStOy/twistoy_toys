#include "json.h"
#include <fstream>

int main() {
	FILE* stream;
	if (freopen_s(&stream, "test.out", "w", stdout) != 0) {
		return 100;
	}

	twistoy::Scanner test("test1");

	auto json = test.load();

	std::cout << json.debugString() << std::endl;

	std::cout << "====================" << std::endl;

	std::cout << json.dump(true) << std::endl;
	std::cout << json.get("glossary").dump() << std::endl;

	return 0;
}