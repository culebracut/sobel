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

  int blur_mask_width = 9;
  int blur_mask_height = 9;

  /* rectangle for cropped objects */
  cv::Rect crop_rect;
  int x = 600;
  int y = 400;
  int width = 600;
  int height = 400;
  crop_rect = cv::Rect (x, y, width, height);

  // first blur
  GaussianBlur(image(crop_rect), image(crop_rect), Size(blur_mask_width, blur_mask_height), 0, 0, BORDER_DEFAULT);
  rectangle(image,crop_rect,Scalar(255,0,0),1,8,0);
  //GaussianBlur(image, image, Size(15, 15), 0, 0, BORDER_DEFAULT);

  // Convert grayscale
  //cvtColor(image, src_gray, COLOR_BGR2GRAY);

  // Sobel on x,y
  Sobel(image, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
  Sobel(image, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);

  // converting back to CV_8U
  convertScaleAbs(grad_x, abs_grad_x);
  convertScaleAbs(grad_y, abs_grad_y);
  
  // weight for axis
  addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, image);

  //image = result;
  
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
  VideoCapture cap("file:////usr/src/jetson_multimedia_api/data/Video/sample_outdoor_car_1080p_10fps.h264"); 

  for (;;)
  {
    // grab a frame
    cap >> image;

    // apply Gaussian blur and Sobel edge detector on ROI
    ssobel(image, ksize, scale, delta, ddepth);

    //
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

  cap.release();

  return EXIT_SUCCESS;
}