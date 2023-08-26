#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define NUM_LINE         2001
#define NUM_LINE_1       2000
#define PI               3.1415926

typedef struct 
{
	float x;
	float y;
	float z;
}Subway_point;

Subway_point cross_product(Subway_point v1, Subway_point v2);
Subway_point normalize(Subway_point v);
void plot_square_in_plane(Subway_point point, Subway_point normal_vector, double size,double height,FILE *fd);
void EstablishSubway(float subway_length, float subway_width , float subway_height , FILE *fd);
void  EstablishCoalSeam(float subway_length, float subway_width , float subway_height , FILE *fd,int num,float he);


int main(int argc , char *argv[])
{
/*如果第5个参数大于70 或者小于0*/
    if(atof(argv[4])>70||atof(argv[4])<0)
    {
        perror("error:倾斜角参数错误，倾斜角范围：0-70");
        return-1;
    }/*如果第2 3 4参数有一个小于0*/
    else if(argv[1]<0||argv[2]<0||argv[3]<0)
    {
        perror("error:长宽高参数错误，长宽高范围：0<x,y,z");
        return-1;
    }
    FILE *fd;
	fd = fopen(argv[7],"a");/*打开文件，并在末尾添加*/
    EstablishSubway(atof(argv[1]),atof(argv[2]),atof(argv[3]),fd);
    for(int  num = 1;num<=atoi(argv[5]);num++)
    {/*新创立一系列文件*/
        FILE *fd1;
        char txt[15] = "";
        sprintf(txt,"wall_%d.xyz",num);
        printf("%s\n",txt);
        fd1 = fopen(txt,"a"); /*打开文件 */
		/*计算并存储文件*/
        EstablishCoalSeam(atof(argv[1]),atof(argv[2]),atof(argv[3]),fd1,num,atof(argv[6]));
        fclose(fd1);/*关闭文件*/

    }
    fclose(fd);/*关闭文件*/
    return 0;
}

void  EstablishCoalSeam(float subway_length, float subway_width , float subway_height , FILE *fd1,int num,float he)
{
    float a = (float)1/30;
    Subway_point linepoint[NUM_LINE_1];
    float y_2 = ((pow(20,a)+1)*subway_length)/2;
    float y_1 = subway_length-((pow(20,a)+1)*subway_length)/2;
    for(float n = -subway_height/2;n<=subway_height/2;n+=0.5)
{

    for(int m =1;m<2000;m++)
    {
        float y_20_2 = ((pow(4+num*he,a)+1+2*(subway_width/2+0.05*m)/subway_length)*subway_length)/2;
        float y_20_1 = subway_length+2*(subway_width/2+0.05*m)-((pow(4+num*he,a)+1+2*(subway_width/2+0.05*m)/subway_length)*subway_length)/2;
        
        for(int i = 0;i <NUM_LINE_1;i++)
        {
            float j = (float)i;
            linepoint[i].x = (y_20_2-y_20_1)*j/NUM_LINE_1+y_20_1;
            linepoint[i].y =pow(2/subway_length*(linepoint[i].x-subway_length/2-subway_width/2-0.05*m),30);
            linepoint[i].z = n;
            if(linepoint[i].x<=subway_length/2&&linepoint[i].y>=4+(num-1)*he)
            {    
                fprintf(fd1,"%-8f %-8f %-8f\n",(y_2+y_1)-linepoint[i].x,linepoint[i].y,linepoint[i].z);	/*存储文件*/
                fprintf(fd1,"%-8f %-8f %-8f\n",linepoint[i].x,linepoint[i].y,linepoint[i].z);	
            }
        }

    }
}

}
void EstablishSubway(float subway_length, float subway_width , float subway_height , FILE *fd)
{
    float a = (float)1/30;
    float y_20_2 = ((pow(20,a)+1)*subway_length)/2;
    float y_20_1 = subway_length-((pow(20,a)+1)*subway_length)/2;


    Subway_point linepoint[NUM_LINE+1];
	for(int i = 0;i <=NUM_LINE;i++)
	{
		float j = (float)i;

        linepoint[i].x = (y_20_2-y_20_1)*j/NUM_LINE+y_20_1;
		linepoint[i].y =pow(2/subway_length*linepoint[i].x-1,30);
		linepoint[i].z = 0;
       fprintf(fd,"%-8f %-8f %-8f\n",linepoint[i].x,linepoint[i].y,linepoint[i].z);	

	}

    for(int i = 0;i<NUM_LINE;i++)
    {
        Subway_point n,n1,n2;
        n.x = linepoint[i+1].x - linepoint[i].x;
        n.y = linepoint[i+1].y - linepoint[i].y;
        n.z = linepoint[i+1].z - linepoint[i].z;

        Subway_point normal_vector = {n.x, n.y, n.z};
        double length = sqrt(normal_vector.x * normal_vector.x + normal_vector.y * normal_vector.y + normal_vector.z * normal_vector.z);
        normal_vector = normalize(normal_vector);
        Subway_point point_in_plane = { linepoint[i].x,  linepoint[i].y,  linepoint[i].z};
        double size = subway_width/2;
        double height = subway_height/2;
        plot_square_in_plane(point_in_plane, normal_vector, size, height, fd);
}

}

