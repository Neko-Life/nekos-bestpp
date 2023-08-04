all: ex example example-search

ex: ex.cpp
	g++ -std=c++17 -g -Iinclude src/nekos-best++.cpp ex.cpp -lcurlpp -lcurl -o ex

example: example.cpp
	g++ -std=c++17 -g -Iinclude src/nekos-best++.cpp example.cpp -lcurlpp -lcurl -o example

example-search: example-search.cpp
	g++ -std=c++17 -g -Iinclude src/nekos-best++.cpp example-search.cpp -lcurlpp -lcurl -o example-search

clean:
	rm ex example example-search
