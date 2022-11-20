#include <stdio.h>
#include "nekos-best++.hpp"

int main() {
	nekos_best::QueryResult result_struct = nekos_best::fetch("neko", 2);

	for (const nekos_best::Meta& data : result_struct.results) {
		printf("Got url: %s\n", data.url.c_str());
	}

	return 0;
}
