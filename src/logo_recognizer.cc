#include "logo_recognizer.h"

#include <iostream>
#include <list>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

bool LogoRecognizer::recognizeLogo(cv::Mat& image) {
  std::vector<Shape> shapes;
  if (image.empty())
    return false;

  cv::Mat hls_image;
  cv::cvtColor(image, hls_image, CV_BGR2HLS);

  // changeContrastBGR(image, 150);
  cv::Mat red_image = image.clone();
  cv::Mat white_image = image.clone();
  // tresholdingBGR(red_image, cv::Scalar(0, 0, 128), cv::Scalar(128, 128, 255));
  // tresholdingBGR(white_image, cv::Scalar(200, 200, 200), cv::Scalar(255, 255, 255));
  tresholdingHLS(hls_image, cv::Scalar(170, 77, 77), cv::Scalar(179, 204, 255));
  //tresholdingHLS(white_image, cv::Scalar(200, 200, 200), cv::Scalar(255, 255, 255));
  cv::cvtColor(hls_image, image, CV_HLS2BGR);
  segmentation(image, shapes);
  return true;
}

void LogoRecognizer::tresholdingHLS(cv::Mat& image, cv::Scalar low_hls,
                                    cv::Scalar high_hls) {
  cv::Mat_<cv::Vec3b> x = image;
  for (int i = 0; i < x.rows; i++) {
    for (int j = 0; j < x.cols; j++) {
      if (x(i, j)[0] >= low_hls[0] && x(i, j)[0] <= high_hls[0] &&
          x(i, j)[1] >= low_hls[1] && x(i, j)[1] <= high_hls[1] &&
          x(i, j)[2] >= low_hls[2] && x(i, j)[2] <= high_hls[2]) {
        x(i, j) = cv::Vec3i(0, 255, 0);
      } else {
        x(i, j) = cv::Vec3i(0, 0, 0);
      }
    }
  }
  image = x;
}

void LogoRecognizer::changeBrightnessBGR(cv::Mat& image, int brightness) {
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
void LogoRecognizer::changeContrastBGR(cv::Mat& image, int contrast) {
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

void LogoRecognizer::tresholdingBGR(cv::Mat& image, cv::Scalar low_bgr,
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

void LogoRecognizer::segmentation(cv::Mat& image, std::vector<Shape>& shapes) {
  int n = 1;
  cv::Vec3b color;
  cv::Mat_<cv::Vec3b> x = image;
  for (int i = 0; i < image.rows; ++i) {
    for (int j = 0; j < image.cols; ++j) {
      std::cout << x(i, j) << "\n";
      if (x(i, j)[0] == 255 && x(i, j)[1] == 255 && x(i, j)[2] == 255 ) {
        cv::RNG rng(n);
        color = randomColor(rng);
        shapes.push_back(Shape(color));
        fillShape(image, i, j, color, shapes.back());
        n++;
      }
    }
  }
  filterLittleShapes();
}

cv::Vec3b LogoRecognizer::randomColor(cv::RNG& rng) {
  int icolor = (unsigned)rng;
  return cv::Vec3b(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}

void LogoRecognizer::fillShape(cv::Mat& image, int x, int y, cv::Vec3b color
    Shape& shape) {
  std::list<cv::Point2i> point_to_check;
  cv::Point2i actual, left, right, up, down;

  cv::Mat_<cv::Vec3b> _image = image;
  point_to_check.push_back(cv::Point2i(y, x));

  while (!point_to_check.empty()) {
    actual = point_to_check.front();
    point_to_check.pop_front();

    _image(actual) = color;
    shape.points.push_back(actual);

    std::cout << "Changed: " << _image(actual) << "\n";
    left = cv::Point2i(actual.x-1, actual.y);
    right = cv::Point2i(actual.x+1, actual.y);
    up = cv::Point2i(actual.x, actual.y-1);
    down = cv::Point2i(actual.x, actual.y+1);

    std::cout << "Point: " << actual << "\n";
    std::cout << "Left: " << _image(left) << "\n";
    std::cout << "Right: " << _image(right) << "\n";
    std::cout << "Up: " << _image(up) << "\n";
    std::cout << "Down: " << _image(down) << "\n";

    if (left.x >= 0 && _image(left)[0] == 255 &&
        _image(left)[1] == 255 &&
        _image(left)[2] == 255 &&
        std::find(point_to_check.begin(), point_to_check.end(), left) ==
        point_to_check.end()) {
      point_to_check.push_back(left);
    }
    if (right.x < _image.cols && _image(right)[0] == 255 &&
        _image(right)[1] == 255 &&
        _image(right)[2] == 255 &&
        std::find(point_to_check.begin(), point_to_check.end(), right) ==
        point_to_check.end()) {
      point_to_check.push_back(right);
    }
    if (up.y >= 0 && _image(up)[0] == 255 &&
        _image(up)[1] == 255 &&
        _image(up)[2] == 255 &&
        std::find(point_to_check.begin(), point_to_check.end(), up) ==
        point_to_check.end()) {
      point_to_check.push_back(up);
    }
    if (down.y < _image.rows && _image(down)[0] == 255 &&
        _image(down)[1] == 255 &&
        _image(down)[2] == 255 &&
        std::find(point_to_check.begin(), point_to_check.end(), down) ==
        point_to_check.end()) {
      point_to_check.push_back(down);
    }
  }

  image = _image;
}

void LogoRecognizer::filterLittleShapes() {

}
