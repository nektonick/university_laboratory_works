// КМБО-01-20, Гребнев Никита, 5 вариант.
#include <iostream>
#include <string>


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
	friend ostream& operator<<(ostream& s, Date d);
	friend ostream& operator>>(ostream& s, Date d);
};
ostream& operator<<(ostream& s, Date d) {
	s << d.day << "." << d.mounth << "." << d.year;
}
ostream& operator>>(ostream& s, Date d) {
	s >> d.day >> d.mounth >> d.year;
}

/*
«Пациент».
Поля: фамилия, имя, дата рождения, телефон, адрес, номер карты, группа крови.
*/
class Patient {
protected:
	string surname;
	string name;
	Date date_of_birth;
	string phone_number; // формат номера неизвестен, поэтому string, а не int
	string address;
	int сard_number;
	int blood_type;
public:
	Patient(string i_surname = "Иванов", string i_name = "Иван", Date i_date_of_birth = Date(), string i_phone_number = "+7-000-000-00-00",
		    string i_address = "ул. Большая Лубянка, 2, Москва, 107031", int i_сard_number = 0, int i_blood_type = 1) {
		surname = i_surname;
		name = i_name;
		date_of_birth = i_date_of_birth;
		phone_number = i_phone_number;
		address = i_address;
		сard_number = i_сard_number;
		blood_type = i_blood_type;
	}
	friend ostream& operator<<(ostream& s, Patient p);
	friend ostream& operator>>(ostream& s, Patient p);
};

ostream& operator<<(ostream& s, Patient p) {
	s << "Фамилия:" << p.surname << " Имя:" << p.name << " Дата рождения:" << p.date_of_birth << " Номер телефона:" << p.phone_number 
		<< " Дом. адресс:" << p.address << " Номер мед. карты:" << p.сard_number << " Группа крови:" << p.blood_type;
}

ostream& operator>>(ostream& s, Patient p) {
	s >> p.surname >> p.name >> p.date_of_birth >> p.phone_number >> p.address >> p.сard_number >> p.blood_type;
}