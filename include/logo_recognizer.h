#ifndef LOGO_RECOGNIZER_H
#define LOGO_RECOGNIZER_H

#include <vector>

#include <opencv2/core/core.hpp>

#include "shape.h"

/** Quite important notes
 * OpenCV has BGR Color Space, NOT RGB.
*/

class LogoRecognizer {
 public:
  LogoRecognizer() = default;
  ~LogoRecognizer() = default;
  bool recognizeLogo(cv::Mat& image);
 private:
  void tresholdingHLS(cv::Mat& image, cv::Scalar low_hls, cv::Scalar high_hls);
  void changeBrightnessBGR(cv::Mat& image, int brightness);
  void changeContrastBGR(cv::Mat& image, int contrast);
  void tresholdingBGR(cv::Mat& image, cv::Scalar low_bgr, cv::Scalar high_bgr);
  void segmentation(cv::Mat& image, std::vector<Shape>& shapes);
  cv::Vec3b randomColor(cv::RNG& rng);
  void fillShape(cv::Mat& image, int x, int y, cv::Vec3b color, Shape& shape);
  void filterShapes(cv::Mat& image, std::vector<Shape>& shapes);
  void analysis(std::vector<Shape>& shapes);
  void recognition(cv::Mat& image, std::vector<Shape>& shapes);
  bool isTriangle(Shape& s);
};

#endif  // LOGO_RECOGNIZER_H
