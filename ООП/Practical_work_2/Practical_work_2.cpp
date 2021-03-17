// КМБО-01-20, Гребнев Никита, 5 вариант.
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class MyArrayParent{
protected:
	int capacity = 128; //выделенно памяти 
	int count = 0; // занято памяти
	double* ptr = nullptr; // указатель на массив
public:

	MyArrayParent(int Dimension = 128)
	{
		cout << "Конструктор MyArrayParent\n";
		ptr = new double[Dimension];
		capacity = Dimension;
		count = 0;
	}

	// A.4.
	MyArrayParent(double* arr, int len)
	{
		cout << "Конструктор MyArrayParent, принимающий обычный массив и его длинну\n";
		if(len > capacity) {
			delete[] ptr;
			ptr = nullptr;
			while (capacity <= len) {
				capacity *= 2;
			}
			ptr = new double[capacity];
			count = len;
			for (int i = 0; i < len; ++i) {
				ptr[i] = arr[i];
			}
		}
		else {
			if (ptr == nullptr) {
				ptr = new double[capacity];
			}

			count = len;
			for (int i = 0; i < len; ++i) {
				ptr[i] = arr[i];
			}
		}
	}

	// A.1.
	MyArrayParent(const MyArrayParent& V)
	{
		cout << "Конструктор копий MyArrayParent\n";
		delete[] ptr; // старая память никем больше не используется
		ptr = nullptr;
		capacity = V.capacity;
		count = V.count;
		ptr = new double[count];
		for (int i = 0; i < count; ++i) {
			ptr[i] = V.ptr[i];
		}
	}

	// A.2.
	MyArrayParent& operator=(const MyArrayParent& V) {
		cout << "Оператор= MyArrayParent\n";
		delete[] ptr; // старая память никем больше не используется
		ptr = nullptr;
		capacity = V.capacity;
		count = V.count;
		ptr = new double[count];
		for (int i = 0; i < count; ++i) {
			ptr[i] = V.ptr[i];
		}
		return *this;
	}

	// A.3.
	double& operator[](int index) {
		if (index < 0 || index >= count) {
			string error = "index " + to_string(index) + " out of range";
			throw (error);
		}

		return ptr[index];
	}

	
	~MyArrayParent()
	{
		cout << "Деструктор MyArrayParent\n";
		if (ptr != nullptr)
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}

	//обращение к полям
	int Capacity() const { 
		return capacity; 
	}
	int Size() const {
		return count; 
	}


	double GetComponent(int index) const {
		if (index < 0 || index >= count) {
			string error = "index " + to_string(index) + " out of range";
			throw (error);
		}

		return ptr[index];
	}

	void SetComponent(int index, double value)
	{
		try
		{
			if (index < 0) {
				throw invalid_argument("Func argument < 0.");
			}
			if (index > 999) {
				throw invalid_argument("Func argument too large.");
			}
			if (index >= 0 && index < count)
				ptr[index] = value;
			//сгенерировать исключение, если индекс неправильный

		}
		catch (invalid_argument& e)
		{
			cout << e.what();
			ptr = nullptr;
		}
	}


	void push(double value)
	{
		if (count < capacity) {
			ptr[count] = value;
			count++;
		}
		else {
			while (capacity <= count) {
				capacity *= 2;
			}

			double* temp_ptr = new double[capacity];
			for (int i = 0; i < count; ++i) {
				temp_ptr[i] = ptr[i];
			}
			delete[] ptr;
			ptr = temp_ptr;

			ptr[count] = value;
			count++;
		}
	}

	//удаление элемента с конца
	void RemoveLastValue()
	{
		if (count > 0) {
			count--;
		}
		//что делаем, если пуст?
	}

	void print() const {
		cout << "MyArrParent, size: " << count << ", values: {";
		int i = 0;
		for (i = 0; i < count; i++)
		{
			cout << ptr[i];
			if (i != count - 1)
				cout << ", ";
		}
		cout << "}\n";
	}
};

class MyArrayChild : public MyArrayParent
{
public:
	MyArrayChild(int Dimension = 128) : MyArrayParent(Dimension) {
		cout << "MyArrayChild constructor\n";
	}

	//B.1.
	void removeAt(int index = -1) {
		if (index < 0 || index >= count) {
			string error = "index " + to_string(index) + " out of range";
			throw (error);
		}
		if (index == (count - 1)) {
			count -= 1;
		}
		else {
			for (int i = 0; i < count - 1; ++i) {
				if (i < index) {
					continue;
				}
				if (i > index) {
					ptr[i] = ptr[i + 1];
				}
			}
			count -= 1;
		}
	}

	//B.2.
	int indexOf(double value, bool bFindFromStart = true) {
		if (bFindFromStart) {
			for (int i = 0; i < count; ++i) {
				if (ptr[i] == value) {
					return i;
				}
			}
			return -1;
		}
		else {
			for (int i = count - 1; i >= 0; --i) {
				if (ptr[i] == value) {
					return i;
				}
			}
			return -1;
		}
	};

