#ifndef COLLISIONOBJECT_SPHERE_H
#define COLLISIONOBJECT_SPHERE_H

#include "../clothMesh.h"
#include "../misc/sphere_drawing.h"
#include "collisionObject.h"

using namespace CGL;
using namespace std;

struct Sphere : public CollisionObject {
public:
    Vector3D currPosition;
    Vector3D prevPosition;
    Vector3D origin;
    double mass;
    double radius;
    double radius2;
   const int type;
  Sphere(const Vector3D &origin, const Vector3D &prev, double radius, double friction, double mass, int num_lat = 40, int num_lon = 40, int textureType=1)
      : origin(origin), radius(radius), radius2(radius * radius),
        friction(friction), m_sphere_mesh(Misc::SphereMesh(num_lat, num_lon)), type(textureType), mass(mass), currPosition(origin), prevPosition(prev) {}

  void render(GLShader &shader);
  void collide(PointMass &pm);
    
    void moveTo(Vector3D newPos);
    void moveStep(Vector3D newPos);
    void simulate(double frames_per_sec,
                  double simulation_steps,
                  vector<Vector3D> external_accelerations);

private:

    double friction;
    
    float const GRAVITY_CONSTANT = 6.67408 * pow(10.0, -11.0);
    
  Misc::SphereMesh m_sphere_mesh;
};

#endif /* COLLISIONOBJECT_SPHERE_H */
