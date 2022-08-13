#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

//This will be the max number of characters that your command line window can accept (highly dependent on each one's computer)
const int character_canvas = 100;
const bool invert_color = false;

int return_max(int a, int b, int c) {
	
	int max = NULL;

	if (a >= b && a >= c) {
		max = a;
	}
	if (b >= a && b >= c) {
		max = b;
	}
	if (c >= a && c >= b) {
		max = c;
	}

	return max;
}

int return_min(int a, int b, int c) {

	int min = NULL;

	if (a <= b && a <= c) {
		min = a;
	}
	if (b <= a && b <= c) {
		min = b;
	}
	if (c <= a && c <= b) {
		min = c;
	}

	return min;
}

//This function calculates the average color that pixels_in_chunk contains
//makes conversion of value into character notation and output it to command line. 
void convert_chunk(int pixels_in_chunk[][3], int size_of_chunk_x, int size_of_chunk_y, int current_pixel_x) {


	//The following sequence represents a grey scale from black -> white. Length of the array is 70
	//$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'. 
	string luminosity_characters[]{ "$","@","B","%","8","&","W","M","#","*","o","a","h","k","b","d","p","q","w","m","Z","O","0","Q","L","C","J","U","Y","X","z","c","v","u","n","x","r","j","f","t","/","\\","|","(",")","1","{","}","[","]","?","-","_","+","~","<",">","i","!","l","I",";",":",",","\"","^","`","\'","."," " };

	//total contains total R,G,B value respectively of each chunk
	int total[3]{ 0 };

	//number of pixels in the chunk
	int n_pixels_per_chunk = size_of_chunk_x * size_of_chunk_y;

	
	for (int component = 0; component < 3; component++) {

		for (int i = 0; i < n_pixels_per_chunk; i++) {
			total[component] += pixels_in_chunk[i][component];
		}

	}

	//this is the average pixel in the chunk
	int mean_pixel[] = { round(total[0] / n_pixels_per_chunk), round(total[1] / n_pixels_per_chunk), round(total[2] / n_pixels_per_chunk) };

	//cout << "(" << mean_pixel[0] << "," << mean_pixel[1] << "," << mean_pixel[2] << ")";

	//Conversion from mean_pixel to luminosity follows the following formula
	//gray_scale = (max(R,G,B)+min(R,G,B)) / 2

	int gray_scale = round((return_max(mean_pixel[0], mean_pixel[1], mean_pixel[2]) + return_min(mean_pixel[0], mean_pixel[1], mean_pixel[2])) / 2);

	//normalize gray_Scale (which goes from 0-255) to 0-69 basis (luminosity_characters is 70 long)
	gray_scale = round((float)gray_scale / (float)255 * (float)69);

	
	//printing each character to the console
	invert_color ? cout << luminosity_characters[gray_scale] : cout << luminosity_characters[69 - gray_scale];


}


int main(int argc, char* argv[]) {

	Mat img = imread(argv[1], IMREAD_COLOR);

	if (img.empty()) {
		cout << "Image " << argv[1] << "Could not be opened or found.";
		return 1;
	}

	//const int size_of_chunk_x = (int)round((float)img.cols / (float)character_canvas);  //The width (in pixels) of the chunk is determined by the number of character accepted in one line in the command window
	//const int size_of_chunk_y = (int)round((float)size_of_chunk_x*img.rows/img.cols); //Rule of three is calculated for the height of the chunk

	//const int number_of_chunks_x = round(img.cols / size_of_chunk_x);
	//const int number_of_chunks_y = round(img.rows / size_of_chunk_y);

	const int size_of_chunk_x = (int)floor((float)img.cols / (float)character_canvas);  //The width (in pixels) of the chunk is determined by the number of character accepted in one line in the command window
	const int size_of_chunk_y = (int)floor((float)(size_of_chunk_x * img.rows) / (float)img.cols); //Rule of three is calculated for the height of the chunk
	
	const int number_of_chunks_x = (int)floor((float)img.cols / (float)size_of_chunk_x);
	const int number_of_chunks_y = (int)floor((float)img.rows / (float)size_of_chunk_y);

	//we repeat this for every chunk value
	for (int i = 0; i < number_of_chunks_y; i++) {
		for (int j = 0; j < number_of_chunks_x; j++) {

			//index for pixels_in_chunk array
			int index = 0;

			//Array that contains all the pixels values of a chunk [how many pixels each chunk has in ][RGB has three components]
			int pixels_in_chunk[100][3] = { {0} };

			//Let's process each pixel value
			for (int y = 0 + ((int)(size_of_chunk_y * i)); y < (int)size_of_chunk_y+((int)size_of_chunk_y*i); y++) {
				for (int x = 0 + ((int)(size_of_chunk_x * j)); x < (int)size_of_chunk_x+((int)size_of_chunk_x*j); x++) {
					
					//cout << "pixel: " << x  << "," << y << " chunk: " << j << "," << i << endl;
	
					pixels_in_chunk[index][0] = img.at<Vec3b>(y, x)[0];   //RED
					pixels_in_chunk[index][1] = img.at<Vec3b>(y, x)[1];   //GREEN
					pixels_in_chunk[index][2] = img.at<Vec3b>(y, x)[2];   //BLUE

					index++;
				}
			}

			//cout << "Count= " << count++ << "/" << number_of_chunks_x * number_of_chunks_y << endl;
			convert_chunk(pixels_in_chunk, size_of_chunk_x, size_of_chunk_y, size_of_chunk_x*j);
			
		}
	}

	return 0;
}