Subway_point cross_product(Subway_point v1, Subway_point v2) {
    Subway_point result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

Subway_point normalize(Subway_point v) {
    double length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    v.x /= length;
    v.y /= length;
    v.z /= length;
    return v;
}

void plot_square_in_plane(Subway_point point, Subway_point normal_vector, double size, double height, FILE *fd) {
    double D = - (normal_vector.x * point.x + normal_vector.y * point.y + normal_vector.z * point.z);
    Subway_point orthogonal_vector1 = {0.0, 0.0, 1.0}; 
    orthogonal_vector1 = normalize(orthogonal_vector1);
    orthogonal_vector1.x -= normal_vector.x * orthogonal_vector1.x;
    orthogonal_vector1.y -= normal_vector.y * orthogonal_vector1.y;
    orthogonal_vector1.z -= normal_vector.z * orthogonal_vector1.z;
    orthogonal_vector1 = normalize(orthogonal_vector1);
    Subway_point orthogonal_vector2 = cross_product(normal_vector, orthogonal_vector1);
    Subway_point square_points[4];
    square_points[0] = (Subway_point){point.x + height * orthogonal_vector1.x +size* orthogonal_vector2.x,
                                  point.y + height * orthogonal_vector1.y +size* orthogonal_vector2.y,
                                  point.z + height * orthogonal_vector1.z +size* orthogonal_vector2.z};
    square_points[1] = (Subway_point){point.x + height * -orthogonal_vector1.x + size*orthogonal_vector2.x,
                                  point.y + height * -orthogonal_vector1.y + size*orthogonal_vector2.y,
                                  point.z + height* -orthogonal_vector1.z +size* orthogonal_vector2.z};
    square_points[2] = (Subway_point){point.x + height * -orthogonal_vector1.x -size* orthogonal_vector2.x,
                                  point.y + height * -orthogonal_vector1.y -size* orthogonal_vector2.y,
                                  point.z + height* -orthogonal_vector1.z - size*orthogonal_vector2.z};
    square_points[3] = (Subway_point){point.x + height * orthogonal_vector1.x - size*orthogonal_vector2.x,
                                  point.y + height * orthogonal_vector1.y -size* orthogonal_vector2.y,
                                  point.z + height * orthogonal_vector1.z -size* orthogonal_vector2.z};

    Subway_point p[50];
    for(int i = 0;i<50;i++)
    {
        float t = (float)i/50;
        p[i].x = square_points[1].x + (square_points[2].x-square_points[1].x)*t;
        p[i].y = square_points[1].y + (square_points[2].y-square_points[1].y)*t;
        p[i].z = square_points[1].z + (square_points[2].z-square_points[1].z)*t;
        fprintf(fd,"%8f %-8f %-8f\n",p[i].x,p[i].y,p[i].z);	
        p[i].x = square_points[2].x + (square_points[3].x-square_points[2].x)*t;
        p[i].y = square_points[2].y + (square_points[3].y-square_points[2].y)*t;
        p[i].z = square_points[2].z + (square_points[3].z-square_points[2].z)*t;
        fprintf(fd,"%8f %-8f %-8f\n",p[i].x,p[i].y,p[i].z);	
    }
}
