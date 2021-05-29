// КМБО-01-20, Гребнев Никита, 5 вариант.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

bool TWO_SIDES_PRINT_IN_TEST_0 = true;

/*
Для хранения даты рождения пациента создан простейший класс даты.
Входные данные в нём не проверяются на коректность, все поля публичные.
Вместо него лучше было бы использовать класс boost::gregorian::date из "boost/date_time/gregorian/gregorian.hpp",
однако в учебном проекте я считаю это излишним.
*/
class Date {
public:
	int year;
	int mounth;
	int day;
	Date(int d = 1, int m = 1, int y = 2000) {
		day = d;
		mounth = m;
		year = y;
	}
	friend ostream& operator<<(ostream& s, const Date& d);
	friend istream& operator>>(istream& s, Date& d);
};
ostream& operator<<(ostream& s, const Date& d) {
	if (typeid(s) == typeid(ofstream)) {
		s << d.day << " " << d.mounth << " " << d.year;
	}
	else {
		s << d.day << "." << d.mounth << "." << d.year;
	}

	return s;
}
istream& operator>>(istream& s, Date& d) {
	if (typeid(s) == typeid(ifstream)) {
		string line;
		getline(s, line);
		std::istringstream iss(line);
		iss >> d.day >> d.mounth >> d.year;

	}
	s >> d.day >> d.mounth >> d.year;
	return s;
}


/*
«Пациент».
Поля: фамилия, имя, дата рождения, телефон, адрес, номер карты, группа крови.
*/
class Patient {
private:
	/* 	Храним количество экземпляров класса, чтобы в номере карты по дефолту присваивать текущее количество пациентов. */
	static int count_of_class_elements;
protected:

public:
	// Так как я всё-равно не осуществяю проверку корректности вводимых данных, а геттеров/сеттеров пришлось бы писать много, данные о пациенте будут public, а не protected
	string surname;
	string name;
	Date date_of_birth;
	string phone_number; // формат номера неизвестен, поэтому string, а не int
	string address;
	int сard_number;
	int blood_type;

	Patient(string i_surname = "Иванов", string i_name = "Иван", Date i_date_of_birth = Date(), string i_phone_number = "+7-000-000-00-00",
		string i_address = "ул. Большая Лубянка, 2, Москва, 107031", int i_сard_number = Patient::count_of_class_elements, int i_blood_type = 1) {
		surname = i_surname;
		name = i_name;
		date_of_birth = i_date_of_birth;
		phone_number = i_phone_number;
		address = i_address;
		сard_number = i_сard_number + 1;
		blood_type = i_blood_type;
		++Patient::count_of_class_elements;
	}

	Patient(const Patient& p) {
		surname = p.surname;
		name = p.name;
		date_of_birth = p.date_of_birth;
		phone_number = p.phone_number;
		address = p.address;
		сard_number = p.сard_number;
		blood_type = p.blood_type;
	}
	Patient& operator=(const Patient& p) {
		surname = p.surname;
		name = p.name;
		date_of_birth = p.date_of_birth;
		phone_number = p.phone_number;
		address = p.address;
		сard_number = p.сard_number;
		blood_type = p.blood_type;
		return *this;
	}

	~Patient() {
		--Patient::count_of_class_elements;
	}
	friend ostream& operator<<(ostream& s, const Patient& p);
	friend istream& operator>>(istream& s, Patient& p);
};

// Инициализируем значение статической переменной.
int Patient::count_of_class_elements = 0;

ostream& operator<<(ostream& s, const Patient& p) {
	if (typeid(s) == typeid(ofstream)) {
		s << p.surname << "\n" << p.name << "\n" << p.date_of_birth << "\n" << p.phone_number
			<< "\n" << p.address << "\n" << p.сard_number << "\n" << p.blood_type;
	}
	else {
		s << "Фамилия: " << p.surname; 
		//<< "\nИмя: " << p.name << "\nДата рождения: " << p.date_of_birth << "\nНомер телефона: " << p.phone_number
			//<< "\nДом. адресс: " << p.address << "\nНомер мед. карты: " << p.сard_number << "\nГруппа крови: " << p.blood_type;
	}
	return s;
}

istream& operator>>(istream& s, Patient& p) {
	if (typeid(s) == typeid(ifstream)) {

		string line;


		getline(s, p.surname);
		getline(s, p.name);

		getline(s, line);
		std::istringstream iss1(line);
		iss1 >> p.date_of_birth;

		getline(s, p.phone_number);
		getline(s, p.address);

		getline(s, line);
		std::istringstream iss2(line);
		iss2 >> p.сard_number;

		getline(s, line);
		std::istringstream iss3(line);
		iss3 >> p.blood_type;
	}
	else {
		cout << "Bad Readed:\n" << p << endl;
		s >> p.surname >> p.name >> p.date_of_birth >> p.phone_number >> p.address >> p.сard_number >> p.blood_type;
	}

	return s;
}



