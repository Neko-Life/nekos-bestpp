all:
	g++ -std=c++17 -Iinclude src/nekos-best++.cpp ex.cpp -lcurlpp -lcurl -o ex
	g++ -std=c++17 -Iinclude src/nekos-best++.cpp example.cpp -lcurlpp -lcurl -o example
	g++ -std=c++17 -Iinclude src/nekos-best++.cpp example-search.cpp -lcurlpp -lcurl -o example-search
