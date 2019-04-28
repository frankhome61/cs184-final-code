#include <iostream>
#include <math.h>
#include <random>
#include <vector>

#include "cloth.h"
#include "collision/plane.h"
#include "collision/sphere.h"

using namespace std;

Cloth::Cloth(double width, double height, int num_width_points,
             int num_height_points, float thickness) {
  this->width = width;
  this->height = height;
  this->num_width_points = num_width_points;
  this->num_height_points = num_height_points;
  this->thickness = thickness;

  buildGrid();
  buildClothMesh();
}

Cloth::~Cloth() {
  point_masses.clear();
  springs.clear();

  if (clothMesh) {
    delete clothMesh;
  }
}

/** Returns a random float number that ranges from (min, max) */
float randomFloat(float min, float max)
{
    float r = (float)rand() / (float)RAND_MAX;
    return min + r * (max - min);
}

/** checks if a given point is a pinned point*/
bool checkPinned(std::vector<vector<int>> target_sequence, Vector3D input) {
    for (std::vector<int> s: target_sequence) {
        if (input.x == s.at(0) && input.y == s.at(1)) {
            return true;
        }
    }
    return false;
}

/** Returns all the springs that creates structural constraint */
std::vector<Spring> Cloth::getStructural(int x, int y) {
    PointMass* currMassPtr = pointAtCoord(x, y);
    vector<Spring> res = std::vector<Spring>();
    if (x - 1 >= 0) {
        PointMass* otherMassPtr = pointAtCoord(x - 1, y);
        res.push_back(Spring(currMassPtr, otherMassPtr, STRUCTURAL));
    }
    if (y - 1 >= 0) {
        PointMass* otherMassPtr = pointAtCoord(x, y -1);
        res.push_back(Spring(currMassPtr, otherMassPtr, STRUCTURAL));
    }
    return res;
}

/** Returns all the springs that creates shearing constraint */
std::vector<Spring> Cloth::getShearing(int x, int y) {
    
    PointMass* currMassPtr = pointAtCoord(x, y);
    vector<Spring> res = std::vector<Spring>();
    if (x - 1 >= 0 && y - 1 >= 0) {
        PointMass* otherMassPtr = pointAtCoord(x - 1, y - 1);
        res.push_back(Spring(currMassPtr, otherMassPtr, SHEARING));
    }
    if (y - 1 >= 0 && x + 1 < this -> num_width_points) {
        PointMass* otherMassPtr = pointAtCoord(x + 1, y - 1);
        res.push_back(Spring(currMassPtr, otherMassPtr, SHEARING));
    }
    return res;
}

/** Returns all the springs that creates bending constraint */
std::vector<Spring> Cloth::getBending(int x, int y) {
    PointMass* currMassPtr = pointAtCoord(x, y);
    vector<Spring> res = std::vector<Spring>();
    if (x - 2 >= 0) {
        PointMass* otherMassPtr = pointAtCoord(x - 2, y);
        res.push_back(Spring(currMassPtr, otherMassPtr, BENDING));
    }
    if (y - 2 >= 0) {
        PointMass* otherMassPtr = pointAtCoord(x, y - 2);
        res.push_back(Spring(currMassPtr, otherMassPtr, BENDING));
    }
    return res;
}

/** Returns a pointer to the pointMass located at (x, y) */
PointMass* Cloth::pointAtCoord(int x, int y) {
    return this -> point_masses.data() + y * this -> num_width_points + x;
}

