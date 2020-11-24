/* Open CV headers */
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

struct _Sobel
{
  // OpenCV mat containing RGB data
  cv::Mat *cvmat;
}