#ifndef SHAPE_H_
#define SHAPE_H_

#include <opencv2/core/core.hpp>

class Shape {
 public:
  Shape(cv::Scalar color) : color(color) {};
  Shape(cv::Mat image, int x, int y, cv::Scalar color) : color(color),
      x_geom(x), y_geom(y), image(image) {};
  void calcAll();

  cv::Scalar color;
  std::vector<cv::Point2i> points;
  int perimeter;
  int area;
  double W3;
  double M1;
  double M7;
  int x_geom;
  int y_geom;
  double x_centr;
  double y_centr;

 private:
  void calcPerimeterAndArea();
  void calcRawMoments();
  void calcCentralMoments();
  void calcParameters();

  cv::Mat image;

  // raw moments
  double m00;
  double m01;
  double m10;
  double m11;
  double m02;
  double m20;

  // central moments
  double M20;
  double M02;
  double M11;
};

#endif  // SHAPE_H
