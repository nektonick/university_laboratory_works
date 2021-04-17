// КМБО-01-20, Гребнев Никита, 5 вариант.
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
class MatrixException : public std::exception
{
protected:
	string info; //сообщение об ошибке
public:
	MatrixException(const char* s)	{
		info = string(s);
	}
	MatrixException(string s)	{
		info = s;
	}
	MatrixException(const MatrixException& e)	{
		info = e.info;
	}
	virtual ~MatrixException() {
		
	}
	virtual void print() const	{
		cout << info << endl;
	}
};

class IndexOutOfBoundsException : public MatrixException
{
protected:
	int index_row, index_column;
public:
	IndexOutOfBoundsException(string s, int r, int c) : MatrixException(s) {
		index_row = r; index_column = c;
	}
	virtual void print() const {
		cout << info << " Элемент с индексом ("<<index_row<<","<<index_column<<") находится вне границ матрицы" << endl;
	}
};

class IncompatibleMatrixDimensionsException : public MatrixException 
{
protected:
	int h1, w1, h2, w2;
public:
	IncompatibleMatrixDimensionsException(string s, int ih1, int iw1, int ih2, int iw2) : MatrixException(s), h1(ih1), w1(iw1), h2(ih2), w2(iw2) {	}
	virtual void print() const {
		cout << info << "Размерности " << "(" << h1 << "," << w1 << ") и" << "(" << h2 << "," << w2 << ") несовместимы\n";
	}
};

// На схеме IncompatibleMatrixDimensionsException наследуется от WrongDimensionsException, 
// но при моей реализации класса IncompatibleMatrixDimensionsException класс WrongMatrixSizeException лучше наследовать от MatrixException, 
// поскольку нет второй матрицы, размеры которой были бы несовместимы с данной
class WrongMatrixSizeException : public MatrixException 
{
protected:
	int h, w;
public:
	WrongMatrixSizeException(string s, int ih, int iw) : MatrixException(s), h(ih), w(iw) {	}
	virtual void print() const {
		cout << info << "Неверный размер матрицы: " << "(" << h << "," << w << ")\n";
	}
};

// Класс исключения для функции delete_zero_lines() 
class ZeroMatrixException : public WrongMatrixSizeException
{
protected:
public:
	ZeroMatrixException(string s, int ih, int iw) : WrongMatrixSizeException(s, ih, iw){ }
	virtual void print() const {
		cout << info << "Матрицы с размерами: " << "(" << h << "," << w << ") оказалась полностью заполнена нулями. При удалении всех строк получется матрица с нелевыми размерами.\n";
	}
};


class BaseMatrix
{
protected:
	double** ptr;
	int height;
	int width;
public:
	BaseMatrix(int Height = 2, int Width = 2)
	{
		if (Height <= 0 || Width <= 0)
			throw WrongMatrixSizeException("Ошибка в конструкторе BaseMatrix(int Height = 2, int Width = 2)\n", Height, Width);
		height = Height;
		width = Width;
		ptr = new double* [height];
		for (int i = 0; i < height; i++)
			ptr[i] = new double[width];
	}
	BaseMatrix(const BaseMatrix& M)
	{
		cout << "Вызван конструктор копирования\n";
		height = M.height;
		width = M.width;
		ptr = new double* [height];
		for (int i = 0; i < height; i++)
		{
			ptr[i] = new double[width];
			for (int j = 0; j < width; j++)
				ptr[i][j] = M.ptr[i][j];
		}
	}
	virtual ~BaseMatrix()
	{
		cout << "Вызван деструктор\n";
		if (ptr != nullptr)
		{
			for (int i = 0; i < height; i++)
				delete[] ptr[i];
			delete[] ptr;
			ptr = nullptr;
		}
	}
	virtual void print() const
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				cout << ptr[i][j] << " ";
			cout << "\n";
		}
	}
	virtual double& operator()(int row, int column) const
	{
		if (row < 0 || column < 0 || row >= height || column >= width)throw IndexOutOfBoundsException("Ошибка в operator()(int row, int column)\n", row, column);
		return ptr[row][column];
	}
	friend ostream& operator << (ostream& ustream, BaseMatrix
		obj);
	friend istream& operator >> (istream& ustream, BaseMatrix&
		obj);
};
ostream& operator << (ostream& ustream, BaseMatrix obj)
{
	//ustream<<my_manip;
	if (typeid(ustream).name() == typeid(ofstream).name())
	{
		ustream << obj.height << " " << obj.width << "\n";
		for (int i = 0; i < obj.height; i++)
		{
			for (int j = 0; j < obj.width; j++)
				ustream << obj.ptr[i][j] << "\n";
		}
		return ustream;
	}
	for (int i = 0; i < obj.height; i++)
	{
		for (int j = 0; j < obj.width; j++)
			ustream << obj.ptr[i][j] << " ";
		ustream << "\n";
	}
	return ustream;
}
istream& operator >> (istream& ustream, BaseMatrix& obj)
{
	if (typeid(ustream) == typeid(ifstream))
		ustream >> obj.height >> obj.width;
	for (int i = 0; i < obj.height; i++)
		for (int j = 0; j < obj.width; j++)
			ustream >> obj.ptr[i][j];
	return ustream;
}
ostream& my_manip(ostream& s)
{
	s.precision(4);
	s.fill('%');
	s.width(10);
	return s;
}

