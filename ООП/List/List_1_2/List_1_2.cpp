// КМБО-01-20, Гребнев Никита, 5 вариант.

#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class Doctor {
public:
	Doctor(string input_surname = "no surname", string  input_name= "no name", string input_specialty = "no specialty", string  input_post= "no post", double  input_experience = 0, double  input_rating = 0) :
		surname(input_surname), name(input_name), specialty(input_specialty), post(input_post), experience(input_experience), rating(input_rating) {};
	virtual void printInfo() const {
		cout<< "Доктор-" << specialty << " " << surname << " " << name
			<< ", занимает должность " << post << ", имеет стаж " << experience << " лет и рейтинг " << rating << " баллов\n";
	}
	virtual ~Doctor() {

	}
protected:
	string surname; // фамилия
	string name; // имя
	string specialty; // специальность
	string post; // должность
	double experience; // стаж
	double rating; // рейтинг
};

class Element : public Doctor{ //элемент связного списка
	
private:
	Element* next;
	Element* prev;
public:
	virtual ~Element() {

	}
	Element(string input_surname = "no surname", string  input_name = "no name", string input_specialty = "no specialty", string  input_post = "no post", double  input_experience = 0, double  input_rating = 0) :
		Doctor(input_surname, input_name, input_specialty, input_post, input_experience, input_rating), next(nullptr), prev(nullptr) {};

	virtual Element* getNext() { return next; }
	virtual Element* getPrevious() { return prev; }

	virtual void setPrevious(Element* value) { prev = value; }
	virtual void setNext(Element* value) { next = value; }
	bool operator<(Element el) {
		if (rating > el.rating)
			return false;
		else
			if (rating == el.rating && experience > el.experience)
				return false;
			else
				if (rating == el.rating && experience == el.experience && surname > el.surname)
					return false;
				else
					if (rating == el.rating && experience == el.experience && surname == el.surname && name > el.name)
						return false;
		return true;
	}

	friend ostream& operator << (ostream& out, const Element& obj);
};


ostream& operator<< (ostream& out, const Element& obj)
{
	out << "Доктор-" << obj.specialty << " " << obj.surname << " " << obj.name 
		<< ", занимает должность " << obj.post << ", имеет стаж " << obj.experience << " лет и рейтинг " << obj.rating << " баллов";
	return out;
}

class LinkedListParent {
protected:
	//достаточно хранить начало и конец
	Element* begin;
	Element* end;
	//для удобства храним количество элементов
	int num;
public:
	virtual int getNumOfElements() const { return num; }

	virtual Element* getBegin() const { return begin; }

	virtual Element* getEnd() const{ return end; }

	LinkedListParent()
	{
		cout << "Parent constructor\n";
		begin = nullptr;
		end = nullptr;
		num = 0;
	}

	virtual ~LinkedListParent() {
		Element* cur = begin;
		Element* next = (begin == nullptr) ? nullptr : begin->getNext();
		while (next != nullptr) {
			delete cur;
			cur = next;
			next = next->getNext();
		}
		cout << "Parent destructor\n";
	}

	
	Element* push(Element& el) {
		Element* newEl = new Element(el);
		if (num == 0) {
			begin = newEl;
			end = newEl;
		}
		else {
			if (num == 1) { // 1 элемент в списке => вставка либо в конец, либо в начало
				if ((*begin) <(*newEl)) { //	вставка в конец
					begin->setNext(newEl);
					newEl->setPrevious(begin);
					end = newEl;
				}
				else { //	вставка в начало
					newEl->setNext(begin);
					begin->setPrevious(newEl);
					begin = newEl;
					end = newEl->getNext();
				}
			}
			else { // больше 1 элемента в списке
				if ((*newEl)<(*begin) ) {
					newEl->setNext(begin);
					begin->setPrevious(newEl);
					begin = newEl;
				}
				else {
					if ((*end)<(*newEl)) {
						newEl->setPrevious(end);
						end->setNext(newEl);
						end = newEl;
					}
					else {
						Element* cur = begin;
						while (cur->getNext() != nullptr && (*cur)<(*newEl)) {
							if ((*(cur->getNext())) <(*newEl)) {
								cur = cur->getNext();
							}
							else {
								break;
							}
						}
						// здесь newEl > cur && newEl < cur->getNext()
						newEl->setNext(cur->getNext());
						cur->getNext()->setPrevious(newEl);
						newEl->setPrevious(cur);
						cur->setNext(newEl);
						if (newEl->getNext() == nullptr) {
							end = newEl;
						}
						if (newEl->getPrevious() == nullptr) {
							begin == newEl;
						}
					}
				}
			}
		}
		++num;
		return newEl;
	}

	// Функция возвращет ссылку на объект, расположенный в динамической памяти (куче), убирает ссылку на текущий объект из связного списка
	Element* pop() { 
		if (num > 0) {
			Element* ans = end;
			if (num > 1) {
				end = end->getPrevious();
				//delete end->getNext();
				end->setNext(nullptr);
			}
			else {
				//delete begin;
				begin = nullptr;
				end = nullptr;
			}
			--num;
			return ans;
		}
		else {
			throw exception("no elements");
		}
	}

	Element* operator[](int i) const {
		//индексация
		if (i < 0 || i > num) return nullptr;
		int k = 0;

		//ищем i-й элемент - вставем в начало и отсчитываем i шагов вперед
		Element* cur = begin;
		for (k = 0; k < i; k++)
		{
			cur = cur->getNext();
		}
		return cur;
	}
	void print()const {
		if (this->num > 0) {
			Element* cur = begin;
			cout << "Elements:\n";
			cur->printInfo();
			while (cur->getNext() != nullptr) {
				cur = cur->getNext();
				cur->printInfo();
			}
		}
		else {
			cout << "no elements\n";
		}
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");
	// сортировка в порядке возрастания в именах и фамилиях происходит от A -> Z 

	Element el1("not a test", "Ivan", "LOR", "Main doctor", 3, 55);
	Element el2("test", "test", "test", "test", 3, 55);
	Element el3("test", "test", "test", "test", 1, 101);
	Element el4("BBB");
	Element el5("DDD");
	Element el6("ABB");
	LinkedListParent list;

	list.push(el1);
	list.push(el2);
	list.push(el3);
	list.push(el4);
	list.push(el5);
	list.push(el6);

	list.print();

	cout << "Deleting elements strart from top:\n";
	Element* ptr = nullptr;
	while (list.getNumOfElements() > 0) {
		ptr = list.pop();
		ptr->printInfo();
		delete ptr;
	}
	cout << "List now:\n";
	list.print();	
	
	return EXIT_SUCCESS;
}