	//B.3.
	void insertValAt(double value, int index = -1){
		if (index < 0 || index > count) {
			string error = "index " + to_string(index) + " out of range";
			throw (error);
		}
		if (index == (count)) {
			if (index > capacity) {
				capacity *= 2;

				double* temp_ptr = new double[capacity];
				for (int i = 0; i < count; ++i) {
					temp_ptr[i] = ptr[i];
				}
				delete[] ptr;
				ptr = temp_ptr;
			}
			++count;
			ptr[index] = value;
		}
		else {
			double temp_arr_item1;
			for (int i = 0; i < count+1; ++i) {
				if (i < index) {
					continue;
				}
				if (i == index) {
					temp_arr_item1 = ptr[i];
					ptr[i] = value;
				}
				if (i > index) {

					value = ptr[i];
					ptr[i] = temp_arr_item1;
					temp_arr_item1 = value;
				}
			}
			count += 1;
		}
	}

	//B.4.
	MyArrayChild spec_func(long long int x, long long int y) {
		MyArrayChild ans;

		for (int i = 0; i < count; ++i) {
			if (x * x + y * y == ptr[i] * ptr[i]) {
				ans.push(ptr[i]);
			}
		}
		return ans;
	}

};

// C.
class MySortedArray : public MyArrayChild {

private:
	int search_Binary(int left, int right, double value)
	{
		int max_right = right;
		int midd = 0;
		while (1)
		{
			midd = (left + right) / 2;

			if (value < ptr[midd])       
				right = midd - 1;     
			else if (value > ptr[midd])  
				left = midd + 1;	   
			else                       
				return midd;           
			if (left > right) {
				while (left >= count)
					left -= 1;
				while (value <= ptr[left])
					left -= 1;
				return left + 1;
			}
		}
	}

public:
	int indexOf(double value, bool bFindFromStart = true) {
		int ans = (search_Binary(0, count, value));
		if (count > 1 && (ptr[ans] == value)){
			return ans;
		}
		else
			return -1;
			
	}

	void push(double value)
	{
		if (count < capacity) {
			int index_for_insert = search_Binary(0, count, value);
			this->insertValAt(value, index_for_insert);
		}
		else {
			while (capacity <= count) {
				capacity *= 2;
			}

			double* temp_ptr = new double[capacity];
			for (int i = 0; i < count; ++i) {
				temp_ptr[i] = ptr[i];
			}
			delete[] ptr;
			ptr = temp_ptr;

			ptr[count] = value;
			count++;
		}
	}
	MySortedArray spec_func(long long int x, long long int y) {
		MySortedArray ans;

		for (int i = 0; i < count; ++i) {
			if (x * x + y * y == ptr[i] * ptr[i]) {
				ans.push(ptr[i]);
			}
		}
		return ans;
	}
};

int main()
{
	setlocale(LC_ALL, "Russian"); //отображение русского языка в консоли

	MyArrayParent A1;
	A1.push(1);	A1.push(2);	A1.push(3);
	A1.print();

	MyArrayParent A2 = A1; //A.1.
	A2.print();


	MyArrayParent A3;
	A3.push(9);	A3.push(9);	A3.push(9);

	A1 = A3; //A.2.
	A1.print();

	//A.3.
	try {
		cout << "A1[0]=" << A1[0] << endl;
		cout << "A1[-1]=" << A1[-1] << endl;
	}
	catch (const std::string& str) {
		cerr << str << endl;
	}
	try {
		cout << "A1[max_int]=" << A1[numeric_limits<int>::max()] << endl;
	}
	catch (const std::string& str) {
		cerr << str << endl;
	}

	double a[10] = { 0 };
	cout << "a={";
	for (int i = 0; i < 10; ++i) {
		a[i] = (i + 1) * 10;
		cout << a[i] << ", ";
	}
	cout << "}\n";

	MyArrayParent A4(a, 10); // A.4.
	A4.print();

	MyArrayChild B1;
	B1.push(12); B1.push(11); B1.push(10);

	cout<<"B1.IndexOf(11) = "<<B1.indexOf(11)<<endl; //B.2.
	B1.removeAt(2); //B.1.
	B1.print();
	B1.removeAt(1);
	B1.print();

	B1.insertValAt(5, 0); //B.3.
	B1.print();
	B1.insertValAt(16, 1);
	B1.insertValAt(-5, B1.Size());
	B1.print();

	MyArrayChild B2 = B1.spec_func(3, 4); //B.4.
	B2.print();

	MySortedArray C1; // C.
	C1.push(50); C1.push(100);
	try {
		C1.push(25); C1.push(75); C1.push(120); C1.push(5); C1.push(-5);
	}
	catch (const std::string& str) {
		cerr << str << endl;
	}
	C1.print();
	cout << C1.indexOf(-6) << " " << C1.indexOf(-5) << " " << C1.indexOf(76) << " " << C1.indexOf(120) << " " << C1.indexOf(130) << endl;


	return 0;
}