// Элементы двусвязного списка
template<class T>
class Element
{
public:
	T info;
	Element* next;
	Element* prev;
	Element() {
		next = prev = nullptr;
	}

	Element(const T& data) {
		next = prev = NULL;
		info = data;
	}

	Element(Element* Prev, T data, Element* Next) {
		next = Next;
		prev = Prev;
		info = data;
	}

	Element(const Element& el) {
		next = el.next;
		prev = el.prev;
		info = el.info;
	}

	template<class T1>
	friend ostream& operator<<(ostream& s, const Element<T1>& el);
	template<class T1>
	friend istream& operator>>(istream& s, Element<T1>& el);

};

template<class T1>
ostream& operator<<(ostream& s, const Element<T1>& el) {
	s << el.info;
	return s;
}

template<class T1>
istream& operator>>(istream& s, Element<T1>& el) {
	s >> el.info;
	return s;
}



template<typename ValueType>
class ListIterator : public std::iterator<std::bidirectional_iterator_tag, ValueType> {
private:
	Element<ValueType>* ptr;
public:
	ListIterator() {
		ptr = nullptr;
	}
	ListIterator(Element<ValueType>* p) {
		ptr = p;
	}
	ListIterator(const ListIterator& it) {
		ptr = it.ptr;
	}
	bool operator!=(ListIterator const& other) const {
		return ptr != other.ptr;
	}
	bool operator==(ListIterator const& other) const {
		return ptr == other.ptr;
	}
	Element<ValueType>& operator*() {
		return *ptr;
	}

	ListIterator& operator++() {
		if (ptr == nullptr) {
			throw exception("No next item");
		}
		ptr = ptr->next;
		return *this;
	}
	ListIterator& operator--() {
		if (ptr == nullptr) {
			throw exception("No prev item");
		}
		ptr = ptr->prev;
		return *this;
	}

	ListIterator& operator++(int) {
		ListIterator temp(*this);
		this->operator++();
		return temp;
	}
	ListIterator& operator--(int) {
		ListIterator temp(*this);
		this->operator--();
		return temp;
	}

	ListIterator& operator=(const ListIterator& it) {
		ptr = it.ptr;
		return *this;
	}
	ListIterator& operator=(Element<ValueType>* p) {
		ptr = p; 
		return *this;
	}
};


/*
Абстрактный класс циклического двусвязного списка
Так как в реализации отсутствует указатель на конец,
неверно было бы наследоваться от других связных списков
*/
template<typename T>
class LoopedListBase {
protected:
	Element<T>* head = nullptr;
	size_t count = 0;
public:
	LoopedListBase() {
		cout << "LoopedListBase()\n";
		this->head = nullptr;
		this->count = 0;
	}

	LoopedListBase(const LoopedListBase& in) {
		cout << "LoopedListBase(const LoopedListBase& in)\n";
		this->head = new Element<T>(*(in.head));
		Element<T>* cur_this = this->head;
		Element<T>* cur_input = in.head;

		for (size_t i = 0; i < in.count; ++i) {
			cur_this->next = new Element<T>(cur_this, cur_input->next->info, nullptr);
			cur_this = cur_this->next;
			cur_input = cur_input->next;

			++this->count;
		}
	}

	LoopedListBase& operator=(LoopedListBase in) = delete;  

	virtual ~LoopedListBase() {
		cout << "~LoopedListBase()\n";
		this->clear();
	}

	virtual void clear() {
		Element<T>* cur = this->head;
		while(this->count > 0) {
			Element<T>* tmp = cur;
			cur = cur->next;
			delete tmp; tmp = nullptr;			
			--this->count;
		}
		this->head = nullptr;
	}

	// Чисто виртуальные функции
	virtual Element<T> pop() = 0;
	virtual void push(const T& value) = 0;
	virtual void insert_at(const T& value, int index) = 0;
	virtual void insert_after(const T& value, int index) = 0;
	virtual Element<T> remove(int index) = 0;
	virtual void print() = 0;
	
	
	// Индексы могут принимать произвольные значения, т.к. список циклический
	virtual Element<T>& operator()(int index) {
		Element<T>* ans = this->head;
		if (index == 0 || (index > 0 && index % this->count == 0)) {
			//ans = this->head;
		}
		else 
			if (index < 0) {
				index = -1 * index;
				index = index % this->count;
				for (size_t i = 0; i < index; ++i) {
					ans = ans->prev;
				}
			}
			else {
				index = index % this->count;
				for (size_t i = 0; i < index; ++i) {
					ans = ans->next;
				}
			}
		return *ans;
	}

