#include <stdio.h>
#include "nekos-best++.hpp"

int main() {
	// init the client first to cache all available endpoints for it to be able to get random endpoint
	nekos_best::init();

	printf("base_url: %s\n", nekos_best::get_base_url().c_str());
	nekos_best::QueryResult result_struct = nekos_best::fetch("", 5);

	// check the response
	const nekos_best::Response res = nekos_best::get_last_request_response();

	printf("[RESPONSE] STATUS_CODE: %ld\n", res.status_code);
	printf("[RESPONSE] HEADERS:\n");

	for (const auto header : res.headers)
		printf("\"%s\": \"%s\"\n", header.first.c_str(), header.second.c_str());

	printf("\n");

	printf("[RESPONSE] JSON:\n%s\n\n", res.raw_json.dump(2).c_str());
	for (const nekos_best::Meta& data : result_struct.results) {
		printf("Got url: %s\n", data.url.c_str());
	}

	return 0;
}
