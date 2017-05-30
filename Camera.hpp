#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"
#include "glm/ext.hpp"

template <class T>
class Camera{
public:

  glm::tmat4x4<T> m_matrix;
  glm::tvec3<T>   m_orbit;
  glm::tvec3<T>   m_lookAt;

  void LookAt();
  void Orbit();
  void Move(glm::tvec3<T> a_v)
  {
    m_matrix[3] += m_matrix * glm::tvec4<T>(a_v, 0);
  }

  void Pan(glm::tvec3<T> a_v)
  {
    glm::tmat4x4<T> matrix = m_matrix;

    if(a_v.x)
    {
      matrix[0] = m_matrix[0];
    }

    m_matrix = matrix;
  }

};

#endif //CAMERA_HPP
