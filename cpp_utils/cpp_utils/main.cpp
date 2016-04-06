#include <iostream>
#include "enumerate.h"
#include <vector>
#include <string>
#include <cstdio>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using twistoy::enumerate;
using std::printf;
using std::tie;
using std::sort;

int main() {
	vector<string> v = {
		"fuck", "shit", "hello", "world"
	};
	for (auto x : enumerate(v)) {
		int a;
		string str;
		tie(a, str) = x;
		printf("(%d, %s)\n", a, str.c_str());
	}
	getchar();

	return 0;
}
