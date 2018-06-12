#include "logo_recognizer.h"

#include <iostream>
#include <list>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

// Main function that agregate functions to detect logo
bool LogoRecognizer::recognizeLogo(cv::Mat& image) {
  std::vector<Shape> shapes;
  if (image.empty())
    return false;

  // Convert to HLS colors
  cv::Mat hls_image;
  cv::cvtColor(image, hls_image, CV_BGR2HLS);
  tresholdingHLS(hls_image, cv::Scalar(170, 77, 77), cv::Scalar(199, 204, 255));
  // Convert back to BGR colors
  cv::cvtColor(hls_image, image, CV_HLS2BGR);
  segmentation(image, shapes);
  analysis(shapes);
  recognition(image, shapes);

  return true;
}

// If pixel color is in expected range than we change it to white, in other case
// it is changed to black
void LogoRecognizer::tresholdingHLS(cv::Mat& image, cv::Scalar low_hls,
                                    cv::Scalar high_hls) {
  cv::Mat_<cv::Vec3b> x = image;
  for (int i = 0; i < x.rows; i++) {
    for (int j = 0; j < x.cols; j++) {
      // We check two times H value, because red color is at the begining of
      // its range and at the end of it
      if (((x(i, j)[0] >= low_hls[0] && x(i, j)[0] <= high_hls[0]) ||
          (high_hls[0] >= 180 && x(i, j)[0] >= 0 &&
          x(i, j)[0] <= (int)high_hls[0] % 180)) &&
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

// Divide image into vector of shapes that is coloured to random color
// Small shapes all removed from image (coloured to black)
void LogoRecognizer::segmentation(cv::Mat& image, std::vector<Shape>& shapes) {
  int n = 1;
  cv::Vec3b color;
  cv::Mat_<cv::Vec3b> x = image;
  for (int i = 0; i < image.rows; ++i) {
    for (int j = 0; j < image.cols; ++j) {
      if (x(i, j)[0] == 255 && x(i, j)[1] == 255 && x(i, j)[2] == 255 ) {
        cv::RNG rng(n);
        color = randomColor(rng);
        shapes.push_back(Shape(color));
        fillShape(image, i, j, color, shapes.back());
        n++;
      }
    }
  }
  filterShapes(image, shapes);
}

cv::Vec3b LogoRecognizer::randomColor(cv::RNG& rng) {
  int icolor = (unsigned)rng;
  return cv::Vec3b(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}

// Fills shape with color and create vector of its points using flood fill method
void LogoRecognizer::fillShape(cv::Mat& image, int x, int y, cv::Vec3b color,
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

    left = cv::Point2i(actual.x-1, actual.y);
    right = cv::Point2i(actual.x+1, actual.y);
    up = cv::Point2i(actual.x, actual.y-1);
    down = cv::Point2i(actual.x, actual.y+1);

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

// Filters too small shapes
void LogoRecognizer::filterShapes(cv::Mat& image, std::vector<Shape>& shapes) {
  cv::Vec3b black(0, 0, 0);
  cv::Mat_<cv::Vec3b> x = image;
  for (auto shape = shapes.begin(); shape != shapes.end(); shape++) {
    shape->area = shape->points.size();
    if (image.cols > 550 && shape->area < 250/* || shape->area > 2000*/) {
      shape->color = black;
      for (auto point : shape->points) {
        x(point.y, point.x)[0] = 0;
        x(point.y, point.x)[1] = 0;
        x(point.y, point.x)[2] = 0;
      }
      shapes.erase(shape);
      shape--;
    }
  }
  image = x;
}

// Calculates needed parameters that will be used to comparision
void LogoRecognizer::analysis(std::vector<Shape>& shapes) {
  for (auto& shape : shapes)
    shape.calcParameters();
}

// Checks if some of shapes belongs to logo
void LogoRecognizer::recognition(cv::Mat& image, std::vector<Shape>& shapes) {
  std::vector<Shape> square, triangle;
  std::vector<Shape> logo;

  for (auto& shape : shapes) {
    if (shape.isSquare()) {
      square.push_back(shape);
    } else if (shape.isTriangle()) {
      triangle.push_back(shape);
    } else {
      for (auto& point : shape.points) {
        image.at<cv::Vec3b>(point.y, point.x) = cv::Vec3b(0, 0, 0);
      }
    }
  }
}
