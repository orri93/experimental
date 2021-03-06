#include <vector>

#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

#include <cv/detection.h>

namespace gos {
namespace cv {
namespace detection {

void initialize(context& context) {

  context.face.drawing.color = ::cv::Scalar(0, 0, 255);
  context.face.drawing.thickness = 1;
  context.face.drawing.line = 8;
  context.face.drawing.shift = 0;

  context.face.size = ::cv::Size(30, 30);
  context.face.factor = 1.1;
  context.face.minimum = 2;

  context.eyes.drawing.color = ::cv::Scalar(255, 0, 0);
  context.eyes.drawing.thickness = 1;
  context.eyes.drawing.line = 8;
  context.eyes.drawing.shift = 0;

  context.eyes.size = ::cv::Size(30, 30);
  context.eyes.factor = 1.1;
  context.eyes.minimum = 2;

  context.thresholds.minimum = 0.75;
  context.thresholds.maximum = 1.3;

  context.scale = 1.0;
}

void detecta(::cv::Mat& image, context& context) {
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
  for (size_t i = 0; i < faces.size(); i++) {
    ::cv::Scalar color = context.face.drawing.color; // Color for Drawing
    ::cv::Rect rect = faces.at(i);
    ::cv::Point center;
    ratio = ((double)rect.width) / ((double)rect.height);
    if (ratio >= context.thresholds.minimum && ratio < context.thresholds.maximum) {
      center.x = cvRound((rect.x + rect.width * 0.5) * context.scale);
      center.y = cvRound((rect.y + rect.height * 0.5) * context.scale);
      radius = cvRound((((double)rect.width) + ((double)rect.height))
        * 0.25 * context.scale);
      ::cv::circle(
        image,
        center,
        radius,
        color,
        context.face.drawing.thickness,
        context.face.drawing.line,
        context.face.drawing.shift);
    } else {
      ::cv::rectangle(
        image,
        ::cv::Point(cvRound(rect.x * context.scale), cvRound(rect.y * context.scale)),
        ::cv::Point(
          cvRound((((double)rect.x) + ((double)rect.width) - 1.0) * context.scale),
          cvRound((((double)rect.y) + ((double)rect.height) - 1.0) * context.scale)),
        color,
        context.face.drawing.thickness,
        context.face.drawing.line,
        context.face.drawing.shift);
    }

    if (!context.eyes.classifier.empty()) {
      roi = small(rect);
      context.eyes.classifier.detectMultiScale(
        roi,                            // Image
        eyes,                           // Objects
        context.eyes.factor,            // Scale factor
        context.eyes.minimum,           // Minimum neighbors
        0 | ::cv::CASCADE_SCALE_IMAGE,  // Flags
        context.eyes.size);             // Minimum size

      color = context.eyes.drawing.color; // Color for Drawing

      // Draw circles around eyes
      for (size_t j = 0; j < eyes.size(); j++) {
        ::cv::Rect eye = eyes.at(i);
        center.x = cvRound((
          ((double)rect.x) +
          ((double)eye.x) +
          ((double)eye.width) * 0.5) * context.scale);
        center.y = cvRound((
          ((double)rect.y) +
          ((double)eye.y) +
          ((double)eye.height) * 0.5) * context.scale);
        radius = cvRound((
          ((double)eye.width) + ((double)eye.height)) * 0.25 * context.scale);
        ::cv::circle(
          image,
          center,
          radius,
          color,
          context.eyes.drawing.thickness,
          context.eyes.drawing.line,
          context.eyes.drawing.shift);
      }
    }
  }
}

void detectb(::cv::Mat& frame, context& context) {
  int radius;

  ::cv::Mat gray, roi;

  ::cv::cvtColor(frame, gray, ::cv::COLOR_BGR2GRAY);
  ::cv::equalizeHist(gray, gray);

  std::vector<::cv::Rect> faces, eyes;

  context.face.classifier.detectMultiScale(gray, faces);

  for (size_t i = 0; i < faces.size(); i++) {
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
    for (size_t j = 0; j < eyes.size(); j++)
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
}

} // namespace detection
} // namespace cv
} // namespace gos
