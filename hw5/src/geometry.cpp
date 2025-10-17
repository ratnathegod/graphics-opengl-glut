#include "geometry.hpp"
#include "util.hpp"

int gShowNormals = 0;

void drawGrid(int n, double s)
{
    glDisable(GL_LIGHTING);
    glColor3f(0.2f,0.2f,0.25f);
    glBegin(GL_LINES);
    for(int i=-n;i<=n;i++){
        glVertex3d(i*s,0,-n*s); glVertex3d(i*s,0,n*s);
        glVertex3d(-n*s,0,i*s); glVertex3d(n*s,0,i*s);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawTorus(double R,double r,int nu,int nv)
{
    for(int i=0;i<nu;i++){
        double u0 = 2*M_PI*i/nu, u1 = 2*M_PI*(i+1)/nu;
        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<=nv;j++){
            double v = 2*M_PI*j/nv;
            double cu0=cos(u0), su0=sin(u0), cu1=cos(u1), su1=sin(u1);
            double cv=cos(v), sv=sin(v);
            double x0=(R + r*cv)*cu0, y0=r*sv, z0=(R + r*cv)*su0;
            double x1=(R + r*cv)*cu1, y1=y0,   z1=(R + r*cv)*su1;
            double nx0=cu0*cv, ny0=sv, nz0=su0*cv;
            double nx1=cu1*cv, ny1=sv, nz1=su1*cv;
            glNormal3d(nx0,ny0,nz0); glVertex3d(x0,y0,z0);
            glNormal3d(nx1,ny1,nz1); glVertex3d(x1,y1,z1);
            if(gShowNormals){ glDisable(GL_LIGHTING); glColor3f(1,1,0); drawNormalLine(x0,y0,z0,nx0,ny0,nz0); drawNormalLine(x1,y1,z1,nx1,ny1,nz1); glEnable(GL_LIGHTING);}        
        }
        glEnd();
    }
}

void drawHelicoid(double umax,double c,int nu,int nv)
{
    for(int i=0;i<nu;i++){
        double u0 = umax*i/nu, u1 = umax*(i+1)/nu;
        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<=nv;j++){
            double v = 2*M_PI*j/nv;
            auto vert=[&](double u,double v,double &x,double &y,double &z){ x=u*cos(v); y=u*sin(v); z=c*v; };
            auto normal=[&](double u,double v,double &nx,double &ny,double &nz){ double ru[3]={cos(v),sin(v),0}, rv[3]={-u*sin(v),u*cos(v),c}; double n[3]={ru[1]*rv[2]-ru[2]*rv[1], ru[2]*rv[0]-ru[0]*rv[2], ru[0]*rv[1]-ru[1]*rv[0]}; double len=sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2])+1e-9; nx=n[0]/len; ny=n[1]/len; nz=n[2]/len; };
            double x0,y0,z0,nx0,ny0,nz0; vert(u0,v,x0,y0,z0); normal(u0,v,nx0,ny0,nz0);
            double x1,y1,z1,nx1,ny1,nz1; vert(u1,v,x1,y1,z1); normal(u1,v,nx1,ny1,nz1);
            glNormal3d(nx0,ny0,nz0); glVertex3d(x0,y0,z0);
            glNormal3d(nx1,ny1,nz1); glVertex3d(x1,y1,z1);
            if(gShowNormals){ glDisable(GL_LIGHTING); glColor3f(1,0,1); drawNormalLine(x0,y0,z0,nx0,ny0,nz0); drawNormalLine(x1,y1,z1,nx1,ny1,nz1); glEnable(GL_LIGHTING);}        
        }
        glEnd();
    }
}

void drawRock(double s)
{
    const float v[][3]={{0,1.2f,0},{1.0f,0.5f,0.2f},{-0.8f,0.6f,0.5f},{-0.2f,0.8f,-0.9f},{0.9f,-0.2f,-0.6f},{-0.9f,-0.3f,-0.2f},{0.4f,-0.7f,0.8f},{-0.1f,-1.0f,0.1f},{0.8f,0.2f,-1.0f},{0.2f,1.0f,0.9f},{-0.8f,0.0f,1.0f}};
    const int f[][3]={{0,1,9},{0,9,2},{0,2,3},{0,3,1},{1,8,9},{1,4,8},{1,6,4},{1,9,6},{2,10,6},{2,9,10},{2,5,3},{2,10,5},{3,5,4},{3,4,1},{4,7,8},{4,6,7},{5,10,7},{5,7,4},{6,10,7},{8,7,9}};
    glPushMatrix(); glScalef(s,s,s);
    glBegin(GL_TRIANGLES);
    for(size_t i=0;i<sizeof(f)/sizeof(f[0]);++i){
        float A[3]={v[f[i][0]][0],v[f[i][0]][1],v[f[i][0]][2]};
        float B[3]={v[f[i][1]][0],v[f[i][1]][1],v[f[i][1]][2]};
        float C[3]={v[f[i][2]][0],v[f[i][2]][1],v[f[i][2]][2]};
        normalFromTriangle(A,B,C);
        glVertex3fv(A); glVertex3fv(B); glVertex3fv(C);
        if(gShowNormals){
            float cx=(A[0]+B[0]+C[0])/3.0f, cy=(A[1]+B[1]+C[1])/3.0f, cz=(A[2]+B[2]+C[2])/3.0f;
            float U[3]={B[0]-A[0],B[1]-A[1],B[2]-A[2]}, V[3]={C[0]-A[0],C[1]-A[1],C[2]-A[2]};
            float N[3]={U[1]*V[2]-U[2]*V[1], U[2]*V[0]-U[0]*V[2], U[0]*V[1]-U[1]*V[0]};
            float len=sqrt(N[0]*N[0]+N[1]*N[1]+N[2]*N[2])+1e-9f; drawNormalLine(cx,cy,cz,N[0]/len,N[1]/len,N[2]/len,0.4f);
        }
    }
    glEnd(); glPopMatrix();
}