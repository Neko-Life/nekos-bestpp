#include <cstdio>
#include <sstream>
#include <string>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>

#include "nlohmann/json.hpp"
#include "nekos-best++.hpp"

namespace nekos_best {
	enum _req_flag {
		NONE = 0,
		NOBODY = 1,
	};

	void _fill_meta(Meta& meta, nlohmann::json& json) {
		meta.status_code = 0;
		meta.artist_href = json.value("artist_href", "");
		meta.artist_name = json.value("artist_name", "");
		meta.source_url = json.value("source_url", "");
		meta.url = json.value("url", "");
		meta.anime_name = json.value("anime_name", "");
	}

	// internal use, returns status code
	long _request(const std::string& req_url, std::string& res, nlohmann::json *header = NULL, _req_flag _flag = _req_flag::NONE)
	{
		using namespace curlpp;

		std::ostringstream result_stream;
		Cleanup clean_up;

		Easy request;

		// set header
		request.setOpt(Options::Header("user-agent: nekos.best++"));
		request.setOpt(Options::WriteStream(&result_stream));

		if (_flag & _req_flag::NOBODY) {
			request.setOpt(Options::NoBody(1));
		}

		request.setOpt(Options::Url(req_url));

		// perform the request
		request.perform();

		const auto res_code = Infos::ResponseCode::get(request);

		if (res_code != 200L) {
			fprintf(stderr, "[nekos-best++ WARN] Status code: %ld\n", res_code);
		}

		const auto header_size = Infos::HeaderSize::get(request);
		const std::string res_str = result_stream.str();
		if (header != NULL) {
			const std::string head = res_str.substr(0, header_size);
			// printf("HEADER:\n%s\n", head.c_str());

			std::istringstream str_stream(head);
			for (std::string line; std::getline(str_stream, line); ) {
				if (header->is_null()) {
					size_t space = line.find(" ");

					(*header)["protocol"] = line.substr(0, space);
					(*header)["status"] = line.substr(space + 1);

					continue;
				}

				// 				turns out the url is not in response header since they're aware we have it already
				for (const auto& key : { "artist_href", "artist_name", "source_url", /*"url",*/ "anime_name" }) {
					const std::string search = std::string(key) + ": ";
					const size_t pos = line.find(search);

					if (pos != std::string::npos) {
						const std::string sub = line.substr(pos + search.length());
						// printf("SUB %s: %s\n", key, sub.c_str());
						(*header)[key] = sub;
						break;
					}
				}
			}
		}

		res = res_str.substr(header_size);

		if (res_code != 200L) {
			fprintf(stderr, "[nekos-best++ WARN] Response JSON:\n%s\n", res.c_str());
		}

		return res_code;
	}

	QueryResult _parse_query_result(const std::string& res, long status_code) {
		QueryResult result;
		result.results = {};

		if (!res.length()) {
			fprintf(stderr, "[nekos-best++ ERROR] Request has no result\n");
			return result;
		}
		
		nlohmann::json json_res = nlohmann::json::parse(res);

		result.json_result = json_res;
		
		if (!json_res.is_object()) {
			fprintf(stderr, "[nekos-best++ ERROR] Unexpected error, JSON Response is not an object:\n%s\n", json_res.dump(2).c_str());
			return result;
		}

		if (json_res.size()) {
			nlohmann::json array = json_res["results"];
			if (!array.is_array()) {
				fprintf(stderr, "[nekos-best++ ERROR] Unexpected error, 'results' is not array:\n%s\n", json_res.dump(2).c_str());
				return result;
			}

			if (!array.size()) {
				return result;
			}

			for (size_t i = 0; i < array.size(); i++) {
				nlohmann::json value = array.at(i);

				if (!value.is_object()) {
					fprintf(stderr, "[nekos-best++ ERROR] Unexpected error, 'value' is not object:\n%s\n", value.dump(2).c_str());
					continue;
				}

				if (!value.size()) continue;

				Meta data;
				_fill_meta(data, value);
				data.status_code = status_code;

				result.results.push_back(data);
			}
		}

		return result;
	}

