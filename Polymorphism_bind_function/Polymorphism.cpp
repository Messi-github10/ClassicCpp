#include <iostream>
using namespace std;

// 多态

// 抽象类
class Figure{
public:
    virtual void display() const = 0;
    virtual double area() const = 0;
};

// 长方形
class Rectangle : public Figure{
public:
    Rectangle(double len, double wid)
    :_length(len)
    ,_width(wid)
    {}

    void display() const override{
        cout << "Rectangle";
    }

    double area() const override{
        // cout << "Rectangle area is " << _length * _width << endl;
        return _length * _width;
    }

    ~Rectangle(){}

private:
    double _length;
    double _width;
};

class Circle : public Figure{
public:
    Circle(double r)
    :_radius(r)
    {}

    void display() const override{
        cout << "Circle";
    }

    double area() const override{
        // cout << "Circle area is " << 3.14 * _radius * _radius << endl;
        return (3.14 * _radius * _radius);
    }

    ~Circle() {}

private:
    double _radius;
};

void func(const Figure &fig){
    fig.display();
    cout << " 的面积是：" << fig.area() << endl;
}

int main(){
    Rectangle rec(10, 20);
    Circle cir(10);

    func(rec);
    func(cir);

    return 0;
}
