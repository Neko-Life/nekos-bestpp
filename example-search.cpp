#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <chrono>

#include "nekos-best++.hpp"

int main(const int argc, const char *argv[]) {	// nekos_best::fetch
	if (argc < 3) {
		printf("Usage: <search_query> [1|2] [category] [amount]");
		return 1;
	}

	// init the client first to cache all available endpoints for it to be able to get random endpoint
	nekos_best::init();

	const auto& res = nekos_best::search(argv[1], (nekos_best::image_format)atoi(argv[2]), argc > 3 ? argv[3] : "", argc > 4 ? atoi(argv[4]) : 1);

	for (const auto& r : res.results) {
		printf("artist_href: \"%s\"\n", r.artist_href.c_str());
		printf("artist_name: \"%s\"\n", r.artist_name.c_str());
		printf("source_url: \"%s\"\n", r.source_url.c_str());
		printf("url: \"%s\"\n", r.url.c_str());
		printf("anime_name: \"%s\"\n", r.anime_name.c_str());
		printf("\n");
	}

	const auto headers = nekos_best::get_last_request_response().headers;
	for (const auto data : headers) {
		printf("'%s': '%s'\n", data.first.c_str(), data.second.c_str());
	}

	int t_count = 0;

	for (int i = 0; i < 5; i++) {
		std::thread t([&t_count, i](){
					for (int j = 0; j < 5; j++) {
						printf("REQUEST %d %d\n", i+1, j+1);
						// lock mutex to prevent data races, but this is a rate limit test so we don't need it
						// std::lock_guard<std::mutex> lk(nekos_best::ns_mutex);
						nekos_best::search("neko", (nekos_best::image_format)1, "neko", 50);
						// const auto headers = nekos_best::get_last_request_response().headers;
						// for (const auto data : headers) {
						// 	printf("'%s': '%s'\n", data.first.c_str(), data.second.c_str());
						// }
						const bool r = nekos_best::is_rate_limited("neko");
						printf("Is rate limited: %d\n", r);
						if (r) printf("RATE LIMITED\n");

						// check rate limit info
						const auto info = nekos_best::get_rate_limit_info("neko");
						// if info exist
						if (info.endpoint.length()) {
							printf("ENDPOINT: '%s'\n", info.endpoint.c_str());

							char buff[128];

							std::strftime(buff, sizeof buff, "%A %c", &info.rate_limit_at);
							printf("RATE LIMITED AT: '%s'\n", buff);

							std::strftime(buff, sizeof buff, "%A %c", &info.rate_limit_reset_at);
							printf("RATE LIMIT RESET AT: '%s'\n", buff);

							std::tm current_time;
							time_t now;							
							time(&now);
							current_time = *std::gmtime(&now);
							std::strftime(buff, sizeof buff, "%A %c", &current_time);
							printf("REQUEST MADE AT: '%s'\n", buff);
						}
					}
					t_count--;
				});
		t_count++;
		t.detach();
	}

	std::thread t([&t_count](){
				std::this_thread::sleep_for(std::chrono::seconds(10));
				std::lock_guard<std::mutex> lk(nekos_best::ns_mutex);
				const bool r = nekos_best::is_rate_limited("neko");
				printf("Is rate limited: %d\n", r);
				if (r) printf("RATE LIMITED\n");
				t_count--;
			});
	t_count++;
	t.detach();

	while (t_count>0) {
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	return 0;
}
