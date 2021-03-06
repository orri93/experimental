#ifndef GOS_EXP_CV_DETECTION_H_
#define GOS_EXP_CV_DETECTION_H_

#include <opencv2/objdetect.hpp>

namespace gos {
namespace cv {
namespace detection {

struct drawing {
  ::cv::Scalar color;
  int thickness;
  int line;
  int shift;
};

struct classification {
  drawing drawing;
  ::cv::CascadeClassifier classifier;
  ::cv::Size size;
  double factor;
  int minimum;
};

struct thresholds {
  double minimum;
  double maximum;
};

struct context {
  classification face;
  classification eyes;
  thresholds thresholds;
  double scale;
};

void initialize(context& context);

void detecta(::cv::Mat& image, context& context);
void detectb(::cv::Mat& frame, context& context);

} // namespace detection
} // namespace cv
} // namespace gos

#endif
