#include <iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include<math.h>

using namespace std;

//define  variable
struct  Point
{
    float num_x;
    float num_y;
    float num_z;
};

struct  nomal_vector
{
  float  num_x;
  float num_y;
  float num_z;  
};
struct thr_point
{
    Point point1;
    Point point2;
    Point point3;

};
struct  mesh
{
    thr_point  point_index;
    nomal_vector vector_index;
};
vector <mesh>my_mesh;


//  define function
vector <string>  split(const string &str,const string &delim )
{
    vector<string> res;
    if (""==str) return res;
    char *cstr = new char[str.length()+1];
    char *cdelim=new char[delim.length()+1];
    strcpy(cstr,str.c_str());
    strcpy(cdelim,delim.c_str());
    char *p=strtok(cstr,cdelim);
    while(p)
    {
        string s=p;
        res.push_back(s);
        p=strtok(NULL,cdelim);
    }
return res;

}
int read_stl(string path)
{
    ifstream my_file;
    my_file.open(path);
    if(!my_file)
    {
        return -1; 
    }
    string line;
    int i=10;
    int num_p;
    char head;
    char head1;
    char head2;
    while (getline(my_file,line) && i>0)
    {
        int len_line = line.length();
        head=line[0];
        head1=line[1];
        head2=line[2];
        vector<string>res = split(line," ");
        mesh Mesh;
        if (head=='f')
        {
            Mesh.vector_index.num_x=stof(res[2]);
            Mesh.vector_index.num_y=stof(res[3]);
            Mesh.vector_index.num_z=stof(res[4]);
            num_p=1;
            continue;
        }
        if (head==' '  &&  head1== ' ' && head2==' '&&  num_p==1  )
        {
            Mesh.point_index.point1.num_x=stof(res[1]);
            Mesh.point_index.point1.num_y=stof(res[2]);
            Mesh.point_index.point1.num_z=stof(res[3]);
            num_p=2;
            continue;
        } 
        if (head==' '  &&  head1== ' ' && head2==' '&&  num_p==2 )
        {
            Mesh.point_index.point2.num_x=stof(res[1]);
            Mesh.point_index.point2.num_y=stof(res[2]);
            Mesh.point_index.point2.num_z=stof(res[3]);
            num_p=3;
            continue;
        }         
        if (head==' '  &&  head1== ' ' && head2==' '&&  num_p==3  )
        {
            Mesh.point_index.point3.num_x=stof(res[1]);
            Mesh.point_index.point3.num_y=stof(res[2]);
            Mesh.point_index.point3.num_z=stof(res[3]);
            num_p=4;
        } 
        if(num_p==4)
        {
            my_mesh.push_back(Mesh);
            num_p=1;
        }
    }
    return 0;
}

double find_min_z()
{
    double min_z=my_mesh[0].point_index.point1.num_z;
    for(int i=0;i< my_mesh.size();i++)
    {
        float a_z=my_mesh[i].point_index.point1.num_z;
        float b_z=my_mesh[i].point_index.point2.num_z;
        float c_z=my_mesh[i].point_index.point3.num_z;         
        if  (a_z<min_z)  
        { 
            min_z=a_z;
        }
        else if(b_z<min_z)
        {
            min_z=b_z;
        }
        else if(c_z<min_z)
        {
            min_z=c_z;
        }
    }
    return min_z;

}
double triangle(float a_x,float a_y,float a_z,float b_x ,float b_y,float b_z,float c_x,float c_y,float c_z)
{
    float X1=a_x-b_x;
    float Y1=a_y-b_y;
    float Z1=a_z-b_z;
    float X2=a_x-c_x;
    float Y2=a_y-c_y;
    float Z2=a_z-c_z;
    float a= Y1*Z2-Y2*Z1;
    float b=X1*Z2-X2*Z1;
    float c=X1*Y2-X2*Y1;
    return (sqrt(a*a+b*b+c*c))/2;
}

