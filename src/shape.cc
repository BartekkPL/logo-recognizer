#include "shape.h"

#include <cmath>

double Shape::calcRawMoment(int p, int q) {
  double moment = 0;
  for (auto& point : points) {
    moment += pow(point.y + 1, p) * pow(point.x + 1, q);
  }
  return moment;
}

void Shape::calcParameters() {
  m01 = calcRawMoment(0, 1);
  m10 = calcRawMoment(1, 0);
  m02 = calcRawMoment(0, 2);
  m20 = calcRawMoment(2, 0);
  m03 = calcRawMoment(0, 3);
  m30 = calcRawMoment(3, 0);
  m11 = calcRawMoment(1, 1);
  m21 = calcRawMoment(2, 1);
  m12 = calcRawMoment(1, 2);

  x_centr = m10 / area;
  y_centr = m01 / area;

  M01 = m01 - (m01 / area) * area;
  M10 = m10 - (m10 / area) * area;
  M11 = m11 - m10 * m01 / area;
  M20 = m20 - m10 * m10 / area;
  M02 = m02 - m01 * m01 / area;
  M21 = m21 - 2 * m11 * x_centr - m20 * y_centr + 2 * m01 * x_centr * x_centr;
  M12 = m12 - 2 * m11 * y_centr - m02 * x_centr + 2 * m10 * y_centr * y_centr;
  M30 = m30 - 3 * m20 * x_centr + 2 * m10 * x_centr * x_centr;
  M03 = m03 - 3 * m02 * y_centr + 2 * m01 * y_centr * y_centr;

  // Most interesting parameters
  NM1 = (M20 + M02) / pow(area, 2);
  NM2 = ((M20 - M02) * (M20 - M02) + 4 * M11 * M11) / pow(area, 4);
  NM3 = ((M30 - 3 * M12) * (M30 - 3 * M12) + (3 * M21 - M03) * (3 * M21 - M03)) / pow(area, 5);
  NM7 = (M20 * M02 - M11 * M11) / pow(area, 4);
}

bool Shape::isSquare() {
  // if (NM1 >= 0.12 && NM1 < 0.21 && NM2 >= 0.0006 && NM2 < 0.0016 && NM3 >= 0.0 &&
  //     NM3 < 0.002 && NM7 >= 0.003 && NM7 < 0.009)
  //   return true;
  // return false;
  return true;
}