	virtual Element<T>& operator[](int index) {
		return this->operator()(index);
	}

	// Геттеры
	virtual bool isEmpty() const {
		return (this->count == 0); 
	}
	virtual size_t get_count() const {
		return this->count;
	}
	virtual Element<T>* get_head() const {
		return this->head;
	}

};

template <typename T>
class IteratedLoopedList : public LoopedListBase<T> {
protected:
	void bind_elements(Element<T>* prev, Element<T>* cur, Element<T>* next) {
		if (prev != nullptr) {
			prev->next = cur;
		}
		if (next != nullptr) {
			next->prev = cur;
		}
		cur->prev = prev; cur->next = next;
	}
	void bind_elements(Element<T>* el1, Element<T>* el2) {
		if (el1 != nullptr) {
			el1->next = el2;
		}
		if (el2 != nullptr) {
			el2->prev = el1;
		}
	}

public:
	ListIterator<T> begin() const {
		ListIterator<T> it = this->head;
		return it;
	}

	IteratedLoopedList() : LoopedListBase<T>() {
		cout << "IteratedLoopedList()\n";
	}
	IteratedLoopedList(const IteratedLoopedList& in) : LoopedListBase<T>(in) {
		cout << "IteratedLoopedList(const IteratedLoopedList& in) \n";
	}

	virtual IteratedLoopedList& operator=(IteratedLoopedList& in){
		this->clear();

		cout << "IteratedLoopedList& operator=(IteratedLoopedList& in)\n";
		this->head = new Element<T>(*(in.head));
		Element<T>* cur_this = this->head;
		Element<T>* cur_input = in.head;

		for (size_t i = 0; i < in.count; ++i) {
			cur_this->next = new Element<T>(cur_this, cur_input->next->info, nullptr);
			cur_this = cur_this->next;
			cur_input = cur_input->next;

			++this->count;
		}
		return *this;
	}

	virtual ~IteratedLoopedList() {
		cout << "~IteratedLoopedList()\n";
	}

	virtual Element<T> pop() override {
		return this->remove(-1);
	}

	virtual void push(const T& value) override {
		this->insert_after(value, -1);
	}

	virtual void insert_at(const T& value, int index) override {
		Element<T>* new_el = new Element<T>(value);
		if (this->head == nullptr) {
			this->head = new_el;
			this->bind_elements(new_el, new_el, new_el);
		}
		else {
			Element<T>* prev_el = this->operator()(index).prev;
			Element<T>* next_el = &(this->operator()(index));
			if (next_el == this->head) {
				this->head = new_el;
			}
			this->bind_elements(prev_el, new_el, next_el);
		}
		this->count += 1;
	}

	virtual void insert_after(const T& value, int index) override {
		Element<T>* new_el = new Element<T>(value);
		if (this->head == nullptr) {
			this->head = new_el;
			this->bind_elements(new_el, new_el, new_el);
		}
		else {
			Element<T>* prev_el = &(this->operator()(index));
			Element<T>* next_el = this->operator()(index).next;
			this->bind_elements(prev_el, new_el, next_el);
			
		}
		this->count += 1;
	}


	virtual Element<T> remove(int index) override {
		Element<T> ans = this->operator()(index);
		Element<T>* to_delete = &(this->operator()(index));
		this->bind_elements(to_delete->prev, to_delete->next);
		if (this->head == to_delete) {
			this->head = to_delete->next;
		}
		delete to_delete; to_delete = nullptr;

		
		this->count -= 1;
		if (this->count == 0) {
			this->head = nullptr;
		}
		return ans;
	}

	virtual void print() override {
		cout << "Циклический связный список из " << this->count << "Элементов:" << endl;
		Element<T>* cur = this->head;
		if (cur == nullptr) {
			cout << "Нет элементов\n";
		}
		else {
			for (size_t i = 0; i < this->count; ++i) {
				cout << cur << endl;
				cur = cur->next;
			}
			cout << endl;
		}
	}
};

// Проверки:

