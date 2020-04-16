#pragma once

//------------------------------------------------------------------------
//  Name:   Vector3D.h
//  Desc:   3D vector struct
//------------------------------------------------------------------------

#include <math.h>
#include <limits>
#include "Core/Math/MathDefs.h"

namespace Future
{

struct Vector3D
{
  double x, y, z;

  Vector3D():x(0.0),y(0.0),z(0.0){}
  Vector3D(double a, double b, double c):x(a),y(b),z(c){}

  //sets x and y to zero
  void Zero(){x=0.0; y=0.0; z=0.0;}

  //sets x and y to one
  void One(){x=1.0; y=1.0; z=1.0;}

  //returns true if both x and y are zero
  bool isZero()const{return (x*x + y*y + z*z) < Math::MinDouble;}

  //returns the length of the vector
  inline double    Length()const;

  //returns the squared length of the vector (thereby avoiding the sqrt)
  inline double    LengthSq()const;

  inline void      Normalize();

  inline double    Dot(const Vector3D& v3)const;

//   //returns positive if v3 is clockwise of this vector,
//   //negative if anticlockwise (assuming the Y axis is pointing down,
//   //X axis to right like a Window app)
//   inline int       Sign(const Vector3D& v3)const;

  //returns the vector that is perpendicular to this one.
  inline Vector3D  Perp()const;

  //adjusts x and y so that the length of the vector does not exceed max
  inline void      Truncate(double max);

  //returns the distance between this vector and th one passed as a parameter
  inline double    Distance(const Vector3D &v3)const;

  //squared version of above.
  inline double    DistanceSq(const Vector3D &v3)const;

  inline void      Reflect(const Vector3D& norm);

  //returns the vector that is the reverse of this vector
  inline Vector3D  GetReverse()const;


  //we need some overloaded operators
  const Vector3D& operator+=(const Vector3D &rhs)
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  const Vector3D& operator-=(const Vector3D &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  const Vector3D& operator*=(const double& rhs)
  {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
  }

  const Vector3D& operator/=(const double& rhs)
  {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
  }

  bool operator==(const Vector3D& rhs)const
  {
    return (Math::isEqual(x, rhs.x) && Math::isEqual(y,rhs.y) && Math::isEqual(z,rhs.z) );
  }

  bool operator!=(const Vector3D& rhs)const
  {
    return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
  }
  
};

//-----------------------------------------------------------------------some more operator overloads
inline Vector3D operator*(const Vector3D &lhs, double rhs);
inline Vector3D operator*(double lhs, const Vector3D &rhs);
inline Vector3D operator-(const Vector3D &lhs, const Vector3D &rhs);
inline Vector3D operator+(const Vector3D &lhs, const Vector3D &rhs);
inline Vector3D operator/(const Vector3D &lhs, double val);
std::ostream& operator<<(std::ostream& os, const Vector3D& rhs);
std::ifstream& operator>>(std::ifstream& is, Vector3D& lhs);


//------------------------------------------------------------------------member functions

//------------------------- Length ---------------------------------------
//
//  returns the length of a 3D vector
//------------------------------------------------------------------------
inline double Vector3D::Length()const
{
  return sqrt(x * x + y * y + z * z);
}


//------------------------- LengthSq -------------------------------------
//
//  returns the squared length of a 3D vector
//------------------------------------------------------------------------
inline double Vector3D::LengthSq()const
{
  return (x * x + y * y + z * z);
}


//------------------------- Vec3DDot -------------------------------------
//
//  calculates the dot product
//------------------------------------------------------------------------
inline double Vector3D::Dot(const Vector3D &v3)const
{
  return x*v3.x + y*v3.y + z*v3.z;
}

// //------------------------ Sign ------------------------------------------
// //
// //  returns positive if v3 is clockwise of this vector,
// //  minus if anticlockwise (Y axis pointing down, X axis to right)
// //------------------------------------------------------------------------
// enum {clockwise = 1, anticlockwise = -1};

// inline int Vector3D::Sign(const Vector3D& v3)const
// {
//   if (y*v3.x > x*v3.y)
//   { 
//     return anticlockwise;
//   }
//   else 
//   {
//     return clockwise;
//   }
// }

//------------------------------ Perp ------------------------------------
//
//  Returns a vector perpendicular to this vector
//------------------------------------------------------------------------
inline Vector3D Vector3D::Perp()const
{
  return Vector3D(-z, y, x);
}

//------------------------------ Distance --------------------------------
//
//  calculates the euclidean distance between two vectors
//------------------------------------------------------------------------
inline double Vector3D::Distance(const Vector3D &v3)const
{
  double ySeparation = v3.y - y;
  double xSeparation = v3.x - x;
  double zSeparation = v3.z - z;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation + zSeparation*zSeparation);
}


//------------------------------ DistanceSq ------------------------------
//
//  calculates the euclidean distance squared between two vectors 
//------------------------------------------------------------------------
inline double Vector3D::DistanceSq(const Vector3D &v3)const
{
  double ySeparation = v3.y - y;
  double xSeparation = v3.x - x;
  double zSeparation = v3.z - z;

  return ySeparation*ySeparation + xSeparation*xSeparation + zSeparation*zSeparation;
}

//----------------------------- Truncate ---------------------------------
//
//  truncates a vector so that its length does not exceed max
//------------------------------------------------------------------------
inline void Vector3D::Truncate(double max)
{
  if (this->Length() > max)
  {
    this->Normalize();

    *this *= max;
  } 
}

//--------------------------- Reflect ------------------------------------
//
//  given a normalized vector this method reflects the vector it
//  is operating upon. (like the path of a ball bouncing off a wall)
//------------------------------------------------------------------------
inline void Vector3D::Reflect(const Vector3D& norm)
{
  *this += 2.0 * this->Dot(norm) * norm.GetReverse();
}

//----------------------- GetReverse ----------------------------------------
//
//  returns the vector that is the reverse of this vector
//------------------------------------------------------------------------
inline Vector3D Vector3D::GetReverse()const
{
  return Vector3D(-this->x, -this->y, -this->z);
}


//------------------------- Normalize ------------------------------------
//
//  normalizes a 3D Vector
//------------------------------------------------------------------------
inline void Vector3D::Normalize()
{ 
  double vector_length = this->Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    this->x /= vector_length;
    this->y /= vector_length;
    this->z /= vector_length;
  }
}


