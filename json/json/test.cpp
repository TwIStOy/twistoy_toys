#include "json.h"
#include <fstream>

int main() {
	FILE* stream;
	if (freopen_s(&stream, "test.out", "w", stdout) != 0) {
		return 100;
	}

	twistoy::Scanner test("test5");

	auto json = test.load();

	std::cout << json.debugString() << std::endl;

	return 0;
}