double triangle_volume(double a_x,double a_y,double a_z,double b_x,double b_y,double b_z,double c_x,double c_y,double c_z,double d_x,double d_y,double d_z)
{
    float X1=a_x-b_x;
    float Y1=a_y-b_y;
    float Z1=a_z-b_z;
    float X2=a_x-c_x;
    float Y2=a_y-c_y;
    float Z2=a_z-c_z;
    float X3=a_x-d_x;
    float Y3=a_y-d_y;
    float Z3=a_z-d_z;
    double volume= (X1*Y2*Z3-X1*Z2*Y3-Y1*X2*Z3+Y1*Z2*X3+Z1*X2*Y3-Z1*Y2*X3)/6;    
    return abs(volume);
}
double mesh_surface_area()
{
    double total_area=0;
    for(int i=0;i< my_mesh.size();i++) 
    {
        float a_x=my_mesh[i].point_index.point1.num_x;
        float a_y=my_mesh[i].point_index.point1.num_y;
        float a_z=my_mesh[i].point_index.point1.num_z;
        float b_x=my_mesh[i].point_index.point2.num_x;
        float b_y=my_mesh[i].point_index.point2.num_y;
        float b_z=my_mesh[i].point_index.point2.num_z;
        float c_x=my_mesh[i].point_index.point3.num_x;
        float c_y=my_mesh[i].point_index.point3.num_y;
        float c_z=my_mesh[i].point_index.point3.num_z;         
        double area=triangle(a_x,a_y,a_z,b_x,b_y,b_z,c_x,c_y,c_z);     
        total_area= total_area+area;
    }
    return total_area;
}
double  mesh_volume()
{
    double z_min=find_min_z();
    int symbol=0;
    double one_minz=0;
    double one_maxz=0;
    double total_volume=0;
    double new_x1;double new_y1;double new_z1;
    double new_x2;double new_y2;double new_z2;
    double x;double y;double z;
    for(int i=0;i< my_mesh.size();i++)  
    {
        float a_x=my_mesh[i].point_index.point1.num_x;
        float a_y=my_mesh[i].point_index.point1.num_y;
        float a_z=my_mesh[i].point_index.point1.num_z;
        float b_x=my_mesh[i].point_index.point2.num_x;
        float b_y=my_mesh[i].point_index.point2.num_y;
        float b_z=my_mesh[i].point_index.point2.num_z;
        float c_x=my_mesh[i].point_index.point3.num_x;
        float c_y=my_mesh[i].point_index.point3.num_y;
        float c_z=my_mesh[i].point_index.point3.num_z;         
        float f_vector_x=my_mesh[i].vector_index.num_x;
        float f_vector_y=my_mesh[i].vector_index.num_y;
        float f_vector_z=my_mesh[i].vector_index.num_z;
        if (f_vector_z<0) 
        {
            symbol=-1;
        }
        else if (f_vector_z==0)
        {
            symbol=0;
        }
        else symbol=1;
         
        one_minz=a_z;one_maxz=a_z;
        if (b_z<one_minz) one_minz=b_z; else if (c_z<one_minz) one_minz=c_z;
        if (b_z>one_maxz) one_maxz=b_z; else if (c_z>one_maxz) one_maxz=c_z;
    
        if  (a_z==one_minz)
        {
            x=a_x;y=a_y;z=a_z;
            new_x1=b_x;new_y1=b_y;new_z1=b_z;new_x2=c_x,new_y2=c_y;new_z2=c_z;
        }
        else if(b_z==one_minz)
        {
             x=b_x;y=b_y;z=b_z;
            new_x1=a_x;new_y1=a_y;new_z1=a_z;new_x2=c_x,new_y2=c_y;new_z2=c_z;
        }
        else if (c_z==one_minz)
        {
             x=c_x;y=c_y;z=c_z;
            new_x1=a_x;new_y1=a_y;new_z1=a_z;new_x2=b_x,new_y2=b_y;new_z2=b_z;
        }
        

        float volume_1=(one_minz-z_min)*triangle(a_x,a_y,0,b_x,b_y,0,c_x,c_y,0);
        float volume_2=triangle_volume(x,y,z,new_x1,new_y1,one_minz,new_x1,new_y1,new_z1,new_x2,new_y2,one_minz);
        float volume_3=triangle_volume(x,y,z,new_x1,new_y1,new_z1,new_x2,new_y2,new_z2,new_x2,new_y2,one_minz);
        float volume=volume_1+volume_2+volume_3;
        total_volume=total_volume+symbol*volume;
    }
    return total_volume;
    
}

int main()
{
   int res =read_stl("data/extrude_ascii.stl");
   if (res<0) cout<<"read worried"<<endl; 
   double min_z=find_min_z();
    double my_mesh_area=mesh_surface_area();
    double my_mesh_volume=mesh_volume();
    cout<<"my_mesh_area=  "<<my_mesh_area<<endl;
    cout<<"my_mash_volume=  "<<my_mesh_volume<<endl;
    return 0;
} 
