#include "logo_recognizer.h"

bool LogoRecognizer::recognizeLogo(cv::Mat& image) {
  if (image.empty())
    return false;

  tresholding(image, cv::Scalar(0, 0, 128), cv::Scalar(100, 100, 255));
  return true;
}


void LogoRecognizer::tresholding(cv::Mat& image, cv::Scalar low_bgr,
                                 cv::Scalar high_bgr) {
  cv::Mat_<cv::Vec3b> x = image;
  for (int i = 0; i < x.rows; i++) {
    for (int j = 0; j < x.cols; j++) {
      if (x(i, j)[0] >= low_bgr[0] && x(i, j)[0] <= high_bgr[0] &&
          x(i, j)[1] >= low_bgr[1] && x(i, j)[1] <= high_bgr[1] &&
          x(i, j)[2] >= low_bgr[2] && x(i, j)[2] <= high_bgr[2]) {
        x(i, j) = cv::Vec3i(255, 255, 255);
      } else {
        x(i, j) = cv::Vec3i(0, 0, 0);
      }
    }
  }
  image = x;
}
