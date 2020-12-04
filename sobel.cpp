#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

void ssobel (cv::Mat image, int ksize, int scale, int delta, int ddepth)
{
  cv::Mat src, src_gray;
  cv::Mat grad_x, grad_y;
  cv::Mat abs_grad_x, abs_grad_y;
  cv::Mat result;

  // first blur
  GaussianBlur(image, image, Size(13, 13), 0, 0, BORDER_DEFAULT);

  // Convert grayscale
  cvtColor(image, src_gray, COLOR_BGR2GRAY);

  // Sobel on x,y
  Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
  Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);

  // converting back to CV_8U
  convertScaleAbs(grad_x, abs_grad_x);
  convertScaleAbs(grad_y, abs_grad_y);
  
  // weight for axis
  addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, result);

  image = result;
  
  return;
}


int main( int argc, char** argv )
{
  cv::CommandLineParser parser(argc, argv,
                               "{@input   |/data/images/dog.jpg|input image}"
                               "{ksize   k|1|ksize (hit 'K' to increase its value at run time)}"
                               "{scale   s|1|scale (hit 'S' to increase its value at run time)}"
                               "{delta   d|0|delta (hit 'D' to increase its value at run time)}"
                               "{help    h|false|show help message}");
  cout << "The sample uses Sobel or Scharr OpenCV functions for edge detection\n\n";
  parser.printMessage();
  cout << "\nPress 'ESC' to exit program.\nPress 'R' to reset values ( ksize will be -1 equal to Scharr function )";
  // First we declare the variables we are going to use

  const String window_name = "Sobel Demo - Simple Edge Detector";
  String imageName = parser.get<String>("@input");
  int ksize = parser.get<int>("ksize");
  int scale = parser.get<int>("scale");
  int delta = parser.get<int>("delta");
  int ddepth = CV_16S;

  cv::Mat image;
  image = imread( samples::findFile( imageName ), IMREAD_COLOR ); // Load an image
  // Check if image is loaded fine
  if( image.empty() )
  {
    printf("Error opening image: %s\n", imageName.c_str());
    return EXIT_FAILURE;
  }

  for (;;)
  {
  image = imread( samples::findFile( imageName ), IMREAD_COLOR ); // Load an image
    // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
    ssobel(image, ksize, scale, delta, ddepth);

    imshow(window_name, image);
    char key = (char)waitKey(1);
    if(key == 27)
    {
      return EXIT_SUCCESS;
    }
    if (key == 'k' || key == 'K')
    {
      ksize = ksize < 30 ? ksize+2 : -1;
    }
    if (key == 's' || key == 'S')
    {
      scale++;
    }
    if (key == 'd' || key == 'D')
    {
      delta++;
    }
    if (key == 'r' || key == 'R')
    {
      scale =  1;
      ksize = -1;
      delta =  0;
    }
  }
  return EXIT_SUCCESS;
}