// Проход по списку в одну и в другую сторону, от начала и до начала же включительно
void test0(IteratedLoopedList<Patient>& in) {
	cout << "Количество элементов:" << in.get_count() << endl;
	if (in.get_head() == nullptr) {
		cout << "Нет элементов\n";
		return;
	}

	cout << "~~~Проход слева направа:~~~\n";
	Element<Patient>* cur = in.get_head();
	for (size_t i = 0; i <= in.get_count(); ++i) {
		cout << *cur << endl;
		cur = cur->next;
	}
	if (TWO_SIDES_PRINT_IN_TEST_0) {
		cur = (in.get_head());
		//cur = (in.get_head()->prev);
		cout << "~~~Проход справа налево:~~~\n";
		for (size_t i = 0; i <= in.get_count(); ++i) {
			cout << *cur << endl;
			cur = cur->prev;
		}
		cout << endl;
	}
	
}

// Конструктор копирования
void test1() {
	cout << "\n~~~Test1~~~\n";
	IteratedLoopedList<Patient> list1; 

	list1.push(Patient("List1_1")); 
	cout << "List1:\n";
	test0(list1);

	list1.push(Patient("List1_2")); 
	cout << "List1:\n";
	test0(list1);

	list1.push(Patient("List1_3"));
	cout << "List1:\n";
	test0(list1);

	IteratedLoopedList<Patient> list2 = list1;

	cout << "List2:\n";
	test0(list2);
}

// Оператор присваивания + проверка множественного присваивания
void test2() {
	cout << "\n~~~Test2~~~\n";
	IteratedLoopedList<Patient> list1;
	list1.push(Patient("List1_1")); list1.push(Patient("List1_2")); list1.push(Patient("List1_3"));
	cout << "List1:\n";
	test0(list1);

	IteratedLoopedList<Patient> list2; list2.push(Patient("List2_1"));
	cout << "List2:\n";
	test0(list2);

	
	IteratedLoopedList<Patient> list3; list3.push(Patient("List3_1"));
	cout << "List3:\n";
	test0(list3);

	list3 = list2 = list1;
	cout << "Выполнено множественное присваивание\n";

	cout << "List2:\n";
	test0(list2);

	cout << "List3:\n";  
	test0(list3);
}

// clear()
void test3() {
	cout << "\n~~~Test3~~~\n";
	IteratedLoopedList<Patient> list1;
	list1.clear();
	cout << "List1:\n";
	test0(list1);

	list1.push(Patient("List1_1"));
	list1.clear();
	cout << "Снова List1:\n";
	test0(list1);

}

// operator() ([] аналогичны, так как просто вызывают operator())
void test4() {
	cout << "\n~~~Test4~~~\n";

	IteratedLoopedList<Patient> list1;
	list1.push(Patient("List1_1")); list1.push(Patient("List1_2")); list1.push(Patient("List1_3"));
	cout << "Получение элемента по индексу (отрицательному и положительному:\n";

	cout << "0: " << list1(0) << endl;
	cout << "1: " << list1(1) << endl;
	cout << "2: " << list1(2) << endl;
	cout << "3: " << list1(3) << endl;
	cout << "4: " << list1(4) << endl;
	cout << endl;
	cout << "-1: " << list1(-1) << endl;
	cout << "-2: " << list1(-2) << endl;
	cout << "-3: " << list1(-3) << endl;
	cout << "-4: " << list1(-4) << endl;

	cout << "\nИзменён элемент по индексу 1\n";
	list1(1) = Element<Patient>(list1(1).prev, Patient("List1_EDITED_1"), list1(1).next);

	cout << "\nИзменён элемент по индексу 0\n";
	list1(0) = Element<Patient>(list1(0).prev, Patient("List1_EDITED_2"), list1(0).next);

	test0(list1);


}

// push() 
void test5() {
	cout << "\n~~~Test5~~~\n";
	IteratedLoopedList<Patient> list1;
	list1.push(Patient("1"));
	list1.push(Patient("2"));
	list1.push(Patient("3"));
	list1.push(Patient("4"));

	cout << "List1:\n";
	test0(list1);
}

// pop()
void test6() {
	cout << "\n~~~Test6~~~\n";
	IteratedLoopedList<Patient> list1;
	list1.push(Patient("1"));
	list1.push(Patient("2"));
	list1.push(Patient("3"));
	list1.push(Patient("4"));
	cout << "Удаляем элементы, начиная с конца:\n";
	while (list1.get_count() > 0) {
		Element<Patient> p = list1.pop();
		cout << "p=" << p.info << endl;
	}

	cout << "Снова добавили и удалили 1 элемент:\n";
	list1.push(Patient("900"));
	Element<Patient> p = list1.pop();
	cout << "p=" << p.info << endl;

	cout << "List1:\n";
	test0(list1);
}

