#include "tgaimage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void line (int x0, int x1, int y0, int y1, TGAImage &img, TGAColor col){
	bool steep = false;
	if (std::abs(x0-x1)<std::abs(y0-y1)){
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;	
	}
	if (x0>x1){
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	for (int x = x0; x<x1; x++){
		float t = (x - x0) / (float) (x1 - x0);
		int y = y0 + (y1 - y0) * t;
		if (steep) img.set(y, x, col);
		else img.set(x, y, col);
	}

	std::cout << "drew line\n";
}

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);
	image.set(52, 41, red);
	line(10, 11, 5, 10, image, white);

	//opening obj & manipulating 
	
	ifstream file; string line;
        file.open("african_head.obj");
	char a;
	float x, y, z;

	if (file.is_open()){
		cout << "opened file\n";
		while(!file.eof()){
			getline(file, line);
			std::istringstream iss(line.c_str()); 
			//cout << "test\n";
			//cout << a;
			if (line.compare(0, 2, "v ") == 0){
				iss >> a;
				iss >> x >> y >> z;
				cout << y << "\n";  
				image.set(x*64+64, y*64+64, red);		
			}
		}
		file.close();
	}else cout << "couldn't open file\n";

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}


