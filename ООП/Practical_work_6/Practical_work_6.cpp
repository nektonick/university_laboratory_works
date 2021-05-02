// КМБО-01-20, Гребнев Никита, 5 вариант.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


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
	Date(int d=1, int m=1, int y=2000) {
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
		//cout << __FUNCTION__ << endl;
	}

	Patient(const Patient& p) {
		cout << __FUNCTION__ << endl;
		surname = p.surname;
		name = p.name;
		date_of_birth = p.date_of_birth;
		phone_number = p.phone_number;
		address = p.address;
		сard_number = p.сard_number;
		blood_type = p.blood_type;
	}
	Patient& operator=(const Patient& p) {
		//cout << __FUNCTION__ << endl;
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
		s << "Фамилия: " << p.surname << "\nИмя: " << p.name << "\nДата рождения: " << p.date_of_birth << "\nНомер телефона: " << p.phone_number
			<< "\nДом. адресс: " << p.address << "\nНомер мед. карты: " << p.сard_number << "\nГруппа крови: " << p.blood_type;
		
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
		iss3>> p.blood_type;
	}
	else {
		cout << "Bad Readed:\n" << p << endl;
		s >> p.surname >> p.name >> p.date_of_birth >> p.phone_number >> p.address >> p.сard_number >> p.blood_type;
	}
	
	return s;
}


