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

    if(a_v.x)
    {

      m_matrix = m_matrix * glm::rotate(a_v.x, glm::tvec3<T>(1.0f,0.0f,0.0f));
    }
    if(a_v.y)
    {

      m_matrix = m_matrix * glm::rotate(a_v.y, glm::tvec3<T>(0.0f,1.0f,0.0f));
    }
    if(a_v.z)
    {
      m_matrix = m_matrix * glm::rotate(a_v.z, glm::tvec3<T>(0.0f,0.0f,1.0f));
    }

    //m_matrix = matrix;
  }

};

#endif //CAMERA_HPP