void Cloth::buildGrid() {
  // TODO (Part 1): Build a grid of masses and springs.
    for (int i = 0; i < this -> num_height_points; i++) {
        for (int j = 0; j < this -> num_width_points; j++) {
            double y_coord = float(i) / float(this -> num_height_points) * this -> height;
            double x_coord = float(j) / float(this -> num_width_points) * this -> width;
            Vector3D pos;
            if (this -> orientation == HORIZONTAL) {
                pos = Vector3D(x_coord, 1, y_coord);
            } else if (this -> orientation == VERTICAL) {
                double z_coord = randomFloat(-1.0/1000.0, 1.0/1000.0);
                pos = Vector3D(x_coord, y_coord, z_coord);
            } else {
                return;
            }
            bool pinned = checkPinned(this -> pinned, Vector3D(i, j, 0));
            point_masses.emplace_back(PointMass(pos, pinned));
        }
    }

    for (int i = 0; i < this -> num_height_points; i++) {
        for (int j = 0; j < this -> num_width_points; j++) {
            int x = j, y = i;
            std::vector<Spring> structural = getStructural(x , y);
            std::vector<Spring> shear = getShearing(x, y);
            std::vector<Spring> bend = getBending(x, y);
            for (Spring s: structural)
                springs.emplace_back(s);
            for (Spring s: shear)
                springs.emplace_back(s);
            for (Spring s: bend)
                springs.emplace_back(s);
        }
    }
}

void Cloth::simulate(double frames_per_sec, double simulation_steps, ClothParameters *cp,
                     vector<Vector3D> external_accelerations,
                     vector<CollisionObject *> *collision_objects) {
  double mass = width * height * cp->density / num_width_points / num_height_points;
  double delta_t = 1.0f / frames_per_sec / simulation_steps;
    
  // TODO (Part 2): Compute total force acting on each point mass.
    Vector3D force(0, 0, 0);
    for (Vector3D accel: external_accelerations) {
        force += mass * accel;
    }
    
    // Apply external Total force on each point mass
    for (PointMass &p: point_masses)
        if (!p.pinned)
            p.forces = force;
    
    // Calculate spring forces
    for (Spring &s: springs) {
        PointMass* ma = s.pm_a;
        PointMass* mb = s.pm_b;
        Vector3D pa = ma -> position;
        Vector3D pb = mb -> position;
        double distAB = (pa - pb).norm();
        double force_abs;
        if (s.spring_type == BENDING)
            force_abs = abs(cp -> ks * 0.2 * (distAB - s.rest_length));
        else
            force_abs = abs(cp -> ks * (distAB - s.rest_length));
        Vector3D dir = (pb - pa).unit();
        if ((s.spring_type == STRUCTURAL && !cp -> enable_structural_constraints) ||
            (s.spring_type == SHEARING && !cp -> enable_shearing_constraints) ||
            (s.spring_type == BENDING && !cp -> enable_bending_constraints)) {
            continue;
        } else {
            if (!ma -> pinned)
                ma -> forces += -force_abs * dir;
            if (!mb -> pinned)
                mb -> forces += force_abs * dir;
        }
    }
    

  // TODO (Part 2): Use Verlet integration to compute new point mass positions
    for (PointMass &p: point_masses) {
        Vector3D lastPos = p.last_position;
        Vector3D currPos = p.position;
        Vector3D newAccel = p.forces / mass;
        Vector3D newPos = currPos + (1 - cp -> damping / 100.0) * (currPos - lastPos) + newAccel * pow(delta_t, 2);
        p.position = newPos;
        p.last_position = currPos;
    }


  // TODO (Part 4): Handle self-collisions.
    build_spatial_map();
    for (PointMass &p: point_masses)
        self_collide(p, simulation_steps);


  // TODO (Part 3): Handle collisions with other primitives.
    for (PointMass &p: point_masses) {
        for (CollisionObject* c: *collision_objects)
            c -> collide(p);
    }
    
  // TODO (Part 2): Constrain the changes to be such that the spring does not change
  // in length more than 10% per timestep [Provot 1995].
    for (Spring &s: springs) {
        PointMass* ma = s.pm_a;
        PointMass* mb = s.pm_b;
        Vector3D pa = ma -> position;
        Vector3D pb = mb -> position;
        double distAB = (pa - pb).norm();
        if (distAB > 1.1 * s.rest_length) {
            if (ma -> pinned && mb -> pinned) {
                continue;
            } else if (ma -> pinned && !mb -> pinned) {
                double k = (1.1 * s.rest_length) / distAB;
                mb -> position = k * (pb - pa) + pa;
            } else if (!ma -> pinned && mb -> pinned) {
                double k = (1.1 * s.rest_length) / distAB;
                ma -> position = k * (pa - pb) + pb;
            } else {
                Vector3D pmid = 0.5 * (pa + pb);
                double pmaNorm = (pmid - pa).norm();
                double pmbNorm = (pmid - pb).norm();
                Vector3D pA_new = pmid + (0.5 * 1.1 * s.rest_length) / pmaNorm * (pa - pmid);
                Vector3D pB_new = pmid + (0.5 * 1.1 * s.rest_length) / pmbNorm * (pb - pmid);
                ma -> position = pA_new;
                mb -> position = pB_new;
            }
        }
    }
}

