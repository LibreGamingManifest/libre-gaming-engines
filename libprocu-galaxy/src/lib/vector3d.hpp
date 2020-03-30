//===================================
// @file   : vector3d.hpp
// @version: 2020-03-19
// @created: 2018-07-14
// @author : pyramid
// @author : pyramid
// @brief  : a simple 3d vector prototype library
// @source : https://efxa.org/2018/07/14/a-simple-3d-vector-prototype-library/
// @source : https://gist.github.com/rishav007/accaf10c7aa06135d34ddf3919ebdb3b
// see also: https://codereview.stackexchange.com/questions/62764/c-3d-vector-implementation
//===================================



#ifndef _VECTOR_3D_H_
#define _VECTOR_3D_H_

#include<math.h>
#include<assert.h>
// for constructor from vector
#include <vector>

class vector3d {
//private:

public:
  double x, y, z;

  vector3d() : vector3d(0, 0, 0) {}

  // constructor from std::vector
  vector3d(const std::vector<float> vec) {
      x = (double)vec[0];
      y = (double)vec[1];
      z = (double)vec[2];
  }
  // constructor from std::vector
  vector3d(const std::vector<double> vec) {
      x = vec[0];
      y = vec[1];
      z = vec[2];
  }

  vector3d(const double x, const double y, const double z) {
      set(x, y, z);
  }

  void set(const double xin, const double yin, const double zin) {
      x = xin;
      y = yin;
      z = zin;
  }

  vector3d operator + (const vector3d & vector) const {
      return vector3d(x + vector.x, y + vector.y, z + vector.z);
  }

  // assigning new result to the vector
  vector3d operator += (const vector3d &vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return vector3d(x,y,z);
  }

  vector3d operator - (const vector3d & vector) const {
      return vector3d(x - vector.x, y - vector.y, z - vector.z);
  }

  vector3d operator -= (const vector3d &vec) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return vector3d(x,y,z);
  }

  vector3d operator * (const double scale) const {
      return vector3d(x * scale, y * scale, z * scale);
  }

  vector3d operator *= (double value) {
      x *= value;
      y *= value;
      z *= value;
    return vector3d(x,y,z);
  }

  vector3d operator / (const double scale) const {
      return vector3d(x / scale, y / scale, z / scale);
  }

  vector3d operator /= (double value) {
      assert(value!=0);
      x /= value;
      y /= value;
      z /= value;
    return vector3d(x,y,z);
  }

  vector3d crossProduct(const vector3d & vector) const {
      return vector3d(y * vector.z - z * vector.y,
                      z * vector.x - x * vector.z,
                      x * vector.y - y * vector.x);
  }

  double innerProduct(const vector3d & vector) const {
      return (x * vector.x + y * vector.y + z * vector.z);
  }

  double length() const {
      return sqrt(x*x + y*y + z*z);
  }

  double distance(const vector3d &vec) {
      vector3d dist = *this - vec;
      return dist.magnitude();
  }

  vector3d negative (const vector3d &vector) const {
      return vector3d(-x, -y, -z);
  }

  void opposite() {
      set(-x, -y, -z);
  }

  vector3d normalized() const {
      const double distance = length();
      return vector3d(x / distance, y / distance, z / distance);
  }

  void normalize() {
      const double distance = length();
      set(x / distance, y / distance, z / distance);
  }

  //Dot product
  double dot_product(const vector3d &vec) {
      return x*vec.x+vec.y*y+vec.z*z;
  }

  //cross product
  vector3d cross_product(const vector3d &vec) {
      double ni=y*vec.z-z*vec.y;
      double nj=z*vec.x-x*vec.z;
      double nk=x*vec.y-y*vec.x;
      return vector3d(ni,nj,nk);
  }

  double magnitude() {
      return sqrt(square());
  }

  double square()
  {
      return x*x+y*y+z*z;
  }

  void print() {
      std::cout << x << " " << y << " " << z<< std::endl;
  }


}; // end class vector3d


#endif // _VECTOR_3D_H_

