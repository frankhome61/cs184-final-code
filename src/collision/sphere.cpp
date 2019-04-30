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

void Sphere::moveTo(Vector3D newPos) {
    this -> origin = newPos;
}

void Sphere::moveStep(Vector3D newPos) {
    this -> origin += newPos;
}

void Sphere::simulate(double frames_per_sec,
                      double simulation_steps,
                      vector<Vector3D> external_accelerations) {
    
    double delta_t = 1.0f / frames_per_sec / simulation_steps;
    Vector3D force(0, 0, 0);
    for (Vector3D accel: external_accelerations) {
        force += mass * accel;
    }
    
    Vector3D currPos = this->currPosition;
    Vector3D prevPos = this->prevPosition;
    Vector3D newAccel = force / this->mass;
    Vector3D newPos = currPos + currPos - prevPos + newAccel * pow(delta_t, 2);
    this->currPosition = newPos;
    this->prevPosition = currPos;
    this->origin = newPos;
    
}
