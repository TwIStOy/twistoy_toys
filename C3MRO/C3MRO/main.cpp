#include "mro.h"

using namespace twistoy;

int main() {
	newMeta("O", {});
	newMeta("A", { "O" });
	newMeta("B", { "A" });
	newMeta("C", { "B" });
	newMeta("D", { "B" });
	newMeta("E", { "A" });
	newMeta("F", { "E" });
	newMeta("G", { "E" });
	newMeta("H", { "C", "D", "F", "G" });
	auto out = L(environment["H"]);
	for (auto str : out) {
		std::cout << str << std::endl;
	}
	system("pause");
	return 0;
}