#include <iostream>
#include "tuple_operator.h"
#include "utils.h"
#include <string>
using namespace std;
using namespace twistoy;

template<int First, int... Rest>
void show(seq<First, Rest...>) {
	cout << First << endl;
	show(seq<Rest...>());
}
void show(seq<>) {
}

int main() {

	tuple<int, int, string, double> t{ 1, 2, "ok", 1.5 };

	//cout << find_index(t, 1) << endl;
	//cout << find_index(t, 1.5) << endl;
	//cout << find_index(t, 2) << endl;
	//cout << find_index(t, "ok") << endl;

	//cout << find_type<int, string, double, char, int>::value << endl;

	//cout << find_type_value<string>(t) << endl;

	//cout << typeid(static_range<10, -1>::type()).name() << endl;
	//show(static_range<10>::type());

	//cout << typeid(get_reverse_tuple(t)).name() << endl;
	traverse_tuple(get_reverse_tuple(t));

	getchar();
	return 0;
}