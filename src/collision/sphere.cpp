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
  m_sphere_mesh.draw_sphere(shader, origin, radius * 1.2);
}

void Sphere::moveTo(Vector3D newPos) {
    this -> origin = newPos;
}

void Sphere::moveStep(Vector3D newPos) {
    this -> origin += newPos;
}

void Sphere::simulate(double frames_per_sec,
                      double simulation_steps,
                      vector<Vector3D> external_accelerations,
                      vector<CollisionObject *> *collision_objs,
                      Vector3D &collisionPt) {
    
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
    
    for (CollisionObject *cp : *collision_objs) {
        Sphere* sq = dynamic_cast<Sphere*>(cp);
        if (sq != nullptr && sq != this && sq->type != 0) { // check not the universe
            collideSphere(*sq, true, collisionPt, frames_per_sec, simulation_steps);
        }
    }
}

void Sphere::collideSphere(Sphere &p, bool realistic, Vector3D &collisionPt, double frames_per_sec,
                           double simulation_steps) {
     double delta_t = 1.0f / frames_per_sec / simulation_steps;
    Vector3D pPos = p.origin;
    Vector3D orig = this -> origin;
    if ((pPos - orig).norm() > 1 * (this->radius + p.radius))
        return;
    if (!realistic) {
        Vector3D dir = (orig - pPos).unit();
        Vector3D tangentPt = pPos + p.radius * dir;
        collisionPt.x = tangentPt.x;
        collisionPt.y = tangentPt.y;
        collisionPt.z = tangentPt.z;
        
        Vector3D actualLoc = pPos + (p.radius + this->radius) * dir;
        Vector3D lastPos = this -> prevPosition;
        Vector3D corrVec = actualLoc - lastPos;
        this->currPosition = lastPos + corrVec;
    } else {
        Vector3D dir = (orig - pPos).unit();
        Vector3D tangentPt = pPos + p.radius * dir;
        collisionPt.x = tangentPt.x;
        collisionPt.y = tangentPt.y;
        collisionPt.z = tangentPt.z;
        
        /////////////Naive Implementation ///////////////
        Vector3D normal = (this->origin - p.origin).unit();
        Vector3D p1Velocity = this->origin - this->prevPosition;
        Vector3D p2Velocity = p.origin - p.prevPosition;
//        Vector3D relVelocity = p1Velocity - p2Velocity;
//        Vector3D normalVelocity = dot(dot(relVelocity, normal), normal);
//
//        p1Velocity = p1Velocity - normalVelocity * 0.5;
//        p2Velocity = p2Velocity + normalVelocity * 0.5;
//
//        this->prevPosition = this->currPosition;
//        this->currPosition += p1Velocity;
//        p.prevPosition = p.currPosition;
//        p.currPosition += p2Velocity;
        ///////////// Conservation of Momentum ///////////////
        double m1 = this->mass;
        double m2 = p.mass;
        Vector3D x1 = this->origin;
        Vector3D x2 = p.origin;
        Vector3D newP1Vel = p1Velocity - (2 * m2/(m1 + m2)) * dot(p1Velocity - p2Velocity, x1 - x2) / normal.norm() * (x1 - x2);
        Vector3D newP2Vel = p2Velocity - (2 * m1/(m1 + m2)) * dot(p2Velocity - p1Velocity, x2 - x1) / normal.norm() * (x2 - x1);
//        newP1Vel *= 0.4;
//        newP2Vel *= 0.4;
        this->prevPosition = this->currPosition;
        this->currPosition += newP1Vel ;
        p.prevPosition = p.currPosition;
        p.currPosition += newP2Vel;
        
        
        
    }
}