/*
Шаблонный класс элемента связного списка.
В моём варианте поле info будет иметь тип Patient.
*/
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
	/*Element(T data)
	{
		next = prev = NULL;
		info = data;
	}*/

	Element(const T& data)
	{
		//cout << __FUNCTION__ << endl;
		next = prev = NULL;
		info = data;
	}

	Element(Element* Next, Element* Prev, T data)
	{
		//cout << __FUNCTION__ << endl;
		next = Next;
		prev = Prev;
		info = data;
	}

	Element(const Element& el)
	{
		//cout << __FUNCTION__ << endl;
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
ostream& operator<<(ostream& s, const Element<T1>& el)
{
	s << el.info;
	return s;
}

template<class T1>
istream& operator>>(istream& s, Element<T1>& el)
{
	s >> el.info;
	return s;
}


/*
Исключение неверного индекса
*/
class IndexOutOfRangeException : public exception {
protected:
	string error_info;
public:
	IndexOutOfRangeException(string in) : exception() {
		error_info = in;
	}
	void print() const {
		cout << this->error_info << endl;
	}
};


/*
Шаблонный класс связного списка
*/
template<class T>
class LinkedList
{
protected:
	Element<T>* head = nullptr;
	Element<T>* tail = nullptr;
	int count = 0;
public:

	LinkedList()
	{
		//cout << __FUNCTION__ << endl;
		head = tail = nullptr;
		count = 0;
	}

	//LinkedList(T* arr, int len){}

	virtual ~LinkedList()
	{
		cout << __FUNCTION__ << endl;
		Element<T>* cur = this->head;
		while (cur != nullptr) {
			Element<T>* to_delete = cur;
			cur = cur->next;
			delete to_delete;
		}
		this->head = nullptr;
		this->count = 0;
		this->head = nullptr;
	}

	virtual Element<T>* pop() = 0;
	virtual Element<T>* push(const T& value) = 0;

	// Рекурсивное получение i-го элемента связного списка
	Element<T>* operator()(int index, int cur_index = 0, Element<T>* p = nullptr) {
		if (cur_index == 0 && p == nullptr) {
			p = this->head;
		}
		if (p != nullptr && cur_index < this->count && index >= cur_index) {
			if (cur_index == index) {
				return p;
			}
			else {

				return operator()(index, cur_index + 1, p->next);
			}
		}
		else {
			return nullptr;
		}
	}

	Element<T>& operator[](int index)
	{
		if (index < 0 || index >= count) {
			throw IndexOutOfRangeException(string("index = " + to_string(index) + ", count of elements = " + to_string(count)));
		}

		Element<T>* current = head;

		for (int i = 0;
			current != NULL && i < index;
			current = current->next, i++);

		return *current;
	}

	virtual bool isEmpty() const { 
		return (this->count == 0); 
	}

	virtual int get_count() {
		return this->count;
	}
	virtual Element<T>* get_head() {
		return this->head;
	}
	virtual Element<T>* get_tail() {
		return this->tail;
	}

	template<class T1>
	friend ostream& operator<<(ostream& s, const LinkedList<T1>& el);
	template<class T1>
	friend istream& operator>>(istream& s, const LinkedList<T1>& el);
};

template<class T1>
ostream& operator<<(ostream& s, const LinkedList<T1>& el)
{
	Element<T1>* current;
	for (current = el.head; current != NULL; current = current->next)
		s << *current << " ";
	return s;
}

template<class T1>
istream& operator>>(istream& s, const LinkedList<T1>& el)
{
	for (Element<T1>* current = el.head; current != NULL; current = current->next)
		s >> *current;
	return s;
}


/*
Добавление: в конец (стек)
Удаление: с конца (стек)
*/
template <typename T1>
class Stack : public LinkedList<T1> {
public:
	Stack() : LinkedList<T1>() {
		//cout << __FUNCTION__ << endl;
	}

	virtual ~Stack() {
		cout << __FUNCTION__ <<endl;

		Element<T1>* cur = this->head;
		while (cur != nullptr) {
			Element<T1>* to_delete = cur;
			cur = cur->next;
			delete to_delete;
		}
		this->head = nullptr;
		this->count = 0;
		this->head = nullptr;
	}

	// В базовом классе не было конструктора копий
	Stack(const Stack& in) {
		//cout << __FUNCTION__ << endl;
		this->count = 0;
		Element<T1>* cur_in = in.head;
		Element<T1>* cur_this = nullptr;
		while (cur_in != nullptr) {

			if (cur_this == nullptr) {
				cur_this = new Element<T1>(*cur_in);
				this->head = cur_this;
				cur_in = cur_in->next;
			}
			else {
				cur_this->next = new Element<T1>(*cur_in);
				cur_this->next->prev = cur_this;
				cur_this = cur_this->next;
				cur_in = cur_in->next;
			}
			++(this->count);
		}
		this->tail = cur_this;
	}

	Stack& operator=(const Stack& in) {
		//cout << __FUNCTION__ << endl;
		this->clear();
		this->count = 0;
		Element<T1>* cur_in = in.head;
		Element<T1>* cur_this = nullptr;
		while (cur_in != nullptr) {

			if (cur_this == nullptr) {
				cur_this = new Element<T1>(*cur_in);
				this->head = cur_this;
				cur_in = cur_in->next;
			}
			else {
				cur_this->next = new Element<T1>(*cur_in);
				cur_this->next->prev = cur_this;
				cur_this = cur_this->next;
				cur_in = cur_in->next;
			}
			++(this->count);
		}
		this->tail = cur_this;
		return *this;
	}


	// Удаляет последний элемент, НЕ очищая память из-под него. Возвращает указатель на него.
	Element<T1>* pop() {
		if (this->isEmpty()) {
			return nullptr;
		}

		Element<T1>* ans = this->tail;

		if (this->count == 1) {
			this->head = nullptr;
			this->tail = nullptr;
		}
		else {
			this->tail = this->tail->prev;
			this->tail->next = nullptr;
		}
		--(this->count);
		ans->prev = nullptr;
		return ans;

	}

	// Добавление элемента в конец
	Element<T1>* push(const T1& value) {
		if (this->isEmpty()) {
			this->head = new Element<T1>(value);
			this->tail = this->head;
			this->head->next = nullptr;
			this->head->prev = nullptr;
		}
		else {
			this->tail->next = new Element<T1>(value);
			this->tail->next->prev = this->tail;
			this->tail = this->tail->next;
		}
		++(this->count);
		return this->tail;
	}

	// Вставка на i-ое место
	Element<T1>* insert(const T1& value, int index) {
		if (index < 0) {
			throw IndexOutOfRangeException("Index < 0");
		}
		else if (index <= this->count) {
			Element<T1>* cur = this->head;
			int cur_num = 0;

			while (cur_num < index) {
				cur = cur->next;
				++cur_num;
			}

			// Вставка в начало
			if (cur == this->head) {
				Element<T1>* old = this->head;
				this->head = new Element<T1>(value);
				this->head->next = old;
				old->prev = this->head;
				++(this->count);
				return this->head;
			}
			// Вставка между началом и концом
			else if (cur != nullptr) {
				Element<T1>* old_prev = cur->prev;

				cur->prev = new Element<T1>(value);
				cur->prev->next= cur;
				cur->prev->prev = old_prev;
				old_prev->next = cur->prev;
				++(this->count);
				return cur->prev;
			}
			// Вставка после хвоста
			else {
				this->tail->next = new Element<T1>(value);
				this->tail->next->prev = this->tail;
				this->tail = this->tail->next;
				++(this->count);
				return this->tail;
			}
		}
		else {
			throw IndexOutOfRangeException("Index > len(Stack)");
		}
		
	}

	// Удаляет i-ый элемент, очищая память
	Element<T1> remove(int index) {
		if (index < 0) {
			throw IndexOutOfRangeException("Index < 0");
		}
		else if (index < this->count) {
			Element<T1>* cur = this->head;
			int cur_num = 0;

			while (cur_num != index) {
				cur = cur->next;
				++cur_num;
			}
			Element<T1>* c_prev = cur->prev;
			Element<T1>* c_next = cur->next;

			if (cur == this->head) {
				this->head = this->head->next;
			}
			if (cur == this->tail) {
				this->tail = this->tail->prev;
			}

			Element<T1> ans(*cur);
			ans.next = nullptr; ans.prev = nullptr;

			delete cur; cur = nullptr;

			if (c_prev != nullptr)
				c_prev->next = c_next;
			if (c_next != nullptr)
				c_next->prev = c_prev;

			--(this->count);
			return ans;

		}
		else {
			throw IndexOutOfRangeException("Index > tail index");
		}
	}

	// Удаляет все элементы, очищая память
	void clear() {
		while (!this->isEmpty()) {
			Element<T1>* ptr = this->pop();
			delete ptr;
		}
	}

	// Удаляет все элементы, очищая память, и печатая их
	void print_and_clear() {
		while (!this->isEmpty()) {
			Element<T1>* ptr = this->pop();
			cout << ptr->info << "\n\n";
			delete ptr;
		}
	}

	// поиск по фамилии, фильтр по группе крови через фильтр, принимающий функцию
	template<typename T2>
	Stack filter(bool (*criterion)(const T1& value_to_comparison, const T2 param), const T2 param_for_search_in_value) {
		Stack<T1> ans;
		Element<T1>* cur = this->head;
		while (cur != nullptr) {
			if (criterion(cur->info, param_for_search_in_value)) {
				ans.push(cur->info);
			}
			cur = cur->next;
		}
		return ans;
	}

	// Рекурсивная реализация функции фильтра
	template<typename T2>
	Stack<T1>* filter_recursive(bool (*criterion)(const T1& value_to_comparison, const T2 param), const T2 param_for_search_in_value,
						        Element<T1>* start_from = nullptr, Stack<T1>* stack_for_ans = nullptr) {
		
		// Инициализация если последние 2 параметра не передали
		if (stack_for_ans == nullptr && start_from == nullptr) {
			start_from = this->head;
			stack_for_ans = new Stack<T1>;
		}

		// Инициализация, если не дали только последний параметр
		if (stack_for_ans == nullptr) {
			stack_for_ans = new Stack<T1>;
		}

		// База рекурсии
		if (start_from == nullptr) {
			return stack_for_ans;
		}

		// Добавление элемента
		if (criterion(start_from->info, param_for_search_in_value)) {
			stack_for_ans->push(start_from->info);
		}
		
		return filter_recursive(criterion, param_for_search_in_value, start_from->next, stack_for_ans);
	}

	void save_to(ofstream& s) {
		s << this->count << endl;
		Element<T1>* cur =  this->head;
		for (int i = 0; i < this->count; ++i) {
			s << (*cur) << endl;
			cur = cur->next;
		}
	}

	void load_from(ifstream& s) {
		this->clear();
		
		int count_of_elements;
		string line;
		getline(s, line);
		std::istringstream iss(line);
		iss >> count_of_elements;
		for (int i = 0; i < count_of_elements; ++i) {
			T1 cur;
			s >> cur;
			this->push(cur);
		}

	}
};

// Функция проверки равенства имён
bool surname_search(const Patient& p, const string param_for_search_in_value = "") {
	if (p.surname == param_for_search_in_value) {
		return true;
	}
	else {
		return false;
	}
}

// Функция, проверяющая что группа крови <= той, что переданна в параметре
bool blood_type_filter(const Patient& p, int type = 1) {
	if (p.blood_type <= type) {
		return true;
	}
	else {
		return false;
	}
}


void set_hex_manip(ostream& f) {
	/*f.unsetf(ios::dec);
	f.setf(ios::hex);
	f.setf(ios::uppercase);*/
	//Аналогично:
	f << hex << uppercase;
}


int main()
{
	setlocale(LC_ALL, "Russian");

	try
	{
		Stack<Patient> s;
		s.push(Patient());
		s.push(Patient("Сидоров"));
		s.push(Patient("Козлов"));

		s.remove(1);


		s.insert(Patient("Insert"), 2);

		
		Stack<Patient>* s2 = s.filter_recursive(surname_search, string("Иванов"));
		Stack<Patient> s3 = s.filter(surname_search, string("Иванов"));


		cout << "\n~~~Изначальный список~~~\n";
		s.print_and_clear();
		cout << "\n~~~Рекурсивно отфильтрованный список s2~~~\n";
		s2->print_and_clear();
		cout << "\n~~~Отфильтрованный список s3~~~\n";
		s3.print_and_clear();

		cout << "\n~~~Изначальный список~~~\n";
		s.push(Patient("Сидоров"));
		s.push(Patient("АААА","FFF", Date(), "1", "", 100, 3));
		s3 = s.filter(blood_type_filter, 2);
		s.print_and_clear();
		cout << "\n~~~Отфильтрованный список~~~\n";
		s3.print_and_clear();

	}
	catch (const IndexOutOfRangeException& e)
	{
		e.print();
	}

	cout << "\n~~~No cast~~~\n";
	LinkedList<Patient>* base_class_ptr = new Stack<Patient>;	
	delete base_class_ptr;

	cout << "\n~~~dynamic_cast~~~\n";
	LinkedList<Patient>* ptr_to_cast = new Stack<Patient>;
	Stack<Patient> *casted = dynamic_cast<Stack<Patient>*>(ptr_to_cast);
	delete casted;


	Stack<Patient> s_to_file;
	Stack<Patient> s_from_file;
	s_to_file.push(Patient()); s_to_file.push(Patient("АААА", "FFF", Date(), "1", "", 100, 3)); s_to_file.push(Patient());
	
	string file_name = "test1.txt";
	ofstream fout(file_name);

	if (fout.is_open()) {
		s_to_file.save_to(fout);
		fout.close();
	}
	else {
		std::cout << "file not opened\n";
	}
	ifstream fin(file_name);

	if (fin.is_open()) {
		s_from_file.load_from(fin);
		fin.close();
	}
	else {
		std::cout << "file not opened\n";
	}
	cout<< "\n~~~Readed from file~~~\n";
	s_from_file.print_and_clear();
	
	set_hex_manip(cout);
	Stack<Patient> s4;
	s4.push(Patient("Логинов", "Майк", Date(10,9,1990), "8-900-900-99-99", "Северный полюс 2.", 14, 3)); 
	s4.print_and_clear();

	return 0;
}