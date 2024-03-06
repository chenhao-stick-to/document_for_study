#include <iostream>
using namespace std;
class Shape {
    public:
    Shape(){
        width =100;
    height =10;
    }
    //  Shape(int a,int b):width(a),height(b){
    //     cout<<"constrructor!";
    //  }
      virtual int area() = 0;
      int width;
      int height;
    
};

class Rectangle: public Shape {
   public:
        Rectangle(int wide,int he){
        //     width = wide;
        //    height = he;
        }
      int area () { 
         cout << "Rectangle class area :"; 
         return (width * height); 
      }
};

class Triangle: public Shape{
   public:
  Triangle(int wide,int he){
           this->width = wide;
           this->height = he;
        }
      int area () { 
         cout << "Triangle class area :"; 
         return (width * height / 2); 
      }
};

int main() {
    Shape *shape;
    Rectangle rec(10,7);
    Triangle  tri(10,5);

    shape = &rec;
    cout<<shape->area();
    cout<<endl;
    shape = &tri;
    cout<<shape->area();

   return 0;
}