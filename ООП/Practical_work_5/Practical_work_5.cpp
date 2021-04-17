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

template <typename T>
class BaseMatrix
{
protected:
	T** ptr;
	int height;
	int width;
public:
	BaseMatrix(int Height = 2, int Width = 2)
	{
		if (Height <= 0 || Width <= 0)
			throw WrongMatrixSizeException("Ошибка в конструкторе BaseMatrix(int Height = 2, int Width = 2)\n", Height, Width);
		height = Height;
		width = Width;
		ptr = new T* [height];
		for (int i = 0; i < height; i++)
			ptr[i] = new T[width];
	}
	BaseMatrix(const BaseMatrix& M)
	{
		cout << "Вызван конструктор копирования\n";
		height = M.height;
		width = M.width;
		ptr = new T* [height];
		for (int i = 0; i < height; i++)
		{
			ptr[i] = new T[width];
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
	virtual T& operator()(int row, int column) const
	{
		if (row < 0 || column < 0 || row >= height || column >= width)throw IndexOutOfBoundsException("Ошибка в operator()(int row, int column)\n", row, column);
		return ptr[row][column];
	}

	template<typename T1>
	friend ostream& operator << (ostream& s, BaseMatrix<T1>& m);
	template<typename T1>
	friend istream& operator >> (istream& s, BaseMatrix<T1>& m);
};

template <typename T1>
ostream& operator << (ostream& s, BaseMatrix<T1>& m)
{
	if (typeid(s) != typeid(ofstream)) {
		for (int i = 0; i < m.height; i++) {
			for (int j = 0; j < m.width; j++) {
				s << m.ptr[i][j] << " ";
			}
			s << "\n";
		}
	}
	else {
		s << " " << m.height << " " << m.width << " ";
		for (int i = 0; i < m.height; i++) {
			for (int j = 0; j < m.width; j++) {
				s << m.ptr[i][j];
				if (m.height != i + 1 || m.width != j + 1)
					s << " ";
			}
		}
	}
	return s;
}

template <typename T1>
istream& operator>> (istream& s, BaseMatrix<T1>& m)
{
	if (typeid(s) != typeid(ifstream)) {
		for (int i = 0; i < m.height; i++) {
			for (int j = 0; j < m.width; j++) {
				try {
					s >> m.ptr[i][j];
				}
				catch (...) {
					cout << "wrong input\n";
				}
			}
		}
	}
	else { //Сначала считываются высота, ширина, потом ячейки
		int h, w;
		s >> h >> w;
		if (h == m.height && w == m.width) {
			for (int i = 0; i < m.height; i++) {
				for (int j = 0; j < m.width; j++) {
					try {
						s >> m.ptr[i][j];
					}
					catch (...) {
						cout << "wrong input\n";
					}
				}
			}

		}
		else {
			if (s.eof()) {
				cout << "NO MATRIX ELEMENTS\n";
			}
			else {
				cout<<"Размеры не совпадают\n";
			}

		}
	}

	return s;
}

ostream& my_manip(ostream& s)
{
	s.precision(4);
	s.fill('%');
	s.width(10);
	return s;
}

template <typename T1>
class DerivedMatrix : public BaseMatrix <T1>
{
	
public:
	DerivedMatrix(int Height = 2, int Width = 2) : BaseMatrix<T1>(Height, Width) {};
	void random_fill(int min= -50, int max = 50) {
		
		for (int i = 0; i < this->height; i++)
			for (int j = 0; j < this->width; j++)
				this->ptr[i][j] = (T1)(rand() % (max+1-min) ) + min;
	}

	DerivedMatrix(istream& s){
		cout << "DerivedMatrix(istream& s)\n";
		s >> this->height >> this->width;

		if (this->height <= 0 || this->width <= 0)
			throw WrongMatrixSizeException("Ошибка в конструкторе BaseMatrix(int Height = 2, int Width = 2)\n", this->height, this->width);
		this->ptr = new T1 * [this->height];
		for (int i = 0; i < this->height; i++)
			this->ptr[i] = new T1[this->width];

		for (int i = 0; i < this->height; i++) {
			for (int j = 0; j < this->width; j++) {
					s >> this->ptr[i][j];
			}
		}
	}

	void overwrite_from_stream(istream& s) {
		if (this->ptr != nullptr)
		{
			for (int i = 0; i < this->height; i++)
				delete[] this->ptr[i];
			delete[] this->ptr;
			this->ptr = nullptr;
		}

		s >> this->height >> this->width;

		if (this->height <= 0 || this->width <= 0)
			throw WrongMatrixSizeException("Ошибка в конструкторе BaseMatrix(int Height = 2, int Width = 2)\n", this->height, this->width);
		this->ptr = new T1 * [this->height];
		for (int i = 0; i < this->height; i++)
			this->ptr[i] = new T1[this->width];

		for (int i = 0; i < this->height; i++) {
			for (int j = 0; j < this->width; j++) {
				s >> this->ptr[i][j];
			}
		}
	}


	// Функция для первого задания - "Удалить из матрицы все нулевые строки и столбцы."
	DerivedMatrix delete_zero_lines() const {
		int new_h = this->height;
		bool* current_row_is_zero = new bool[this->height]; //Обычный массив компилятор не даёт создать


		// Сначала пройдёмся по всем строкам и поймём, сколько у нас ненулевых
		// Помечаем также в current_row_is_zero строки как нулевые или ненулевые
		for (int i = 0; i < this->height; ++i) {
			bool zero_row = true;
			for (int j = 0; j < this->width; ++j) {
				if (this->ptr[i][j] != 0) {
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
		int new_w = this->width;
		bool* current_column_is_zero = new bool[this->width];
		
		for (int i = 0; i < this->width; ++i) {
			bool zero_column = true;
			for (int j = 0; j < this->height; ++j) {
				if (this->ptr[j][i] != 0) {
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
			throw ZeroMatrixException("Ошибка в delete_zero_lines()\n", this->height, this->width);
		}

		DerivedMatrix<T1> ans(new_h, new_w);

		int new_matrix_i = 0;
		for (int i = 0; i < this->height; ++i) {
			if (current_row_is_zero[i]) {
				continue;
			}
			int new_matrix_j = 0;
			for (int j = 0; j < this->width; ++j) {
				if (current_column_is_zero[j]) {
					continue;
				}
				ans(new_matrix_i, new_matrix_j) = this->ptr[i][j];
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

	DerivedMatrix<double> m1(3, 3);
	m1.random_fill(0, 2);
	m1(0, 0) = 0; m1(0, 1) = 0; m1(0, 2) = 0; m1(1, 2) = 0; m1(2, 2) = 0;
	m1.print();
	DerivedMatrix<double> m2 = m1.delete_zero_lines();
	m2.print();

	try
	{
		m1.random_fill(0, 0);
		DerivedMatrix<double> m3 = m1.delete_zero_lines();
	}
	catch (ZeroMatrixException e) {
		e.print();
		//Дальнейшая обработка исключение - например, перегенерация матрицы m1
	}
	catch (...)
	{
		cout << "Невыловленная ошибка\n";
	}

	DerivedMatrix<double> m3(3, 3);
	m3.random_fill(-5, 5);
	cout << "m3, записываемая в файл (2 раза)\n";
	m3.print();


	string file_name = "test1.txt";
	ofstream fout(file_name);

	if (fout.is_open()) {
		fout << m3 << m3;
		fout.close();
	}
	else {
		std::cout << "file not opened\n";
	}

	ifstream fin(file_name);
	if (fin) {
		while (!fin.eof())
		{
			fin >> m3;
			cout << "m3, считанная из файла\n";
			m3.print();
		}
		fin.close();
	}
	else {
		std::cout << "file not opened\n";
	}
	
	fin.open(file_name);
	if (fin) {
		cout << "Создаём матрицу m4, передавая в качестве аргумента поток\n";
		DerivedMatrix<double> m4(fin);
		m4.print();
		cout << "m2 до перезаписи из файла:\n";
		m2.print();
		m2.overwrite_from_stream(fin);
		cout << "m2 после перезаписи из файла:\n";
		m2.print();
	}
	else {
		std::cout << "file not opened\n";
	}

	

}