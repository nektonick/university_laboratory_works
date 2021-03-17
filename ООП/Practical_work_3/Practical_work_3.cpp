// КМБО-01-20, Гребнев Никита, 5 вариант.

#include <iostream>
using namespace std;

class A1 {
protected:
	int a1;
public:
	A1(int input) : a1(input) {
		cout << "A1 constructor" << endl;
	};
	virtual void print() const {
		cout << "variable of A1 class" << endl;
	}
	virtual void show() const {
		cout << "a1=" << a1 << endl;
	}
};

class B1 : virtual public A1{
protected:
	int b1;
public:
	B1(int input1, int input2) : A1(input1), b1(input2) {
		cout << "B1 constructor" << endl;
	};
	virtual void print() const {
		cout << "variable of B1 class" << endl;
	}
	virtual void show() const {
		cout << "a1=" << a1 << ", b1=" << b1 << endl;
	}
};

class B2 : virtual public A1 {
protected:
	int b2;
public:
	B2(int input1, int input2) : A1(input1), b2(input2) {
		cout << "B2 constructor" << endl;
	};
	virtual void print() const {
		cout << "variable of B2 class" << endl;
	}
	virtual void show() const {
		cout << "a1=" << a1 << ", b2=" << b2 << endl;
	}
};

class C1 : virtual public B1, virtual public B2 {
protected:
	int c1;
public:
	C1(int input1, int input2, int input3, int input4) : A1(input1), B1(input1, input2), B2(input1, input3), c1(input4) {
		cout << "C1 constructor" << endl;
	};
	virtual void print() const {
		cout << "variable of C1 class" << endl;
	}
	virtual void show() const {
		cout << "a1=" << a1 << ", b1=" << b1 << ", b2=" << b2 << ", c1=" << c1 << endl;
	}
};


class C2 : virtual public B1, virtual public B2 {
protected:
	int c2;
public:
	C2(int input1, int input2, int input3, int input4) : A1(input1), B1(input1, input2), B2(input1, input3), c2(input4) {
		cout << "C2 constructor" << endl;
	};
	virtual void print() const {
		cout << "variable of C2 class" << endl;
	}
	virtual void show() const {
		cout << "a1=" << a1 << ", b1=" << b1 << ", b2=" << b2 << ", c2=" << c2 << endl;
	}
};

int main() {

	A1 test1(1);
	test1.print();
	test1.show();

	B1 test2(12, 11);
	test2.print();
	test2.show();

	B2 test3(112, 111);
	test3.print();
	test3.show();

	C1 test4(-1, -2, -3, -4);
	test4.print();
	test4.show();

	C2 test5(-10, -20, -30, -40);
	test5.print();
	test5.show();
}