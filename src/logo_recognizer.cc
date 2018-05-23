#include "logo_recognizer.h"

bool LogoRecognizer::recognizeLogo(cv::Mat& image) {
  if (image.empty())
    return false;

  changeContrast(image, 150);
  tresholding(image, cv::Scalar(0, 0, 128), cv::Scalar(128, 128, 255));
  return true;
}

void LogoRecognizer::changeBrightness(cv::Mat& image, int brightness) {
  cv::Mat_<cv::Vec3b> x = image;
  for (int i = 0; i < x.rows; i++) {
    for (int j = 0; j < x.cols; j++) {
      for (int k = 0; k < 3; k++) {
        int color = x(i, j)[k] + brightness;
        x(i, j)[k] = color > 255 ? 255 : color < 0 ? 0 : color;
      }
    }
  }
  image = x;
}

/** Algorithm come from
 * http://www.dfstudios.co.uk/articles/programming/image-programming-algorithms/
 * image-processing-algorithms-part-5-contrast-adjustment/ webpage
 */
void LogoRecognizer::changeContrast(cv::Mat& image, int contrast) {
  float contrastFactor = (259 * (contrast + 255)) / (255 * (259 - contrast));
  cv::Mat_<cv::Vec3b> x = image;
  for (int i = 0; i < x.rows; i++) {
    for (int j = 0; j < x.cols; j++) {
      for (int k = 0; k < 3; k++) {
        int color = contrastFactor * (x(i, j)[k] - 128) + 128;
        x(i, j)[k] = color > 255 ? 255 : color < 0 ? 0 : color;
      }
    }
  }
  image = x;
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