//
// Created by yujiaze on 5/7/19.
//

#ifndef COLLISIONOBJECT_PARTICLE_H
#define COLLISIONOBJECT_PARTICLE_H

#include "../clothMesh.h"
#include "../misc/sphere_drawing.h"
#include "collisionObject.h"

using namespace CGL;
using namespace std;

struct Particle : public CollisionObject {
public:
    Vector3D currPosition;
    Vector3D prevPosition;
    Vector3D origin;
    double mass;
    double radius;
    double radius2;
    const int type;

    Particle(const Vector3D &origin, const Vector3D &vel, double radius, double friction, double mass,
             int num_lat = 40, int num_lon = 40, int textureType = 1)
            : origin(origin), radius(radius), radius2(radius * radius),
              friction(friction), m_sphere_mesh(Misc::SphereMesh(num_lat, num_lon)), type(textureType), mass(mass),
              currPosition(origin) {}

    void render(GLShader &shader);

    void collide(PointMass &pm);

    void moveTo(Vector3D newPos);

    void moveStep(Vector3D newPos);

    void simulate(double frames_per_sec,
                  double simulation_steps,
                  vector<Vector3D> external_accelerations,
                  vector<CollisionObject *> *collision_objs,
                  Vector3D &collisionPt);

    void collideSphere(Particle &s, bool realistic, Vector3D &collisionPt);

private:

    double friction;

    float const GRAVITY_CONSTANT = 6.67408 * pow(10.0, -11.0);

    Misc::SphereMesh m_sphere_mesh;
};

#endif /* COLLISIONOBJECT_PARTICLE_H */
