#include <math.h>
#include "scale_angle.h"

extern int dsvd(float a[][2], int m, int n, float *w, float v[][2]);

static float frob_norm(float * f, int n)
{
   float sum=0;

   for(int i=0;i<n;i++)
     sum+=f[i]*f[i];

   return sqrt(sum);
}

static void matrix_dot(float * a, float * b, float * c, int m, int n, int k)
{
   for(int i=0;i<m;i++)
      for(int j=0;j<n;j++)
      {
        c[i*n+j]=0;
        for(int l=0;l<k;l++)
        {
          c[i*n+j]+=a[i*k+l]*b[l*n+j];

        }
      }

}


#define SWAP(a,b) do{float tmp; tmp=a;a=b;b=tmp;}while(0)
static void compute_affine_matrix(float cov[][2],float sigma, float trans_m[][2] )
{

   float u[2][2];
   float w[2];
   float v[2][2];
   float det=cov[0][0]*cov[1][1]-cov[0][1]*cov[1][0];
   float s[2][2]={{1,0},{0,1}};
   float c=1.0;
   float r0[2][2];
   float r[2][2];

   u[0][0]=cov[0][0];
   u[0][1]=cov[0][1];

   u[1][0]=cov[1][0];
   u[1][1]=cov[1][1];

   dsvd(u,2,2,(float *)w,v);

   SWAP(u[0][0],u[0][1]);
   SWAP(u[1][0],u[1][1]);
   SWAP(w[0],w[1]);
   SWAP(v[0][0],v[0][1]);
   SWAP(v[1][0],v[1][1]);

   if(det<0)
   {
       if(w[0]<w[1])
           s[1][1]=-1;
       else
           s[0][0]=-1;
   }

   matrix_dot((float *)u,(float *)s,(float *)r0,2,2,2);
   matrix_dot((float *)r0,(float *)v,(float *)r,2,2,2);


   if(sigma)
   {
     float diag[2][2];
     float trace[2][2];

     diag[0][0]=w[0];
     diag[0][1]=0;
     diag[1][0]=0;
     diag[1][1]=w[1];

     matrix_dot((float *)diag,(float *)s,(float *)trace,2,2,2);

     c=1.0/sigma*(trace[0][0]+trace[1][1]);

   }
     

  trans_m[0][0]=r[0][0]*c;
  trans_m[0][1]=r[0][1]*c;
  trans_m[1][0]=r[1][0]*c;
  trans_m[1][1]=r[1][1]*c;
  

}

static int get_probe_vec(float * landmark, int landmark_number, float * probe_vec, int probe_size, int desired_size)
{
   int i;

   float mean_face_x[]={0.224152, 0.75610125, 0.490127, 0.254149, 0.726104};
   float mean_face_y[]={0.2119465, 0.2119465, 0.628106, 0.780233, 0.780233};
   float from_mean[2]={0,0};
   float to_mean[2]={0,0};
   float sigma_from=0;
   float sigma_to=0;
   float cov[2][2]={{0,0},{0,0}};
   float trans_m[2][2];
   float padding=0.37;
   

   if(landmark_number!=5 ||  probe_size!=2)
      return -1;
 

   for(i=0;i<5;i++)
   {
       mean_face_x[i]=(padding+mean_face_x[i])/(2*padding+1)*desired_size;
       mean_face_y[i]=(padding+mean_face_y[i])/(2*padding+1)*desired_size;
   } 

   for(i=0;i<5;i++)
   {
       from_mean[0]+=landmark[i];
       from_mean[1]+=landmark[i+5];
       to_mean[0]+=mean_face_x[i];
       to_mean[1]+=mean_face_y[i];
   }

   from_mean[0]=from_mean[0]/5;
   from_mean[1]=from_mean[1]/5;

   to_mean[0]=to_mean[0]/5;
   to_mean[1]=to_mean[1]/5;


   for(i=0;i<5;i++)
   {
     float gap_from[2];
     float gap_to[2];
     float distance;

     gap_from[0]=landmark[i]-from_mean[0];
     gap_from[1]=landmark[i+5]-from_mean[1];

     distance=frob_norm(gap_from,2);

    sigma_from+=distance*distance;

     gap_to[0]=mean_face_x[i]-to_mean[0];
     gap_to[1]=mean_face_y[i]-to_mean[1];

     distance=frob_norm(gap_to,2);

     sigma_to+=distance*distance;

     cov[0][0]+=gap_to[0]*gap_from[0];
     cov[0][1]+=gap_to[0]*gap_from[1];
     cov[1][0]+=gap_to[1]*gap_from[0];
     cov[1][1]+=gap_to[1]*gap_from[1];
  
   }

   sigma_from=sigma_from/5;
   sigma_to=sigma_to/5;

   cov[0][0]/=5;
   cov[0][1]/=5;
   cov[1][0]/=5;
   cov[1][1]/=5;

   compute_affine_matrix(cov,sigma_from,trans_m);

   probe_vec[0]=trans_m[0][0];
   probe_vec[1]=trans_m[1][0];

   return 0;
   
}


int cal_scale_and_angle(float * landmark, int landmark_number, int desired_size,float * scale, float * angle)
{
   float probe_vec[2];

   if(get_probe_vec(landmark,landmark_number,probe_vec,2,desired_size)<0)
      return -1;

   scale[0]=frob_norm(probe_vec,2);
   angle[0]=180.0/M_PI*atan2(probe_vec[1],probe_vec[0]);

   return 0;
} 
