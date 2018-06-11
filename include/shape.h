#ifndef SHAPE_H_
#define SHAPE_H_

#include <opencv2/core/core.hpp>

class Shape {
 public:
  Shape(cv::Vec3b color) : color(color) {};
  void calcParameters();
  bool isSquare();
  bool isTriangle();

  cv::Vec3b color;
  std::vector<cv::Point2i> points;
  int area;

 private:
  double calcRawMoment(int p, int q);

  double x_centr;
  double y_centr;

  // raw moments
  double m01;
  double m10;
  double m02;
  double m20;
  double m03;
  double m30;
  double m11;
  double m21;
  double m12;

  // central moments
  double M01;
  double M10;
  double M11;
  double M20;
  double M02;
  double M21;
  double M12;
  double M30;
  double M03;
  double NM1;
  double NM2;
  double NM3;
  double NM7;
};

#endif  // SHAPE_H
