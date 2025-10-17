
#ifdef _WIN32
  #include <windows.h>
  #include <GL/gl.h>
#elif __APPLE__
  #include <OpenGL/gl.h>
#else
  #include <GL/gl.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


static void tri_normal(const float* a,const float* b,const float* c, float* n){
  float u[3]={ b[0]-a[0], b[1]-a[1], b[2]-a[2] };
  float v[3]={ c[0]-a[0], c[1]-a[1], c[2]-a[2] };
  n[0]=u[1]*v[2]-u[2]*v[1];
  n[1]=u[2]*v[0]-u[0]*v[2];
  n[2]=u[0]*v[1]-u[1]*v[0];
  float L = sqrtf(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
  if(L>1e-12f){ n[0]/=L; n[1]/=L; n[2]/=L; }
}

static void add_nor(float* nor, int i, const float* n){
  nor[3*i+0]+=n[0]; nor[3*i+1]+=n[1]; nor[3*i+2]+=n[2];
}

static void normalize_all(float* nor, int n_verts){
  for(int i=0;i<n_verts;i++){
    float x=nor[3*i+0], y=nor[3*i+1], z=nor[3*i+2];
    float L=sqrtf(x*x+y*y+z*z);
    if(L>1e-12f){ nor[3*i+0]=x/L; nor[3*i+1]=y/L; nor[3*i+2]=z/L; }
  }
}


void mesh_free(Mesh* m){
  if(!m) return;
  free(m->pos); free(m->nor); free(m->idx);
  memset(m,0,sizeof(*m));
}

void mesh_draw_triangles(const Mesh* m){
  glBegin(GL_TRIANGLES);
  for(int t=0;t<m->n_tris;t++){
    unsigned int i0=m->idx[3*t+0], i1=m->idx[3*t+1], i2=m->idx[3*t+2];
    const float* n0=&m->nor[3*i0]; const float* p0=&m->pos[3*i0];
    const float* n1=&m->nor[3*i1]; const float* p1=&m->pos[3*i1];
    const float* n2=&m->nor[3*i2]; const float* p2=&m->pos[3*i2];
    glNormal3fv(n0); glVertex3fv(p0);
    glNormal3fv(n1); glVertex3fv(p1);
    glNormal3fv(n2); glVertex3fv(p2);
  }
  glEnd();
}


Mesh mesh_make_twisted_torus(int Nu,int Nv, float R,float r, int twist_k){
  Mesh m={0};
  m.n_verts = Nu*Nv;
  m.n_tris  = (Nu-1)*(Nv-1)*2;
  m.pos = (float*)calloc(3*m.n_verts,sizeof(float));
  m.nor = (float*)calloc(3*m.n_verts,sizeof(float));
  m.idx = (unsigned int*)calloc(3*m.n_tris,sizeof(unsigned int));
  if(!m.pos||!m.nor||!m.idx){ fprintf(stderr,"OOM torus\n"); exit(1); }

  for(int j=0;j<Nv;j++){
    float v = (float)j/(Nv-1)*2.0f*(float)M_PI;
    for(int i=0;i<Nu;i++){
      float u = (float)i/(Nu-1)*2.0f*(float)M_PI;
      float theta = u + twist_k*v;
      float ct=cosf(theta), st=sinf(theta);
      float cv=cosf(v),     sv=sinf(v);
      float x=(R + r*ct)*cv;
      float y=(R + r*ct)*sv;
      float z= r*st;
      int id=j*Nu+i;
      m.pos[3*id+0]=x; m.pos[3*id+1]=y; m.pos[3*id+2]=z;
    }
  }
  int t=0;
  for(int j=0;j<Nv-1;j++){
    for(int i=0;i<Nu-1;i++){
      int i0=j*Nu+i, i1=j*Nu+i+1, i2=(j+1)*Nu+i, i3=(j+1)*Nu+i+1;
      m.idx[3*t+0]=i0; m.idx[3*t+1]=i2; m.idx[3*t+2]=i1; t++;
      m.idx[3*t+0]=i1; m.idx[3*t+1]=i2; m.idx[3*t+2]=i3; t++;

      float n[3];
      const float* a=&m.pos[3*i0]; const float* b=&m.pos[3*i2]; const float* c=&m.pos[3*i1];
      tri_normal(a,b,c,n); add_nor(m.nor,i0,n); add_nor(m.nor,i2,n); add_nor(m.nor,i1,n);
      a=&m.pos[3*i1]; b=&m.pos[3*i2]; c=&m.pos[3*i3];
      tri_normal(a,b,c,n); add_nor(m.nor,i1,n); add_nor(m.nor,i2,n); add_nor(m.nor,i3,n);
    }
  }
  normalize_all(m.nor, m.n_verts);
  return m;
}

/* ...existing code... */
static float sgn(float x){ return (x>0)-(x<0); }
static float pwr(float v,float e){ return powf(fabsf(v), e); }

Mesh mesh_make_superellipsoid(int Nu,int Nv, float a,float b,float c, float e1,float e2){
  Mesh m={0};
  m.n_verts = Nu*Nv;
  m.n_tris  = (Nu-1)*(Nv-1)*2;
  m.pos = (float*)calloc(3*m.n_verts,sizeof(float));
  m.nor = (float*)calloc(3*m.n_verts,sizeof(float));
  m.idx = (unsigned int*)calloc(3*m.n_tris,sizeof(unsigned int));
  if(!m.pos||!m.nor||!m.idx){ fprintf(stderr,"OOM superellipsoid\n"); exit(1); }

  for(int j=0;j<Nv;j++){
    float v = - (float)M_PI + (float)j/(Nv-1)*(2.0f*(float)M_PI);
    float sv = sinf(v), cv = cosf(v);
    for(int i=0;i<Nu;i++){
      float u = - (float)M_PI/2.0f + (float)i/(Nu-1)*(float)M_PI;
      float su = sinf(u), cu = cosf(u);

      float cu_e1 = pwr(cu,e1), su_e1 = pwr(su,e1);
      float cv_e2 = pwr(cv,e2), sv_e2 = pwr(sv,e2);

      float x = a * sgn(cu)*cu_e1 * sgn(cv)*cv_e2;
      float y = b * sgn(cu)*cu_e1 * sgn(sv)*sv_e2;
      float z = c * sgn(su)*su_e1;

      int id=j*Nu+i;
      m.pos[3*id+0]=x; m.pos[3*id+1]=y; m.pos[3*id+2]=z;
    }
  }
  int t=0;
  for(int j=0;j<Nv-1;j++){
    for(int i=0;i<Nu-1;i++){
      int i0=j*Nu+i, i1=j*Nu+i+1, i2=(j+1)*Nu+i, i3=(j+1)*Nu+i+1;
      m.idx[3*t+0]=i0; m.idx[3*t+1]=i2; m.idx[3*t+2]=i1; t++;
      m.idx[3*t+0]=i1; m.idx[3*t+1]=i2; m.idx[3*t+2]=i3; t++;

      float n[3];
      const float* a=&m.pos[3*i0]; const float* b=&m.pos[3*i2]; const float* c=&m.pos[3*i1];
      tri_normal(a,b,c,n); add_nor(m.nor,i0,n); add_nor(m.nor,i2,n); add_nor(m.nor,i1,n);
      a=&m.pos[3*i1]; b=&m.pos[3*i2]; c=&m.pos[3*i3];
      tri_normal(a,b,c,n); add_nor(m.nor,i1,n); add_nor(m.nor,i2,n); add_nor(m.nor,i3,n);
    }
  }
  normalize_all(m.nor, m.n_verts);
  return m;
}
