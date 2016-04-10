#include <iostream>
#include "utils.h"
using namespace std;
using namespace twistoy;

struct tmp {
	int a, b, c, d, e, f;
};

int main() {
	MaxType<int, long long, long, tmp>::type a;
	cout << typeid(a).name() << endl;
	cout << sizeof(decltype(a)) << endl;

	//cout << MaxInteger<1, 2, 3, 4>::value << endl;
	getchar();
	return 0;
}