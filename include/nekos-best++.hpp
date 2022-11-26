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
		// response status
		long status_code;
		// response headers
		std::map<std::string, std::string> headers;
		// raw json response
		nlohmann::json raw_json;
	};

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
	 * @brief Metadata struct, for gif format usually anime_name and url are populated,
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
	 * @brief Fetch to a category endpoint
	 */
	QueryResult fetch(const std::string& category, const int amount = 1);

	/**
	 * @brief Fetch single image directly
	 * @param category
	 * @param filename image filename, variable digit of number as string
	 * @param format generally `png` or `gif`
	 */
	Meta fetch_single(const std::string& category, const std::string& filename, const std::string& format);

	/**
	 * @brief Search for image
	 * @param query search query
	 * @param type 1 for static image format, 2 for gif image format.
	 * 		Depending on category, it usually only accept one type.
	 * @param category image category
	 * @param amount result amount
	 */
	QueryResult search(const std::string& query, const int type, const std::string& category = "", const int amount = 1);
} // nekos_best

#endif // NEKOS_BEST_H