class DerivedMatrix : public BaseMatrix 
{
public:
	DerivedMatrix(int Height = 2, int Width = 2) : BaseMatrix(Height, Width) {};
	void random_fill(int min= -50, int max = 50) {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				ptr[i][j] = (double)(rand() % (max+1-min) ) + min;
	}

	// Функция для первого задания - "Удалить из матрицы все нулевые строки и столбцы."
	DerivedMatrix delete_zero_lines() const {
		int new_h = height;
		bool* current_row_is_zero = new bool[height]; //Обычный массив компилятор не даёт создать


		// Сначала пройдёмся по всем строкам и поймём, сколько у нас ненулевых
		// Помечаем также в current_row_is_zero строки как нулевые или ненулевые
		for (int i = 0; i < height; ++i) {
			bool zero_row = true;
			for (int j = 0; j < width; ++j) {
				if (ptr[i][j] != 0) {
					zero_row = false;
					current_row_is_zero[i] = false;
					break;
				}
			}
			if (zero_row) {
				current_row_is_zero[i] = true;
				--new_h;
			}
		}

		// Аналогично для столбцов
		int new_w = width;
		bool* current_column_is_zero = new bool[width];
		
		for (int i = 0; i < width; ++i) {
			bool zero_column = true;
			for (int j = 0; j < height; ++j) {
				if (ptr[j][i] != 0) {
					zero_column = false;
					current_column_is_zero[i] = false;
					break;
				}
			}
			if (zero_column) {
				--new_w;
				current_column_is_zero[i] = true;
			}
		}

		if (new_h == 0 || new_w == 0) {
			throw ZeroMatrixException("Ошибка в delete_zero_lines()\n", height, width);
		}

		DerivedMatrix ans(new_h, new_w);

		int new_matrix_i = 0;
		for (int i = 0; i < height; ++i) {
			if (current_row_is_zero[i]) {
				continue;
			}
			int new_matrix_j = 0;
			for (int j = 0; j < width; ++j) {
				if (current_column_is_zero[j]) {
					continue;
				}
				ans(new_matrix_i, new_matrix_j) = ptr[i][j];
				++new_matrix_j;
			}
			++new_matrix_i;
		}

		delete[] current_row_is_zero;
		delete[] current_column_is_zero;
		return ans; 
		/* Вся память, выделенная под ans будет удалена после завершения функции в соответствующем дискрукторе, 
		   но если мы правильно написали оператор копирования, то копирование нужных данных в новую память должно произойти раньше, так что всё ок */
	}
	
};


int main()
{
	setlocale(LC_ALL, "Russian");
	//srand(time(0));
	srand(1);

	DerivedMatrix m1(3, 3);
	m1.random_fill(0, 2);
	m1(0, 0) = 0; m1(0, 1) = 0; m1(0, 2) = 0; m1(1, 2) = 0; m1(2, 2) = 0;
	m1.print();
	DerivedMatrix m2 = m1.delete_zero_lines();
	m2.print();

	try
	{
		m1.random_fill(0, 0);
		DerivedMatrix m3 = m1.delete_zero_lines();
	}
	catch (ZeroMatrixException e) {
		e.print();
		//Дальнейшая обработка исключение - например, перегенерация матрицы m1
	}
	catch (...)
	{
		cout << "Невыловленная ошибка\n";
	}
}