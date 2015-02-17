#ifndef TWISTOY_TOYS_C3_MRO_H_
#define TWISTOY_TOYS_C3_MRO_H_

#include <vector>
#include <string>
#include <iostream>
#include <map>

namespace twistoy {
	typedef std::pair<std::string, std::vector<std::string>> Meta;
	typedef std::vector<std::string> StringList;

	std::map<std::string, Meta> environment;

	void newMeta(std::string name, std::vector<std::string> fathers) {
		environment[name] = std::make_pair(name, fathers);
	}

	std::vector<StringList> handleDelete(std::string str, std::vector<StringList> info) {
		std::vector<StringList> rv;
		for (auto sl : info) {
			StringList tmp; tmp.clear();
			for (auto s : sl) {
				if (s != str) {
					tmp.push_back(s);
				}
			}
			if (!tmp.empty())
				rv.push_back(tmp);
		}
		return rv;
	}

	StringList merge(std::vector<StringList> info) {
		StringList rv;
		if (info.empty()) {
			return rv;
		}
		if (info.size() == 1) {
			return info[0];
		}
		for (int i = 0; i < info.size(); i++) {
			auto head = info[i][0];
			bool flag = true;
			for (int j = 0; j < info.size(); j++) {
				if (j == i) continue;
				for (int k = 1; k < info[j].size(); k++) {
					auto str = info[j][k];
					if (str == head) {
						flag = false;
						break;
					}
				}
				if (!flag)
					break;
			}
			if (flag) {
				rv.push_back(head);
				for (auto p : merge(handleDelete(head, info))) {
					rv.push_back(p);
				} return rv;
			}
		}
		std::cout << "Error" << std::endl;
		exit(1);
	}
	
	StringList L(Meta C) {
		StringList rv = {C.first};
		std::vector<StringList> arg;
		for (auto str : C.second) {
			arg.push_back(L(environment[str]));
		}
		if (!C.second.empty()) {
			arg.push_back(C.second);
		}
		for (auto str : merge(arg)) {
			rv.push_back(str);
		}
		return rv;
	}
}

#endif
