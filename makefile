all: a b

a: A.cpp
	g++ -std=c++20 -o A A.cpp

b: B.cpp
	g++ -std=c++20 -lpthread -o B B.cpp

