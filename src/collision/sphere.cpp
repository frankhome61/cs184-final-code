#include <nanogui/nanogui.h>

#include "../clothMesh.h"
#include "../misc/sphere_drawing.h"
#include "sphere.h"

using namespace nanogui;
using namespace CGL;


void Sphere::collide(PointMass &pm) {
  // TODO (Part 3): Handle collisions with spheres.
    Vector3D pPos = pm.position;
    Vector3D orig = this -> origin;
    // Collision does not happen
    if ((pPos - orig).norm() >= this -> radius)
        return;
    Vector3D dir = (pPos - orig).unit();
    Vector3D tangentPt = orig + this -> radius * dir;
    Vector3D lastPos = pm.last_position;
    Vector3D corrVec = tangentPt - lastPos;
    pm.position = lastPos + (1 - this -> friction) * corrVec;
}

void Sphere::render(GLShader &shader) {
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
  m_sphere_mesh.draw_sphere(shader, origin, radius * 0.92);
}
