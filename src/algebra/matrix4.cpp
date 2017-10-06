#include "altadore/algebra/matrix4.h"

#include <math.h>
#include <string.h>
#include "altadore/algebra/algebra_constants.h"
#include "altadore/algebra/point3.h"
#include "altadore/algebra/vector3.h"
#include "bonavista/base/macros.h"

Matrix4 Matrix4::GetRotation(Axis axis, double angle) {
  Matrix4 ret;
  double c = cos(angle * kPi/180);
  double s = sin(angle * kPi/180);

  switch (axis) {
    case AXIS_X:
      ret.m_[1][1] = c;
      ret.m_[1][2] = -s;
      ret.m_[2][1] = s;
      ret.m_[2][2] = c;
      break;
    case AXIS_Y:
      ret.m_[0][0] = c;
      ret.m_[0][2] = s;
      ret.m_[2][0] = -s;
      ret.m_[2][2] = c;
      break;
    case AXIS_Z:
      ret.m_[0][0] = c;
      ret.m_[0][1] = -s;
      ret.m_[1][0] = s;
      ret.m_[1][1] = c;
      break;
    default:
      DCHECK(false);
      break;
  }

  return ret;
}

Matrix4 Matrix4::GetScaling(double s) {
  return GetScaling(s, s, s);
}

Matrix4 Matrix4::GetScaling(double x, double y, double z) {
  Matrix4 ret;
  ret.m_[0][0] = x;
  ret.m_[1][1] = y;
  ret.m_[2][2] = z;
  return ret;
}

Matrix4 Matrix4::GetTranslation(double x, double y, double z) {
  Matrix4 ret;
  ret.m_[0][3] = x;
  ret.m_[1][3] = y;
  ret.m_[2][3] = z;
  return ret;
}

Matrix4::Matrix4() {
  memset(m_, 0, 16 * sizeof(double));

  m_[0][0] = 1.0;
  m_[1][1] = 1.0;
  m_[2][2] = 1.0;
  m_[3][3] = 1.0;
}

Matrix4::Matrix4(double d[4][4]) {
  memcpy(m_, d, 16 * sizeof(double));
}

Matrix4::~Matrix4() {
}

Matrix4::Matrix4(const Matrix4& m) {
  *this = m;
}

Matrix4& Matrix4::operator=(const Matrix4& m) {
  memcpy(m_, m.m_, 16 * sizeof(double));
  return *this;
}

Matrix4 Matrix4::GetInverse() const {
  Matrix4 m1 = *this;
  Matrix4 ret;
  double d;

  for (int c = 0; c < 4; ++c) {
    // Make diagonal value 1.
    d = m1.m_[c][c];
    if (d != 1.0) {
      for (int i = 0; i < 4; ++i) {
        m1.m_[c][i] /= d;
        ret.m_[c][i] /= d;
      }
    }

    // Row reduce.
    for (int r = 0; r < 4; ++r) {
      if (r != c) {
        d = m1.m_[r][c];
        if (d != 0.0) {
          for (int i = 0; i < 4; ++i) {
            m1.m_[r][i] -= d * m1.m_[c][i];
            ret.m_[r][i] -= d * ret.m_[c][i];
          }
        }
      }
    }
  }

  return ret;
}

Matrix4 Matrix4::GetTranspose() const {
  Matrix4 ret;
  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      ret.m_[r][c] = m_[c][r];
    }
  }

  return ret;
}

Matrix4 Matrix4::operator*(const Matrix4& m) const {
  Matrix4 ret;
  double sum;

  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      sum = 0.0;
      for (int i = 0; i < 4; ++i) {
        sum += m_[r][i] * m.m_[i][c];
      }
      ret.m_[r][c] = sum;
    }
  }

  return ret;
}

Point3 Matrix4::operator*(const Point3& p) const {
  Point3 ret;
  double sum;

  for (int r = 0; r < 3; ++r) {
    sum = 0.0;
    for (int c = 0; c < 3; ++c) {
      sum += m_[r][c] * p[c];
    }
    ret[r] = sum + m_[r][3];
  }

  return ret;
}

Vector3 Matrix4::operator*(const Vector3& v) const {
  Vector3 ret;
  double sum;

  for (int r = 0; r < 3; ++r) {
    sum = 0.0;
    for (int c = 0; c < 3; ++c) {
      sum += m_[r][c] * v[c];
    }
    ret[r] = sum;
  }

  return ret;
}

bool Matrix4::operator==(const Matrix4& m) const {
  for (int r = 0; r < 3; ++r) {
    for (int c = 0; c < 3; ++c) {
      if (fabs(m_[r][c] - m.m_[r][c]) > 0.00000001) {
        return false;
      }
    }
  }
  return true;
}
