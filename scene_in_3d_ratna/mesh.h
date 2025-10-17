
#ifndef MESH_H
#define MESH_H

typedef struct {
  int n_verts;
  int n_tris;
  float *pos;
  float *nor;
  unsigned int *idx;
} Mesh;


void   mesh_free(Mesh* m);


void   mesh_draw_triangles(const Mesh* m);


Mesh   mesh_make_twisted_torus(int Nu,int Nv, float R,float r, int twist_k);
Mesh   mesh_make_superellipsoid(int Nu,int Nv, float a,float b,float c, float e1,float e2);

#endif
