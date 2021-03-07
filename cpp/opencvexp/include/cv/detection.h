#ifndef GOS_EXP_CV_DETECTION_H_
#define GOS_EXP_CV_DETECTION_H_

#include <string>

#include <opencv2/objdetect.hpp>

namespace gos {
namespace cv {
namespace detection {

enum class method { undefined, a, b, unknown };

struct drawing {
  ::cv::Scalar color;
  int thickness;
  int line;
  int shift;
};

struct classification {
  drawing drawing;
  ::std::string file;
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
  method method;
  double scale;
  int timer;
};

void initialize(context& context);

int detecta(::cv::Mat& frame, context& context);
int detectb(::cv::Mat& image, context& context);

} // namespace detection
} // namespace cv
} // namespace gos

#endif
