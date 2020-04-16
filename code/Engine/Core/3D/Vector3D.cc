#include "Core/3D/Vector3D.h"
#include <fstream>

namespace Future
{

std::ostream& operator<<(std::ostream& os, const Vector3D& rhs)
{
  os << " " << rhs.x << " " << rhs.y << " " << rhs.z;
  return os;
}

std::ifstream& operator>>(std::ifstream& is, Vector3D& lhs)
{
  is >> lhs.x >> lhs.y >> lhs.z;
  return is;
}

} // namespace Future