// insert_at()
void test7_1() {
	cout << "\n~~~Test7~~~\n";
	IteratedLoopedList<Patient> list1;

	cout << "\nВставка в пустой список\n";
	list1.insert_at(Patient("1"), 2);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом -2 \n";
	list1.insert_at(Patient("2"), -2);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом 0 \n";
	list1.insert_at(Patient("3"), 0);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом 1 \n";
	list1.insert_at(Patient("4"), 1);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом 2 \n";
	list1.insert_at(Patient("5"), 2);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом list1.get_count()-1 \n";
	list1.insert_at(Patient("6"), list1.get_count() - 1);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом  list1.get_count() = " << list1.get_count() <<"\n";
	list1.insert_at(Patient("7"), list1.get_count());
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом list1.get_count()+1 \n";
	list1.insert_at(Patient("8"), list1.get_count()+1);
	cout << "List1:\n";
	test0(list1);

}

// insert_after()
void test7_2() {
	cout << "\n~~~Test7~~~\n";
	IteratedLoopedList<Patient> list1;

	cout << "\nВставка в пустой список\n";
	list1.insert_after(Patient("1"), 2);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом -2 \n";
	list1.insert_after(Patient("2"), -2);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом 0 \n";
	list1.insert_after(Patient("3"), 0);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом 1 \n";
	list1.insert_after(Patient("4"), 1);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом 2 \n";
	list1.insert_after(Patient("5"), 2);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом list1.get_count()-1 \n";
	list1.insert_after(Patient("6"), list1.get_count() - 1);
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом  list1.get_count() = " << list1.get_count() << "\n";
	list1.insert_after(Patient("7"), list1.get_count());
	cout << "List1:\n";
	test0(list1);

	cout << "\nВставка с индексом list1.get_count()+1 \n";
	list1.insert_after(Patient("8"), list1.get_count() + 1);
	cout << "List1:\n";
	test0(list1);

}

// remove()
void test8() {
	IteratedLoopedList<Patient> list1;
	for (size_t i = 1; i < 12; ++i) {
		list1.push(to_string(i));
	}
	cout << "List1:\n";
	test0(list1);

	cout << "\nremove с индексом 0 \n";
	list1.remove(0);
	cout << "List1:\n";
	test0(list1);

	cout << "\nremove с индексом 1 \n";
	list1.remove(1);
	cout << "List1:\n";
	test0(list1);
	cout << "\nremove с индексом 2 \n";
	list1.remove(2);
	cout << "List1:\n";
	test0(list1);

	cout << "\nremove с индексом -1 \n";
	list1.remove(-1);
	cout << "List1:\n";
	test0(list1);
	cout << "\nremove с индексом -2 \n";
	list1.remove(-2);
	cout << "List1:\n";
	test0(list1);

	cout << "\nremove с индексом  list1.get_count()-1 \n";
	list1.remove(list1.get_count() - 1);
	cout << "List1:\n";
	test0(list1);

	cout << "\nremove с индексом  list1.get_count() = "<< list1.get_count() << "\n";
	list1.remove(list1.get_count());
	cout << "List1:\n";
	test0(list1);

	cout << "\nremove с индексом  list1.get_count()+1 \n";
	list1.remove(list1.get_count() + 1);
	cout << "List1:\n";
	test0(list1);

	IteratedLoopedList<Patient> list2;
	cout << "\nremove из пустого списка \n";
	list1.remove(0);
	cout << "List2:\n";
	test0(list2);


}

// Работа итераторов
void test9_1() {
	IteratedLoopedList<Patient> list1;
	for (size_t i = 1; i < 12; ++i) {
		list1.push(to_string(i));
	}

	for (auto i = list1.begin(); i != --(list1.begin()); ++i) {
		cout << *i << endl;
	}
	// Вывод последнего элемента
	auto i = --(list1.begin());
	cout << *i << endl;
}

void test9_2() {
	IteratedLoopedList<Patient> list1;
	for (size_t i = 1; i < 12; ++i) {
		list1.push(to_string(i));
	}

	int i = list1.get_count() * 2;
	auto iter = list1.begin();
	while (i > 0) {
		cout << *iter << endl;
		++iter;
		// Если пользоваться постфиксным инкрементом, то возникает ошибка доступа к памяти, не знаю, где именно баг 
		// cout << *(iter++) << endl;
		--i;
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	TWO_SIDES_PRINT_IN_TEST_0 = false; // изменить на true, чтобы также был вывод справа - налево

	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7_1();
	//test7_2();
	//test8();
	//test9_1();
	//test9_2();


	return 0;
}