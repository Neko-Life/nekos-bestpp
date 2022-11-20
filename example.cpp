#include <stdio.h>
#include "nekos-best++.hpp"

int main(const int argc, const char *argv[]) {	// nekos_best::fetch
	printf("%s\n", nekos_best::get_base_url().c_str());
	const std::map<std::string, nekos_best::EndpointSpec> res = nekos_best::get_available_endpoints();

	for (const auto& d : res) {
		printf("[RESULT] res:\n%s\n", d.second.json_result.dump(2).c_str());
		printf("KEY: \"%s\"\n", d.first.c_str());

		printf("FORMAT: \"%s\"\n", d.second.format.c_str());
		printf("MAX: \"%s\"\n", d.second.max.c_str());
		printf("MIN: \"%s\"\n", d.second.min.c_str());
		printf("NAME: \"%s\"\n", d.second.name.c_str());
		printf("\n");
	}

	const auto res2 = nekos_best::fetch("neko", 50);
	printf("[RESULT] res2:\n%s\n", res2.json_result.dump(2).c_str());

	for (const auto& r : res2.results) {
		printf("artist_href: \"%s\"\n", r.artist_href.c_str());
		printf("artist_name: \"%s\"\n", r.artist_name.c_str());
		printf("source_url: \"%s\"\n", r.source_url.c_str());
		printf("url: \"%s\"\n", r.url.c_str());
		printf("anime_name: \"%s\"\n", r.anime_name.c_str());
		printf("\n");
	}
	
	const auto res3 = nekos_best::fetch("hug", 50);
	printf("[RESULT] res3:\n%s\n", res3.json_result.dump(2).c_str());

	for (const auto& r : res3.results) {
		printf("artist_href: \"%s\"\n", r.artist_href.c_str());
		printf("artist_name: \"%s\"\n", r.artist_name.c_str());
		printf("source_url: \"%s\"\n", r.source_url.c_str());
		printf("url: \"%s\"\n", r.url.c_str());
		printf("anime_name: \"%s\"\n", r.anime_name.c_str());
		printf("\n");
	}
	{
		nekos_best::Meta meta = nekos_best::fetch_single("neko", "0154", "png");

		printf("artist_href: \"%s\"\n", meta.artist_href.c_str());
		printf("artist_name: \"%s\"\n", meta.artist_name.c_str());
		printf("source_url: \"%s\"\n", meta.source_url.c_str());
		printf("url: \"%s\"\n", meta.url.c_str());
		printf("anime_name: \"%s\"\n", meta.anime_name.c_str());
		printf("\n");
	}
	{
		nekos_best::Meta meta = nekos_best::fetch_single("hug", "035", "gif");

		printf("artist_href: \"%s\"\n", meta.artist_href.c_str());
		printf("artist_name: \"%s\"\n", meta.artist_name.c_str());
		printf("source_url: \"%s\"\n", meta.source_url.c_str());
		printf("url: \"%s\"\n", meta.url.c_str());
		printf("anime_name: \"%s\"\n", meta.anime_name.c_str());
		printf("\n");
	}
	{
		nekos_best::Meta meta = nekos_best::fetch_single("unknown", "0069", "webp");

		printf("artist_href: \"%s\"\n", meta.artist_href.c_str());
		printf("artist_name: \"%s\"\n", meta.artist_name.c_str());
		printf("source_url: \"%s\"\n", meta.source_url.c_str());
		printf("url: \"%s\"\n", meta.url.c_str());
		printf("anime_name: \"%s\"\n", meta.anime_name.c_str());
		printf("\n");
	}
	return 0;
}
