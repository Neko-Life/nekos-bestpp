#include <stdlib.h>
#include <stdio.h>
#include "nekos-best++.hpp"

int main(const int argc, const char *argv[]) {	// nekos_best::fetch
	if (argc < 3) {
		printf("Usage: <search_query> [1|2] [category] [amount]");
		return 1;
	}

	const auto& res = nekos_best::search(argv[1], atoi(argv[2]), argc > 3 ? argv[3] : "", argc > 4 ? atoi(argv[4]) : 1);

	for (const auto& r : res.results) {
		printf("artist_href: \"%s\"\n", r.artist_href.c_str());
		printf("artist_name: \"%s\"\n", r.artist_name.c_str());
		printf("source_url: \"%s\"\n", r.source_url.c_str());
		printf("url: \"%s\"\n", r.url.c_str());
		printf("anime_name: \"%s\"\n", r.anime_name.c_str());
		printf("\n");
	}

	return 0;
}
