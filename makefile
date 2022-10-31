all: a b

a: A.cpp
	g++ -o A A.cpp

b: B.cpp
	g++ -lpthread -o B B.cpp

