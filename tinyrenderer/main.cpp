#include "tgaimage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include "geometry.h"

using namespace std;


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void texturing(vec3 v0, TGAImage &img, TGAImage &tex){

	// int x0 = (v0.x+1) * 512;
	// int y0 = (v0.y+1) * 512;

	// multiply by the width-height of the texture image and you will get the color to put in your render.
	// 1024 * 1024

	TGAColor loc = tex.get(v0.x+100, v0.y+100);

	// cout << loc.val <<"\n";

	// std::cout << loc.r <<"\n";
	// std::cout << loc.g <<"\n";
	// std::cout << loc.b <<"\n";

	img.set(v0.x, v0.y, loc);

}

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

	// std::cout << "drew line\n";
}

void triangle(vec3 v0, vec3 v1, vec3 v2, TGAImage &img, TGAImage &tex, TGAColor col, float *buffer){

	if (v0.y > v1.y) std::swap(v0, v1);
	if (v0.y > v2.y) std::swap(v0, v2);
	if (v1.y > v2.y) std::swap(v1, v2);

	int x0 = (v0.x+1) * 400;
	int y0 = (v0.y+1) * 400;
	int x1 = (v1.x+1) * 400;
	int y1 = (v1.y+1) * 400;
	int x2 = (v2.x+1) * 400;
	int y2 = (v2.y+1) * 400;

	int z0 = (v0.z+1) * 400;
	int z1 = (v1.z+1) * 400;
	int z2 = (v2.z+1) * 400;


	int height = y2 - y0;

	float z = 0;

	for(int i = y0; i<=y1; i++){
		int s_height = y1-y0+1;
		float a = (float)(i-y0)/height;
		float b = (float)(i-y0)/s_height;
		int A = x0 + (x2-x0)*a;
		int B = x0 + (x1-x0)*b;
		//std::cout << "1\n";
		if (A > B) std::swap(A, B);
		z = z0 + z1 + z2;
		for (int k = A; k<=B; k++){
			if(z > buffer[k+i*800]){
			//	std::cout << a << " " << b << "\n";
				buffer[k+i*800] = z;
				vec3 vector;
				vector.x = k;
				vector.y = i;
				texturing(vector, img, tex);
				// img.set(k, i, col);
			}
		}
	}

	for(int i = y1; i<=y2; i++){
		int s_height = y2-y1+1;
		float a = (float)(i-y0)/height;
		float b = (float)(i-y1)/s_height;
		int A = x0 + (x2-x0)*a;
		int B = x1 + (x2-x1)*b;
		//std::cout << "1\n";
		if (A > B) std::swap(A, B);
		z = z0 + z1 + z2;
		for (int k = A; k<=B; k++){
			if(z > buffer[k+i*800]){
				//std::cout << B << " " << k << "\n";
				buffer[k+i*800] = z;
				vec3 vector;
				vector.x = k;
				vector.y = i;
				texturing(vector, img, tex);
				// img.set(k, i, col);
			}
		}
	}
	
	//line(x0, x1, y0, y1, img, white);
	//line(x1, x2, y1, y2, img, white);
	//line(x2, x0, y2, y0, img, white);
}

int main(int argc, char** argv) {
	TGAImage image(800, 800, TGAImage::RGB);
	TGAImage tex;
	bool read = tex.read_tga_file("african_head_diffuse.tga");
	tex.flip_horizontally();
	tex.flip_vertically();
	if (!read){
		cout << "problem";
		return -1;
	}else{
		cout << tex.get(0, 100).r%255 << "\n";
		cout << tex.get(0, 100).g%255 << "\n";
	}
	image.set(52, 41, red);
	line(10, 11, 5, 10, image, white);
	float *buffer = new float[800*800];
	//opening obj & manipulating 
	
	ifstream file; string line_s;
        file.open("african_head.obj");
	char a;
	float x, y, z;
	int d, e, f;

	vector<vec3> vertex;
	vector<vec3> t_vertex;


	vector<vector<int>> f_vertex;
    vector<vector<int>> tex_vertex;

	vec3 light_dir;
	light_dir.x = 0; light_dir.y = 0; light_dir.z = -1;
	
	if (file.is_open()){
		cout << "opened file\n";
		while(!file.eof()){
			getline(file, line_s);
			std::istringstream iss(line_s.c_str()); 
			//cout << "test\n";
			//cout << a;
			if (line_s.compare(0, 2, "v ") == 0){
				iss >> a;
				iss >> x >> y >> z;
			//	cout << y << "\n"; 
			        vec3 vert;
				vert[0] = x;
				vert[1] = y;
				vert[2] = z;	
				image.set(x*64+48, y*64+32, red);		
				vertex.push_back(vert);
			}
			if (line_s.compare(0, 3, "vt ") == 0){
				iss >> a;
				iss >> x >> y >> z;
				vec3 vert;
				vert[0] = x;
				vert[1] = y;
				vert[2] = z;
				t_vertex.push_back(vert);
			}
		
			if (line_s.compare(0, 2, "f ") == 0){
				vector<int> face;
				vector<int> vert;
                iss >> a;
				while(iss >> d >> a >> e >> a >> f){
					face.push_back(d);
                    vert.push_back(e);
				//	cout << f_vertex.size() << "\n";
				}
			f_vertex.push_back(face);
            tex_vertex.push_back(vert);
		//		iss >> a; //int d, e, f;
				//push back more
		//		iss >> d  >> a >> e >> a >> f;
		//		cout << d <<  " " << e <<  " " << f << "\n";

			
			}

		}

		file.close();
	}else cout << "couldn't open file\n";

	for(int i = 0; i<(int)f_vertex.size(); i++){
		vec3 w_coord[3];
		vec3 s_coord[3];
		vector<int> current_triangle = f_vertex[i];
			vec3 v0 = vertex[current_triangle[0]-1];
			vec3 v1 = vertex[current_triangle[1]-1];
			vec3 v2 = vertex[current_triangle[2]-1];
			w_coord[0] = v0;
			w_coord[1] = v1;
			w_coord[2] = v2;
	

	

		vec3 l = cross((w_coord[2]-w_coord[1]), (w_coord[1]-w_coord[0])); 
		l.normalized();
		float in = l*light_dir;
		// cout << in << "\n";	
		//triangle(v0, v1, v2, image, TGAColor(rand()%255, rand()%255, rand()%255, 255), buffer);
		if(in > 0.){
		// triangle(v0, v1, v2, image, TGAColor((in*50*255), (50*in*255), (50*in*255), 255), buffer);
		triangle(v0, v1, v2, image, tex, TGAColor((in*50*255), (50*in*255), (50*in*255), 255), buffer);
		//	triangle(v0, v1, v2, image, white, buffer);
		}
			
		

	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}


