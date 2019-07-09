all : main.cpp kmeans.cpp Datapoint.cpp globals.cpp main's_functions.cpp kmeans_functions.cpp mylibLSH.a 
	g++ -std=c++11 -o cluster main.cpp kmeans.cpp Datapoint.cpp globals.cpp main\'s_functions.cpp kmeans_functions.cpp mylibLSH.a
