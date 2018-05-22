#ifndef LOGO_RECOGNIZER_H
#define LOGO_RECOGNIZER_H

#include <opencv2/core/core.hpp>

/** Quite important notes
 * OpenCV has BGR Color Space, NOT RGB.
*/

class LogoRecognizer {
 public:
  LogoRecognizer() = default;
  ~LogoRecognizer() = default;
  bool recognizeLogo(cv::Mat& image);
 private:
  void tresholding(cv::Mat& image, cv::Scalar low_bgr, cv::Scalar high_bgr);
};

#endif  // LOGO_RECOGNIZER_H