//------------------------------------------------------------------------non member functions

inline Vector3D Vec3DNormalize(const Vector3D &v)
{
  Vector3D vec = v;

  double vector_length = vec.Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    vec.x /= vector_length;
    vec.y /= vector_length;
    vec.z /= vector_length;
  }

  return vec;
}


inline double Vec3DDistance(const Vector3D &v1, const Vector3D &v3)
{

  double ySeparation = v3.y - v1.y;
  double xSeparation = v3.x - v1.x;
  double zSeparation = v3.z - v1.z;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation + zSeparation*zSeparation);
}

inline double Vec3DDistanceSq(const Vector3D &v1, const Vector3D &v3)
{

  double ySeparation = v3.y - v1.y;
  double xSeparation = v3.x - v1.x;
  double zSeparation = v3.z - v1.z;

  return ySeparation*ySeparation + xSeparation*xSeparation + zSeparation*zSeparation;
}

inline double Vec3DLength(const Vector3D& v)
{
  return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline double Vec3DLengthSq(const Vector3D& v)
{
  return (v.x*v.x + v.y*v.y + v.z*v.z);
}


// inline Vector3D POINTStoVector(const POINTS& p)
// {
//   return Vector3D(p.x, p.y, p.z);
// }

// inline Vector3D POINTtoVector(const POINT& p)
// {
//   return Vector3D((double)p.x, (double)p.y, (double)p.z);
// }

// inline POINTS VectorToPOINTS(const Vector3D& v)
// {
//   POINTS p;
//   p.x = (short)v.x;
//   p.y = (short)v.y;
//   p.z = (short)v.z;
//   return p;
// }

// inline POINT VectorToPOINT(const Vector3D& v)
// {
//   POINT p;
//   p.x = (long)v.x;
//   p.y = (long)v.y;
//   p.z = (long)v.z;
//   return p;
// }



//------------------------------------------------------------------------operator overloads
inline Vector3D operator*(const Vector3D &lhs, double rhs)
{
  Vector3D result(lhs);
  result *= rhs;
  return result;
}

inline Vector3D operator*(double lhs, const Vector3D &rhs)
{
  Vector3D result(rhs);
  result *= lhs;
  return result;
}

//overload the - operator
inline Vector3D operator-(const Vector3D &lhs, const Vector3D &rhs)
{
  Vector3D result(lhs);
  result.x -= rhs.x;
  result.y -= rhs.y;
  result.z -= rhs.z;
  return result;
}

//overload the + operator
inline Vector3D operator+(const Vector3D &lhs, const Vector3D &rhs)
{
  Vector3D result(lhs);
  result.x += rhs.x;
  result.y += rhs.y;
  result.z += rhs.z;
  return result;
}

//overload the / operator
inline Vector3D operator/(const Vector3D &lhs, double val)
{
  Vector3D result(lhs);
  result.x /= val;
  result.y /= val;
  result.z /= val;
  return result;
}

///////////////////////////////////////////////////////////////////////////////


//treats a window as a toroid
inline void WrapAround(Vector3D &pos, int MaxX, int MaxY, int MaxZ)
{
  if (pos.x > MaxX) {pos.x = 0.0;}

  if (pos.x < 0)    {pos.x = (double)MaxX;}

  if (pos.y > MaxY) {pos.y = 0.0;}

  if (pos.y < 0)    {pos.y = (double)MaxY;}

  if (pos.z > MaxZ) {pos.z = 0.0;}

  if (pos.z < 0)    {pos.z = (double)MaxZ;}
}

//returns true if the point p is not inside the region defined by top_left
//and bot_rgt
inline bool NotInsideRegion(Vector3D p,
                            Vector3D top_left,
                            Vector3D bot_rgt)
{
  return (p.x < top_left.x) || (p.x > bot_rgt.x) || 
         (p.y < top_left.y) || (p.y > bot_rgt.y);
}

inline bool InsideRegion(Vector3D p,
                         Vector3D top_left,
                         Vector3D bot_rgt)
{
  return !((p.x < top_left.x) || (p.x > bot_rgt.x) || 
         (p.y < top_left.y) || (p.y > bot_rgt.y));
}

inline bool InsideRegion(Vector3D p, int left, int top, int right, int bottom)
{
  return !( (p.x < left) || (p.x > right) || (p.y < top) || (p.y > bottom) );
}

//------------------ isSecondInFOVOfFirst -------------------------------------
//
//  returns true if the target position is in the field of view of the entity
//  positioned at posFirst facing in facingFirst
//-----------------------------------------------------------------------------
inline bool isSecondInFOVOfFirst(Vector3D posFirst,
                                 Vector3D facingFirst,
                                 Vector3D posSecond,
                                 double    fov)
{
  Vector3D toTarget = Vec3DNormalize(posSecond - posFirst);

  return facingFirst.Dot(toTarget) >= cos(fov/2.0);
}

} // namespace Future