	std::string get_base_url() {
		return BASE_URL + API_VERSION;
	}

	Meta fetch_single(const std::string& category, const std::string& filename, const std::string& format) {
		Meta data;

		const std::string req_url = get_base_url() + "/" + curlpp::escape(category) + "/" + curlpp::escape(filename) + "." + curlpp::escape(format);

		data.url = req_url;

		std::string res = "";
		nlohmann::json header;

		const long status_code = _request(req_url, res, &header, _req_flag::NOBODY);
		data.status_code = status_code;

		if (header.is_null()) {
			return data;
		}

		// printf("HEADER:\n%s\n", header.dump(2).c_str());

		const std::string artist_href_with_cr = curlpp::unescape(header.value("artist_href", ""));
		data.artist_href = artist_href_with_cr.substr(0, artist_href_with_cr.length() - 1);

		const std::string artist_name_with_cr = curlpp::unescape(header.value("artist_name", ""));
		data.artist_name = artist_name_with_cr.substr(0, artist_name_with_cr.length() - 1);

		const std::string source_url_with_cr = curlpp::unescape(header.value("source_url", ""));
		data.source_url = source_url_with_cr.substr(0, source_url_with_cr.length() - 1);

		// const std::string url_with_cr = curlpp::unescape(header.value("url", ""));
		// data.url = url_with_cr.substr(0, url_with_cr.length() - 1);

		const std::string anime_name_with_cr = curlpp::unescape(header.value("anime_name", ""));
		data.anime_name = anime_name_with_cr.substr(0, anime_name_with_cr.length() - 1);

		return data;
	}

	std::map<std::string, EndpointSpec> get_available_endpoint() {
		using return_type = std::map<std::string, EndpointSpec>;
		
		return_type endpoints = {};

		const std::string req_url = get_base_url() + "/endpoints";

		std::string res = "";

		// request
		const long status_code = _request(req_url, res);

		if (!res.length()) {
			fprintf(stderr, "[nekos-best++ ERROR] Request has no result\n");
			return endpoints;
		}
		
		nlohmann::json json_res = nlohmann::json::parse(res);
		
		if (!json_res.is_object()) {
			fprintf(stderr, "[nekos-best++ ERROR] Unexpected error, JSON Response is not an object:\n%s\n", json_res.dump(2).c_str());
			return endpoints;
		}

		if (json_res.size()) {
			for (auto spec_iterator = json_res.begin(); spec_iterator != json_res.end(); spec_iterator++) {
				nlohmann::json value = spec_iterator.value();

				if (!value.is_object()) {
					fprintf(stderr, "[nekos-best++ ERROR] Expected JSON object, got:\n%s\n", value.dump(2).c_str());
					continue;
				}

				if (!value.size()) continue;

				EndpointSpec spec;
				spec.json_result = value;
				spec.name = spec_iterator.key();
				spec.format = value.value("format", "");
				spec.min = value.value("min", "");
				spec.max = value.value("max", "");

				endpoints.insert(std::make_pair(spec.name, spec));
			}
		}

		return endpoints;
	}

	QueryResult fetch(const std::string& category, const int amount) {
		std::string req_url = get_base_url() + "/" + curlpp::escape(category);

		if (amount > 1) {
			req_url += "?amount=" + std::to_string(amount);
		}

		std::string res = "";

		// request
		const long status_code = _request(req_url, res);

		return _parse_query_result(res, status_code);
	}

	QueryResult search(const std::string& query, const int type, const std::string& category, const int amount) {
		std::string req_url = get_base_url() + "/search?query=" + curlpp::escape(query) + "&type=" + std::to_string(type);

		if (category.length()) {
			req_url += "&category=" + curlpp::escape(category);
		}

		if (amount > 1) {
			req_url += "&amount=" + std::to_string(amount);
		}

		std::string res = "";

		// request
		const long status_code = _request(req_url, res);

		return _parse_query_result(res, status_code);
	}
} // nekos_best