void Cloth::build_spatial_map() {
      for (const auto &entry : map) {
        delete(entry.second);
      }
      map.clear();
    // TODO (Part 4): Build a spatial map out of all of the point masses.
    for (PointMass &p: point_masses) {
        float hash = hash_position(p.position);
        if (map.find(hash) == map.end())
            map[hash] = new vector<PointMass* >();
        map[hash] -> push_back(&p);
    }
}

void Cloth::self_collide(PointMass &pm, double simulation_steps) {
  // TODO (Part 4): Handle self-collision for a given point mass.
    Vector3D corr(0, 0, 0);
    int count = 0;
    float hash = hash_position(pm.position);
    
    if (map.find(hash) != map.end()) {
        vector<PointMass*>* neighbors = map[hash];
        for (PointMass* p: *neighbors) {
            // Checks if it's not self
            double dist = (p -> position - pm.position).norm();
            if (dist != 0 && dist < 2.0 * this -> thickness) {
                Vector3D newP = p -> position + 2 * this -> thickness * (pm.position - p -> position).unit();
                Vector3D currCorr = newP - pm.position;
                
                corr += currCorr / simulation_steps;
                count ++;
            }
        }
    }
    
    if (count != 0) {
        corr /= count;
        pm.position += corr;
    }
}

float Cloth::hash_position(Vector3D pos) {
  // TODO (Part 4): Hash a 3D position into a unique float identifier that represents membership in some 3D box volume.
    float w = 3 * this -> width / this -> num_width_points;
    float h = 3 * this -> height / this -> num_height_points;
    float t = max(w, h);
    float xHash, yHash, zHash;
    xHash = int(pos.x / w);
    yHash = int(pos.y / h);
    zHash = int(pos.z / t);
    float val = xHash * 31 + yHash * pow(31, 2) + zHash * pow(31, 3);
    return val;
}

///////////////////////////////////////////////////////
/// YOU DO NOT NEED TO REFER TO ANY CODE BELOW THIS ///
///////////////////////////////////////////////////////

void Cloth::reset() {
  PointMass *pm = &point_masses[0];
  for (int i = 0; i < point_masses.size(); i++) {
    pm->position = pm->start_position;
    pm->last_position = pm->start_position;
    pm++;
  }
}

