// КМБО-01-20, Гребнев Никита, 5 вариант.

#include <iostream>
#include <string>

// Для сложения с точной (3 коордианты) создадим класс точки
class Point {
private:
	double x, y, z;
public:
	Point(double input_x = 0, double input_y = 0, double input_z = 0) : x(input_x), y(input_y), z(input_z) {}
	std::string get_formated_info() const {
		return ("(" + std::to_string(x) + "; " + std::to_string(y) + "; " + std::to_string(z) + ")");
	}
	~Point() {
		//nothing here
	}
	double get_x() const {
		return (this->x);
	}
	double get_y() const {
		return (this->y);
	}
	double get_z() const {
		return (this->z);
	}
};

// Требуемый по заданию 1.1 класс
class Vector_3d {
private:
	double delta_x;
	double delta_y;
	double delta_z;
public:
	Vector_3d(double input_x = 0, double input_y = 0, double input_z = 0) : delta_x(input_x), delta_y(input_y), delta_z(input_z) {
		//nothing here
	}
	virtual ~Vector_3d() {
		//nothing here
	}
	double get_x() const {
		return this->delta_x;
	}
	double get_y() const {
		return this->delta_y;
	}
	double get_z() const {
		return this->delta_z;
	}
	virtual std::string get_formated_info() const {
		return ( "(" + std::to_string(delta_x) + "; " + std::to_string(delta_y) + "; " + std::to_string(delta_z) + ")" );
	}
	Point operator+ (Point& point) const {
		return Point(this->delta_x + point.get_x(), this->delta_y + point.get_y(), this->delta_z + point.get_z());
	}
	double operator*(Vector_3d vector) const {
		return (this->delta_x * vector.delta_x + this->delta_y + vector.delta_y + this->delta_z * vector.delta_z);
	}
	/* //данная функция пока не нужна
	virtual double abs() const { //возвращает длинну вектора
		return std::sqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
	}
	*/
};

// Требуемый по заданию 1.2 класс
class Vector_3d_subclass final: public Vector_3d{
public:
	Vector_3d_subclass(double input_x = 0, double input_y = 0, double input_z = 0) : Vector_3d(input_x, input_y, input_z) {
		//nothing here
	}
	~Vector_3d_subclass() {
		//nothing here
	}
	Vector_3d_subclass operator* (double a) const {
		return Vector_3d_subclass(this->get_x() * a, this->get_y() * a, this->get_z() * a);
	}

	// в общем-то, следующая строка не нужна
	friend Vector_3d_subclass operator*(double a, Vector_3d_subclass& vector);
};

Vector_3d_subclass operator*(double a, Vector_3d_subclass& vector) {
	std::cout << "происходит умножение \"double * vector_3d\"\n";
	return (vector * a);
}

int main() {
	setlocale(LC_ALL, "Russian"); //отображение русского языка в консоли

	//задание 1.1
	Vector_3d v1;
	std::cout << "v1 = " << v1.get_formated_info()<<std::endl;

	Vector_3d v2(1.0, -1.0, 5.5);
	std::cout << "v2 = " << v2.get_formated_info() << std::endl;

	Vector_3d v3(10.0, 10.0, 10.);
	std::cout << "v3 = " << v3.get_formated_info() << std::endl;

	Point point_to_add(2.0, 2.0, 2.5);
	std::cout << "point_to_add = " << point_to_add.get_formated_info() << std::endl;

	Point result = v2 + point_to_add;
	std::cout << "point + v2 = " << result.get_formated_info() << std::endl;

	std::cout << "Скалярное произведение (v2, v3) = " << (v2*v3) << std::endl;

	//задание 1.2
	Vector_3d_subclass v4(1.0, -1.0, .0);
	std::cout << "v4 = " << v4.get_formated_info() << std::endl;
	float multiplier = 2.5;
	v4 = multiplier * v4;
	std::cout << multiplier << " * v4 = " << v4.get_formated_info() << std::endl;
	

}