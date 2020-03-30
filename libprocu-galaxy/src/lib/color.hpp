//
// @file   : color.hpp
// @version: 2018-09-13
// @created: 2018-09-01
// @author : pyramid
//


//----------------------------------------------
// headers
//----------------------------------------------

// header include guards
#ifndef LIBPROCU_COLOR_H
#define LIBPROCU_COLOR_H

#include <ostream>

//#include "random.hpp"


//----------------------------------------------
// namespace
//----------------------------------------------

namespace procu {

//----------------------------------------------
// types
//----------------------------------------------

// forward declarations
typedef uint8_t byte;


//----------------------------------------------
// color struct
//----------------------------------------------


/**
 * @brief Global definition of color (rgba).
 * RGBA color can also be used in RGB images by
 * ignoring the alpha component.
 * @author pyramid
 */
struct Color {
  byte r; byte g; byte b; byte a;

  /**
   * @brief define rgba color with component range [0..255].
  **/
  Color(byte red, byte green, byte blue, byte alpha) {
    r = red;
    g = green;
    b = blue;
    a = alpha;    
  };

  /**
   * @brief define rgb color with component range [0..255].
   * This uses the c++11 function called delegating constructors
   * which allows to call a constructor from another constructor
  **/
  Color(byte red, byte green, byte blue) : Color(red, green, blue, (byte)0) { };
    //r = red;
    //g = green;
    //b = blue;
    //a = 0;    

  /**
   * @brief define rgb color with component range [0..255].
   * This uses the c++11 function called delegating constructors
   * which allows to call a constructor from another constructor
  **/
  Color(uint red, uint green, uint blue) : Color((byte)red, (byte)green, (byte)blue) { };

  /**
   * @brief define rgb color with component range [0..255].
   *
   * This uses the c++11 function called delegating constructors
   * which allows to call a constructor from another constructor
  **/
  Color(int red, int green, int blue) : Color((byte)red, (byte)green, (byte)blue) { };


  /**
   * @brief define rgba color with component range [0.0f..1.0f].
  **/
  Color(float red, float green, float blue, float alpha) {
    r = 255*red;
    g = 255*green;
    b = 255*blue;
    a = 255*alpha;    
  };

  /**
   * @brief define multiply * operator color with double
  **/
  Color& operator *(const double& factor) {
      r = r * factor;
      g = g * factor;
      b = b * factor;
      return *this;
  }


//----------------------------------------------

  void print() {
    std::cout << *this << "\n";
  }

  /**
   * @brief print rgba color components
   * this approach works but requires the friend definition
  **/
  friend std::ostream& operator<< (std::ostream& stream, const Color& color) {
    stream << "(r,g,b,a) = ";
    stream << (uint)color.r << ", ";
    stream << (uint)color.g << ", ";
    stream << (uint)color.b << ", ";
    stream << (uint)color.a;
    return stream;
  }
  //

  // ERROR
  // no match for ‘operator<<’ (operand types are ‘std::ostream’ {aka ‘std::basic_ostream<char>’} and ‘procu::Color’)
  // with and without inline
  /*
  std::ostream& operator<< (std::ostream& stream) {
    stream << (uint)r << ", ";
    stream << (uint)g << ", ";
    stream << (uint)b << ", ";
    stream << (uint)a << ", ";
    return stream;
  }
  */

}; // end color struct


/**
  * @brief print color components
  * this approach works but requires the inline definition
**/
/*
inline std::ostream& operator<< (std::ostream &stream, const Color& col) {
  stream << (uint)col.r << ", ";
  stream << (uint)col.g << ", ";
  stream << (uint)col.b << ", ";
  stream << (uint)col.a;
  return stream;
};
*/

//----------------------------------------------

/**
 * @brief Alternative color definition (rgb).
**/
struct ColorRGB { byte r; byte g; byte b; };


//----------------------------------------------

}; // end namespace


#endif // end header guards


