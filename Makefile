HM: main.cpp
	g++ -O3 main.cpp -o HM -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -ldl -lXxf86vm -lXinerama -lXcursor -lSOIL -lrt -lm -std=c++11

clean:
	rm HM