void Cloth::buildClothMesh() {
  if (point_masses.size() == 0) return;

  ClothMesh *clothMesh = new ClothMesh();
  vector<Triangle *> triangles;

  // Create vector of triangles
  for (int y = 0; y < num_height_points - 1; y++) {
    for (int x = 0; x < num_width_points - 1; x++) {
      PointMass *pm = &point_masses[y * num_width_points + x];
      // Get neighboring point masses:
      /*                      *
       * pm_A -------- pm_B   *
       *             /        *
       *  |         /   |     *
       *  |        /    |     *
       *  |       /     |     *
       *  |      /      |     *
       *  |     /       |     *
       *  |    /        |     *
       *      /               *
       * pm_C -------- pm_D   *
       *                      *
       */
      
      float u_min = x;
      u_min /= num_width_points - 1;
      float u_max = x + 1;
      u_max /= num_width_points - 1;
      float v_min = y;
      v_min /= num_height_points - 1;
      float v_max = y + 1;
      v_max /= num_height_points - 1;
      
      PointMass *pm_A = pm                       ;
      PointMass *pm_B = pm                    + 1;
      PointMass *pm_C = pm + num_width_points    ;
      PointMass *pm_D = pm + num_width_points + 1;
      
      Vector3D uv_A = Vector3D(u_min, v_min, 0);
      Vector3D uv_B = Vector3D(u_max, v_min, 0);
      Vector3D uv_C = Vector3D(u_min, v_max, 0);
      Vector3D uv_D = Vector3D(u_max, v_max, 0);
      
      
      // Both triangles defined by vertices in counter-clockwise orientation
      triangles.push_back(new Triangle(pm_A, pm_C, pm_B, 
                                       uv_A, uv_C, uv_B));
      triangles.push_back(new Triangle(pm_B, pm_C, pm_D, 
                                       uv_B, uv_C, uv_D));
    }
  }

  // For each triangle in row-order, create 3 edges and 3 internal halfedges
  for (int i = 0; i < triangles.size(); i++) {
    Triangle *t = triangles[i];

    // Allocate new halfedges on heap
    Halfedge *h1 = new Halfedge();
    Halfedge *h2 = new Halfedge();
    Halfedge *h3 = new Halfedge();

    // Allocate new edges on heap
    Edge *e1 = new Edge();
    Edge *e2 = new Edge();
    Edge *e3 = new Edge();

    // Assign a halfedge pointer to the triangle
    t->halfedge = h1;

    // Assign halfedge pointers to point masses
    t->pm1->halfedge = h1;
    t->pm2->halfedge = h2;
    t->pm3->halfedge = h3;

    // Update all halfedge pointers
    h1->edge = e1;
    h1->next = h2;
    h1->pm = t->pm1;
    h1->triangle = t;

    h2->edge = e2;
    h2->next = h3;
    h2->pm = t->pm2;
    h2->triangle = t;

    h3->edge = e3;
    h3->next = h1;
    h3->pm = t->pm3;
    h3->triangle = t;
  }

  // Go back through the cloth mesh and link triangles together using halfedge
  // twin pointers

  // Convenient variables for math
  int num_height_tris = (num_height_points - 1) * 2;
  int num_width_tris = (num_width_points - 1) * 2;

  bool topLeft = true;
  for (int i = 0; i < triangles.size(); i++) {
    Triangle *t = triangles[i];

    if (topLeft) {
      // Get left triangle, if it exists
      if (i % num_width_tris != 0) { // Not a left-most triangle
        Triangle *temp = triangles[i - 1];
        t->pm1->halfedge->twin = temp->pm3->halfedge;
      } else {
        t->pm1->halfedge->twin = nullptr;
      }

      // Get triangle above, if it exists
      if (i >= num_width_tris) { // Not a top-most triangle
        Triangle *temp = triangles[i - num_width_tris + 1];
        t->pm3->halfedge->twin = temp->pm2->halfedge;
      } else {
        t->pm3->halfedge->twin = nullptr;
      }

      // Get triangle to bottom right; guaranteed to exist
      Triangle *temp = triangles[i + 1];
      t->pm2->halfedge->twin = temp->pm1->halfedge;
    } else {
      // Get right triangle, if it exists
      if (i % num_width_tris != num_width_tris - 1) { // Not a right-most triangle
        Triangle *temp = triangles[i + 1];
        t->pm3->halfedge->twin = temp->pm1->halfedge;
      } else {
        t->pm3->halfedge->twin = nullptr;
      }

      // Get triangle below, if it exists
      if (i + num_width_tris - 1 < 1.0f * num_width_tris * num_height_tris / 2.0f) { // Not a bottom-most triangle
        Triangle *temp = triangles[i + num_width_tris - 1];
        t->pm2->halfedge->twin = temp->pm3->halfedge;
      } else {
        t->pm2->halfedge->twin = nullptr;
      }

      // Get triangle to top left; guaranteed to exist
      Triangle *temp = triangles[i - 1];
      t->pm1->halfedge->twin = temp->pm2->halfedge;
    }

    topLeft = !topLeft;
  }

  clothMesh->triangles = triangles;
  this->clothMesh = clothMesh;
}
