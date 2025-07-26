#include <iostream>
#include <functional>
using namespace std;

class Figure{
public:
    // 定义函数指针类型
    using DisplayCallback = function<void()>;
    using AreaCallback = function<double()>;
    DisplayCallback _displayCallback;
    AreaCallback _areaCallback;

    // 绑定回调函数
    void setDisplayCallback(DisplayCallback &&cb){
        _displayCallback = std::move(cb);
    }

    void setAreaCallback(AreaCallback && cb){
        _areaCallback = std::move(cb);
    }

    // 调用回调函数
    void handleDisplayCallback(){
        if(_displayCallback){
            _displayCallback();
        }
    }

    double handleAreaCallback(){
        if(_areaCallback){
            return _areaCallback();
        }
        return 0.0;
    }
};

class Rectangle{
public:
    Rectangle(double len, double wid)
    :_length(len)
    ,_width(wid)
    {}

    void display() const{
        cout << "Rectangle";
    }

    double area() const{
        // cout << "Rectangle area is " << _length * _width << endl;
        return _length * _width;
    }

    ~Rectangle(){}

private:
    double _length;
    double _width;
};

class Circle{
public:
    Circle(double r)
    :_radius(r)
    {}

    void display() const{
        cout << "Circle";
    }

    double area() const{
        // cout << "Circle area is " << 3.14 * _radius * _radius << endl;
        return (3.14 * _radius * _radius);
    }

    ~Circle() {}

private:
    double _radius;
};

void func(Figure &fig){
    fig.handleDisplayCallback();
    cout << " 的面积是：" << fig.handleAreaCallback() << endl;
}

int main(){
    Rectangle rec(10, 20);
    Circle cir(10);

    Figure fig;
    fig.setDisplayCallback(bind(&Rectangle::display, &rec));
    fig.setAreaCallback(bind(&Rectangle::area, &rec));
    func(fig);

    fig.setDisplayCallback(bind(&Circle::display, &cir));
    fig.setAreaCallback(bind(&Circle::area, &cir));
    func(fig);

    return 0;
}