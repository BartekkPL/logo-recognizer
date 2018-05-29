#include "shape.h"

#include <cmath>

void Shape::calcAll() {
  calcRawMoments();
  calcCentralMoments();
  calcPerimeterAndArea();
  calcParameters();
}

void Shape::calcPerimeterAndArea() {
  perimeter = 0;
  area = 0;
  cv::Mat_<cv::Vec3b> x = image;
  for (int i = 1; i < image.rows - 1; ++i) {
    for (int j = 1; j < image.cols - 1; ++j) {
      if (x(i, j)[0] == 0)
        area++;
      if (x(i, j)[0] == 0 && (x(i + 1, j)[0] != 0 ||
        x(i, j + 1)[0] != 0 || x(i - 1, j)[0] != 0 ||
        x(i, j - 1)[0] != 0 || x(i + 1, j + 1)[0] != 0 ||
        x(i - 1, j + 1)[0] != 0 || x(i - 1, j - 1)[0] != 0 ||
        x(i + 1, j - 1)[0] != 0))
        perimeter++;
    }
  }
}

void Shape::calcRawMoments() {
  m00 = m01 = m10 = m11 = m02 = m20 = 0;
  cv::Mat_<cv::Vec3b> x = image;
  for (int i = 1; i < image.rows - 1; ++i) {
    for (int j = 1; j < image.cols - 1; ++j) {
      if (x(i, j)[0] == 0) {
        m00 += 1;
        m01 += j;
        m10 += i;
        m11 += i * j;
        m02 += pow(j, 2);
        m20 += pow(i, 2);
      }
    }
  }
}

void Shape::calcCentralMoments() {
  M20 = m20 - pow(m10, 2) / m00;
  M02 = m02 - pow(m01, 2) / m00;
  M11 = m11 - m10 * m01 / m00;
}

void Shape::calcParameters() {
  W3 = (double)perimeter / ((double)2 * sqrt(((double)M_PI * (double)area))) - 1;
  M1 = (M20 + M02) / pow(m00, 2);
  M7 = (M20 * M02 - pow(M11, 2)) / pow(m00, 4);
}
