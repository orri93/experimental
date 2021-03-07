#include <vector>

#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

#include <cv/detection.h>

// #define GOS_CV_DEFAULT_FACE_CASCADE_FILE "C:\\lib\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalcatface.xml"
#define GOS_CV_DEFAULT_FACE_CASCADE_FILE "C:\\lib\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalface_alt.xml"
#define GOS_CV_DEFAULT_EYES_CASCADE_FILE "C:\\lib\\opencv\\build\\etc\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml"

namespace gos {
namespace cv {
namespace detection {

void initialize(context& context) {

  context.face.drawing.color = ::cv::Scalar(0, 0, 255);
  context.face.drawing.thickness = 1;
  context.face.drawing.line = 8;
  context.face.drawing.shift = 0;

  context.face.file = GOS_CV_DEFAULT_FACE_CASCADE_FILE;
  context.face.size = ::cv::Size(30, 30);
  context.face.factor = 1.1;
  context.face.minimum = 2;

  context.eyes.drawing.color = ::cv::Scalar(255, 0, 0);
  context.eyes.drawing.thickness = 1;
  context.eyes.drawing.line = 8;
  context.eyes.drawing.shift = 0;

  context.eyes.file = GOS_CV_DEFAULT_EYES_CASCADE_FILE;
  context.eyes.size = ::cv::Size(30, 30);
  context.eyes.factor = 1.1;
  context.eyes.minimum = 2;

  context.thresholds.minimum = 0.75;
  context.thresholds.maximum = 1.3;

  context.method = method::a;

  context.scale = 1.0;
  context.timer = 100;
}

int detecta(::cv::Mat& frame, context& context) {
  int radius;

  size_t fc, ec;
  ::cv::Mat gray, roi;

  ::cv::cvtColor(frame, gray, ::cv::COLOR_BGR2GRAY);
  ::cv::equalizeHist(gray, gray);

  std::vector<::cv::Rect> faces, eyes;

  context.face.classifier.detectMultiScale(gray, faces);

  fc = faces.size();
  for (size_t i = 0; i < fc; i++) {
    ::cv::Point center(
      faces[i].x + faces[i].width / 2,
      faces[i].y + faces[i].height / 2);
    ::cv::ellipse(
      frame,
      center,
      ::cv::Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360,
      context.face.drawing.color,
      context.face.drawing.thickness);

    roi = gray(faces[i]);
    context.eyes.classifier.detectMultiScale(roi, eyes);
    ec = eyes.size();
    for (size_t j = 0; j < ec; j++)
    {
      ::cv::Point eyecenter(
        faces[i].x + eyes[j].x + eyes[j].width / 2,
        faces[i].y + eyes[j].y + eyes[j].height / 2);
      radius = cvRound((
        ((double)(eyes[j].width)) + ((double)(eyes[j].height))) * 0.25);
      ::cv::circle(
        frame,
        eyecenter,
        radius,
        context.eyes.drawing.color,
        context.eyes.drawing.thickness);
    }
  }
  return static_cast<int>(faces.size());
}

int detectb(::cv::Mat& image, context& context) {
  double ratio;
  int radius;

  std::vector<::cv::Rect> faces, eyes;
  ::cv::Mat gray, small, roi;

  double fx = 1.0 / context.scale;

  // Convert to Gray scale
  ::cv::cvtColor(image, gray, ::cv::COLOR_BGR2GRAY);

  // Resize the Gray scale Image
  ::cv::resize(gray, small, ::cv::Size(), fx, fx, ::cv::INTER_LINEAR);
  ::cv::equalizeHist(small, small);

  // Detect faces of different sizes using cascade classifier  
  context.face.classifier.detectMultiScale(
    small,                          // Image
    faces,                          // Objects
    context.face.factor,            // Scale factor
    context.face.minimum,           // Minimum neighbors
    0 | ::cv::CASCADE_SCALE_IMAGE,  // Flags
    context.face.size);             // Minimum size

  // Draw circle or rectangle around the faces
  for (const ::cv::Rect& frect : faces) {
    ::cv::Point center;
    ratio = ((double)frect.width) / ((double)frect.height);
    if (ratio >= context.thresholds.minimum && ratio < context.thresholds.maximum) {
      center.x = cvRound((frect.x + frect.width * 0.5) * context.scale);
      center.y = cvRound((frect.y + frect.height * 0.5) * context.scale);
      radius = cvRound((((double)frect.width) + ((double)frect.height))
        * 0.25 * context.scale);
      ::cv::circle(
        image,
        center,
        radius,
        context.face.drawing.color,
        context.face.drawing.thickness,
        context.face.drawing.line,
        context.face.drawing.shift);
    } else {
      ::cv::rectangle(
        image,
        ::cv::Point(cvRound(frect.x * context.scale), cvRound(frect.y * context.scale)),
        ::cv::Point(
          cvRound((((double)frect.x) + ((double)frect.width) - 1.0) * context.scale),
          cvRound((((double)frect.y) + ((double)frect.height) - 1.0) * context.scale)),
        context.face.drawing.color,
        context.face.drawing.thickness,
        context.face.drawing.line,
        context.face.drawing.shift);
    }

    if (!context.eyes.classifier.empty()) {
      roi = small(frect);
      context.eyes.classifier.detectMultiScale(
        roi,                            // Image
        eyes,                           // Objects
        context.eyes.factor,            // Scale factor
        context.eyes.minimum,           // Minimum neighbors
        0 | ::cv::CASCADE_SCALE_IMAGE,  // Flags
        context.eyes.size);             // Minimum size

      // Draw circles around eyes
      for (const ::cv::Rect& erect : eyes) {
        center.x = cvRound((
          ((double)frect.x) +
          ((double)erect.x) +
          ((double)erect.width) * 0.5) * context.scale);
        center.y = cvRound((
          ((double)frect.y) +
          ((double)erect.y) +
          ((double)erect.height) * 0.5) * context.scale);
        radius = cvRound((
          ((double)erect.width) + ((double)erect.height)) * 0.25 * context.scale);
        ::cv::circle(
          image,
          center,
          radius,
          context.eyes.drawing.color,
          context.eyes.drawing.thickness,
          context.eyes.drawing.line,
          context.eyes.drawing.shift);
      }
    }
  }
  return static_cast<int>(faces.size());
}

} // namespace detection
} // namespace cv
} // namespace gos
