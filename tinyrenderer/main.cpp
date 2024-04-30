#include "tgaimage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include "geometry.h"

using namespace std;

const float camera_dist = 0.5f;


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void texturing(vec3 v0, vec3 pos, TGAImage &img, TGAImage &tex, TGAColor col){

	// int x0 = (v0.x+1) * 512;
	// int y0 = (v0.y+1) * 512;

	// multiply by the width-height of the texture image and you will get the color to put in your render.
	// 1024 * 1024

	TGAColor loc = tex.get(v0.x, v0.y);
	// cout << loc.val%255 <<"\n";

	// std::cout << loc.r <<"\n";
	// std::cout << loc.g <<"\n";
	// std::cout << loc.b <<"\n";

	img.set(pos.x, pos.y, loc);

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

void triangle(vec3 v0, vec3 v1, vec3 v2, vec3 v0_t, vec3 v1_t, vec3 v2_t, TGAImage &img, TGAImage &tex, TGAColor col, float *buffer){

	if (v0.y > v1.y) std::swap(v0, v1);
	if (v0.y > v2.y) std::swap(v0, v2);
	if (v1.y > v2.y) std::swap(v1, v2);

	if (v0_t.y > v1_t.y) std::swap(v0_t, v1_t);
	if (v0_t.y > v2_t.y) std::swap(v0_t, v2_t);
	if (v1_t.y > v2_t.y) std::swap(v1_t, v2_t);

	int x0 = (v0.x+1) * 400;
	int y0 = (v0.y+1) * 400;
	int x1 = (v1.x+1) * 400;
	int y1 = (v1.y+1) * 400;
	int x2 = (v2.x+1) * 400;
	int y2 = (v2.y+1) * 400;

	int x0_t = (v0_t.x) * 1024;
	int y0_t = (v0_t.y) * 1024;
	int x1_t = (v1_t.x) * 1024;
	int y1_t = (v1_t.y) * 1024;
	int x2_t = (v2_t.x) * 1024;
	int y2_t = (v2_t.y) * 1024;

	// cout << x0_t << " " << x1_t << " " << x2_t << " " << y0_t << " " << y1_t <<  " " << y2_t << "\n";


	// cout << x0_t << " " << y0_t << "\n";


	int z0 = (v0.z+1) * 400;
	int z1 = (v1.z+1) * 400;
	int z2 = (v2.z+1) * 400;


	int height = y2 - y0;
	int height_t = y2_t - y0_t;


	float z = 0;

	int i2 = y0_t;

	for(int i = y0; i<=y1; i++){
		i2 = i2 + 1;
		int s_height = y1-y0+1;
		int s_height_t = y1_t-y0_t+1;
		float a = (float)(i-y0)/height;
		float a_t = (float)(i2-y0_t)/height_t;
		float b = (float)(i-y0)/s_height;
		float b_t = (float)(i2-y0_t)/s_height;
		int A = x0 + (x2-x0)*a;
		int B = x0 + (x1-x0)*b;
		int A_t = x0_t + (x2_t-x0_t)*a_t;
		// cout  << A_t << "\n";
		int B_t = x0_t + (x1_t-x0_t)*b_t;
		if (B_t < 0){
			cout << x0_t << " " << x1_t << " " << x2_t << " " << y0_t << " " << y1_t <<  " " << y2_t << "\n";
			cout << x0_t << " " << y1_t << " " << y0_t << " " << s_height << " " << s_height_t << "\n";

		}
		//std::cout << "1\n";
		if (A > B) std::swap(A, B);
		if (A_t > B_t) std::swap(A_t, B_t);
		z = z0 + z1 + z2;
		int k2 = A_t;
		for (int k = A; k<=B; k++){
			k2 = k2 + 1;
			if(z > buffer[k+i*800]){
				// cout << "hi" << "\n";
				// cout << k2 <<"\n";
			//	std::cout << a << " " << b << "\n";
				buffer[k+i*800] = z;
				vec3 vector, pos;
				vector.x = k2 ;
				vector.y = i2;
				pos.x = k;
				pos.y = i;

				// cout <<  pos.x << " " << pos.y << "\n";
				texturing(vector, pos, img, tex, col);
				// img.set(k, i, col);
			}
		}
	}

	i2 = y1_t;

	for(int i = y1; i<=y2; i++){
		i2 = i2 + 1;
		int s_height = y2-y1+1;
		int s_height_t = y2_t-y1_t+1;
		float a = (float)(i-y0)/height;
		float a_t = (float)(i2-y0_t)/height_t;
		float b = (float)(i-y1)/s_height;
		float b_t = (float)(i2-y1_t)/s_height;
		int A = x0 + (x2-x0)*a;
		int B = x1 + (x2-x1)*b;
		int A_t = x0_t + (x2_t-x0_t)*a_t;
		int B_t = x1_t + (x2_t-x1_t)*b_t;
		//std::cout << "1\n";
		if (A > B) std::swap(A, B);
		if (A_t > B_t) std::swap(A_t, B_t);
		z = z0 + z1 + z2;
		int k2 = A_t;
		for (int k = A; k<=B; k++){
			k2 = k2 + 1;
			if(z > buffer[k+i*800]){
				//std::cout << B << " " << k << "\n";
				buffer[k+i*800] = z;
				vec3 vector, pos;
				vector.x = k2;
				vector.y = i2; 
				pos.x = k;
				pos.y = i;
				texturing(vector, pos, img, tex, col);
				// img.set(k, i, col);
			}
		}
	}

	// cout << "hi2" << "\n";
	
	//line(x0, x1, y0, y1, img, white);
	//line(x1, x2, y1, y2, img, white);
	//line(x2, x0, y2, y0, img, white);
}

void flip_tex(TGAImage &tex){
	tex.flip_vertically();
}

int main(int argc, char** argv) {

	float c_dist = 1.0f;


	TGAImage image(800, 800, TGAImage::RGB);
	TGAImage tex(1024, 1024, TGAImage::RGB);
	bool read = tex.read_tga_file("african_head_diffuse.tga");
	flip_tex(tex);
	// tex.write_tga_file("tex.tga");
	cout << "red " << tex.get(748, 855).r%255 << "\n";
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
	char b[2];
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
				iss >> b;
				iss >> x >> y >> z;
				// cout << b << " " << x << y << z << "\n";
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
					// cout << e << "\n";
				}
			f_vertex.push_back(face);
            tex_vertex.push_back(vert); // TODO: use this please :)
				// iss >> a; //int d, e, f;
				//push back more
		//		iss >> d  >> a >> e >> a >> f;
				// cout << d <<  " " << e <<  " " << f << "\n";
				// cout << e <<  "\n";

			
			}

		}

		file.close();
	}else cout << "couldn't open file\n";

	int g = 1;

	for(int i = 0; i<(int)f_vertex.size(); i++){
		vec3 w_coord[3];
		vec3 s_coord[3];
		vector<int> current_triangle = f_vertex[i];
		vector<int> current_tex_triangle = tex_vertex[i];
			vec3 v0 = vertex[current_triangle[0]-1];
			vec3 v1 = vertex[current_triangle[1]-1];
			vec3 v2 = vertex[current_triangle[2]-1];
			// Texturing pass should be done afterwards, not one to one on tex coords 
		vec3 v0_t = t_vertex[current_tex_triangle[0]-1];
		vec3 v1_t = t_vertex[current_tex_triangle[1]-1];
		vec3 v2_t = t_vertex[current_tex_triangle[2]-1];

		cout << v0.x << "\n";

		v0.z = v0.z / (1 - (v0.z/camera_dist));
		v1.z = v1.z / (1 - (v1.z/camera_dist));
		v2.z = v2.z / (1 - (v2.z/camera_dist));

		v0.x = v0.x / (1 - (v0.z/camera_dist));
		v1.x = v1.x / (1 - (v1.z/camera_dist));
		v2.x = v2.x / (1 - (v2.z/camera_dist));

		v0.y = v0.y / (1 - (v0.z/camera_dist));
		v1.y = v1.y / (1 - (v1.z/camera_dist));
		v2.y = v2.y / (1 - (v2.z/camera_dist));


		cout << v0 << "\n";

		// cout << current_triangle[0] << " " << current_tex_triangle[0] << " " << vertex[current_triangle[0]-1] << " " << t_vertex[current_tex_triangle[0]-1]<< "\n";

			w_coord[0] = v0_t;
			w_coord[1] = v1_t;
			w_coord[2] = v2_t;
	

	

		vec3 l = cross((w_coord[2]-w_coord[1]), (w_coord[1]-w_coord[0])); 
		l.normalized();
		float in = l*light_dir;
		// cout << v0_t << " " << v1_t << "\n";	
		//triangle(v0, v1, v2, image, TGAColor(rand()%255, rand()%255, rand()%255, 255), buffer);
		if(in > 0.){
		// triangle(v0, v1, v2, image, TGAColor((in*50*255), (50*in*255), (50*in*255), 255), buffer);
			triangle(v0, v1, v2, v0_t, v1_t, v2_t, image, tex, TGAColor((in*50*255), (50*in*255), (50*in*255), 255), buffer);
		//	triangle(v0, v1, v2, image, white, buffer);
		}
			
		// line((v0_t.x+1)*400, (v1_t.x+1)*400, (v0_t.y+1)*400, (v1_t.y+1)*400, image, white);
		// line((v1_t.x+1)*400, (v2_t.x+1)*400, (v1_t.y+1)*400, (v2_t.y+1)*400, image, white);
		// line((v2_t.x+1)*400, (v0_t.x+1)*400, (v2_t.y+1)*400, (v0_t.y+1)*400, image, white);
		

	}

	// for(int i = 0; i<(int)f_vertex.size(); i++){
	// 	vec3 w_coord[3];
	// 	vec3 s_coord[3];
	// 	vector<int> current_triangle = f_vertex[i];
	// 	vector<int> current_tex_triangle = tex_vertex[i];
	// 		vec3 v0 = vertex[current_triangle[0]-1];
	// 		vec3 v1 = vertex[current_triangle[1]-1];
	// 		vec3 v2 = vertex[current_triangle[2]-1];
	
	// 	vec3 l = cross((w_coord[2]-w_coord[1]), (w_coord[1]-w_coord[0])); 
	// 	l.normalized();
	// 	float in = l*light_dir;
	// 	// cout << v0_t << " " << v1_t << "\n";	
	// 	//triangle(v0, v1, v2, image, TGAColor(rand()%255, rand()%255, rand()%255, 255), buffer);
	// 	// if(in > 0.){
	// 	// // triangle(v0, v1, v2, image, TGAColor((in*50*255), (50*in*255), (50*in*255), 255), buffer);
	// 	// 	triangle(v0, v1, v2, v0_t, v1_t, v2_t, image, tex, TGAColor((in*50*255), (50*in*255), (50*in*255), 255), buffer);
	// 	// //	triangle(v0, v1, v2, image, white, buffer);
	// 	// }
			
	// 	line((v0.x+1)*400, (v1.x+1)*400, (v0.y+1)*400, (v1.y+1)*400, image, white);
	// 	line((v1.x+1)*400, (v2.x+1)*400, (v1.y+1)*400, (v2.y+1)*400, image, white);
	// 	line((v2.x+1)*400, (v0.x+1)*400, (v2.y+1)*400, (v0.y+1)*400, image, white);
		

	// }

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}


