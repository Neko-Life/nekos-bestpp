#ifndef NEKOS_BEST_H
#define NEKOS_BEST_H

#include <string>
#include <vector>
#include <map>
#include "nlohmann/json.hpp"

namespace nekos_best {
	static const std::string 		API_VERSION 	= "v2"; 					// specify api version
	static const std::string 		BASE_URL 	= "https://nekos.best/api/"; 			// base url for request
	static const std::vector<std::string> 	PNG_CATEGORIES 	= { "husbando", "kitsune", "neko", "waifu" }; 	// list of category with png format
	static const std::vector<std::string> 	GIF_CATEGORIES 	= 
								{ 	// list of category with gif format
									"baka", "bite", "blush", "bored", "cry",
									"cuddle", "dance", "facepalm", "feed",
									"handhold", "happy", "highfive", "hug",
									"kick", "iss", "laugh", "pat",
									"poke", "pout", "punch", "shoot",
									"shrug", "slap", "sleep", "smile",
									"smug", "stare", "think", "thumbsup",
									"tickle", "wave", "wink", "yeet",
								};

	/**
	 * @brief Known and available image format
	 */
	enum image_format {
		// default value for function parameter
		if_none = 0,
		// PNG
		if_png = 1,
		// GIF
		if_gif = (1 << 1),
	};

	/**
	 * @brief Get API base url
	 */
	std::string get_base_url();

	/**
	 * @brief Response container
	 */
	struct Response {
		std::string protocol;
		
		// response status, you can use this to determine whether a request succeeded
		long status_code;
		
		// response headers, a key value pair
		std::map<std::string, std::string> headers;

		// raw json response, can contain error messages,
		// probably want to add a wrapper for that but no doc to be found.
		// It will be as it is for now.
		nlohmann::json raw_json;
	};

	// !TODO: error struct, get_error func

	/**
	 * @brief Endpoint specification struct
	 */
	struct EndpointSpec {
		std::string name;
		std::string min;
		std::string max;
		std::string format;
	};

	/**
	 * @brief Get API list of available endpoint
	 */
	std::map<std::string, EndpointSpec> get_available_endpoints();

	/**
	 * @brief Metadata struct, for gif format usually only anime_name and url are populated,
	 * 	whereas anime_name isn't populated if the image were a png format
	 */
	struct Meta {
		std::string artist_href;
		std::string artist_name;
		std::string source_url;
		std::string url;
		std::string anime_name;
	};

	/**
	 * @brief Query result struct
	 */
	struct QueryResult {
		std::vector<Meta> results;
	};

	/**
	 * @brief Get last API response, emptying the cache.
	 * 	You should copy it somewhere else if you were gonna keep it.
	 */
	Response get_last_request_response();

	/**
	 * @brief Fetch to a category endpoint
	 * @param category will pick one randomly if omitted
	 * @param amount default to 1
	 */
	QueryResult fetch(const std::string& category = "", const int amount = 1);

	/**
	 * @brief Fetch single image directly
	 * @param category will pick one randomly if omitted
	 * @param filename image filename, variable digit of number as string, will pick one randomly if omitted
	 * @param format will pick one randomly if omitted
	 */
	Meta fetch_single(const std::string& category = "", const std::string& filename = "", const image_format format = if_none);

	/**
	 * @brief Search for image
	 * @param query search query
	 * @param type will pick one randomly if omitted
	 * @param category image category, will pick randomly if omitted
	 * @param amount result amount, default to 1
	 */
	QueryResult search(const std::string& query,
			const image_format format = if_none,
			const std::string& category = "",
			const int amount = 1);
} // nekos_best

#endif // NEKOS_BEST_H
