# КМБО-01-20, Гребнев Никита, вариант 5 

# Практическое задание 3

# Номер волны выборки - 19
# Подмножество для пункта 5 - Городские жители, не состоявшие в браке; разведенные женщины, без высшего образования

#Установка необходимых компонентов: 

#install.packages("devtools")
#devtools::install_github("bdemeshev/rlms")

library("lmtest")
library("GGally") 
library(car)  # функция vif()
library("rlms") # Пакет предназначен для работы с данными исследования RLMS
library("dplyr") # Пакет для различных манипуляций с данными
library(sandwich) # Пакет для оценки стандартной ошибки 

data <- rlms_read("r23i_os26c.sav") # Считываю данные исследования для своего варианта

"
sh5 Пол респондента 
1 МУЖСКОЙ 
2 ЖЕНСКИЙ

age Количество полных лет

s_marst СЕМЕЙНОЕ ПОЛОЖЕНИЕ 
1 Никогда в браке не состояли
2 Состоите в зарегистрированном браке
3 Живете вместе, но не зарегистрированы
4 Разведены и в браке не состоите
5 Bдовец (вдова)
6 ОФИЦИАЛЬНО ЗАРЕГИСТРИРОВАНЫ, НО ВМЕСТЕ НЕ ПРОЖИВАЮТ

s_diplom ЗАКОНЧЕННОЕ ОБРАЗОВАНИЕ (ГРУППА) 
1 окончил 0 - 6 классов
2 незаконч среднее образование (7 - 8 кл)
3 незаконч среднее образование (7 - 8 кл) + что-то еще
4 законч среднее образование
5 законч среднее специальное образование
6 законч высшее образование и выше

status ТИП НАСЕЛЕННОГО ПУНКТА 
1 областной центр
2 город
3 ПГТ
4 село

sj13.2 За последние 12 месяцев какова была Ваша среднемесячная зарплата на этом
предприятии после вычета налогов - независимо от того, платят Вам ее вовремя или
нет?

sj6.2 Сколько часов в среднем продолжается Ваша обычная рабочая неделя?
"

data = select(data, sh5, s_age, s_marst, s_diplom, status, sj13.2, sj6.2)
data = na.omit(data)
glimpse(data)

data2 = select(data,) #Новая база данных для нормализованных значений

#Пол
data2["sex"] = 0
data2$sex[which(data$sh5 == 1)] <- 1

#Возраст
age = data$s_age
data2["age"] = (age - mean(age)) / sqrt(var(age))

#Семейное положение:
#Состоит ли в зарегестрированном браке?
data2$wed1 = 0
data2$wed1[which(data$s_marst==2)] <- 1
data2$wed1[which(data$s_marst==6)] <- 1

#Разведён или вдовец?
data2$wed2 = 0
data2$wed2[which(data$s_marst==4)] <- 1
data2$wed2[which(data$s_marst==5)] <- 1

#Никогда не состоял в браке?
data2$wed3 = 0
data2$wed3[which(data$s_marst==1)] <- 1

# Проверим, что отсутствует линейная зависимость между симейными положениями
vif(lm(data$sj13.2 ~ data2$wed1 + data2$wed2 + data2$wed3)) 

#Наличие высшего образования
data2$higher_educ = 0
data2$higher_educ[which(data$s_diplom==6)] <- 1

#Живёт в городе?
data2$city_status = 0
data2$city_status[which(data$status==1)] <- 1
data2$city_status[which(data$status==2)] <- 1

#Нормализованное среднее число рабочих часов в неделю
working_hours = data$sj6.2
data2$working_hours = (working_hours - mean(working_hours)) / sqrt(var(working_hours))

#Нормализованная средняя зарплата
salary = data$sj13.2
data2$salary = (salary - mean(salary)) / sqrt(var(salary))


# 1. Постройте линейную регрессию зарплаты на все параметры, которые Вы выделили из данных мониторинга. Не забудьте оценить коэффициент вздутия дисперсии VIF.

model1 = lm(data = data2, salary ~ sex + age + wed1 + wed2 + wed3 + higher_educ + city_status + working_hours)
vif(model1)
summary(model1)

model1 = lm(data = data2, salary ~ sex + age + wed3 + higher_educ + city_status + working_hours)
vif(model1)
summary(model1)


# 2. Поэкспериментируйте с функциями вещественных параметров: используйте логарифм и степени (хотя бы от 0.1 до 2 с шагом 0.1).

# с логарифмами:
model1 = lm(data = data2, salary ~ sex + working_hours + age + wed3 + higher_educ + city_status + I(log(working_hours)) + I(log(age)))
vif(model1)
summary(model1) 
#R^2 ~ 0.21
# Не самые хорошие p-статистика и vif, увеличился R^2


model1 = lm(data = data2, salary ~ sex + working_hours + age + wed3 + higher_educ + city_status + I(log(age)))
vif(model1)
summary(model1) 
# R^2 ~ 0.179

model1 = lm(data = data2, salary ~ sex + age + wed3 + higher_educ + city_status + I(log(working_hours)) + I(log(age)))
vif(model1)
summary(model1) 
# R^2 ~ 0.2082 

model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(log(working_hours)) + I(log(age)))
vif(model1)
summary(model1) 
# R^2 ~ 0.1924, хорошая p-статистика


#Со степенями:
current_pow = 0.1
model1 = lm(data = data2, salary ~ sex + working_hours + age + wed3 + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.213
# Плохие значения vif

model1 = lm(data = data2, salary ~ sex + wed3 + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.196
# Плохая p-статистика у wed3

model1 = lm(data = data2, salary ~ sex+ higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.196
# Везде хорошая p-статистика


current_pow = 0.2
model1 = lm(data = data2, salary ~ sex + working_hours + age + wed3 + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.213
# Плохие значения vif

model1 = lm(data = data2, salary ~ sex  + age + wed3 + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.21
# Плохие значения vif

model1 = lm(data = data2, salary ~ sex + wed3 + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.199
# Плохая p-статистика у wed3

model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.199
# Везде хорошая p-статистика

current_pow = 0.3
model1 = lm(data = data2, salary ~ sex + working_hours + age + wed3 + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.213
# Плохие значения vif

model1 = lm(data = data2, salary ~ sex + wed3 + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.20
# Плохая p-статистика у wed3

model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.202
# Везде хорошая p-статистика


current_pow = 0.4
model1 = lm(data = data2, salary ~ sex + working_hours + age + wed3 + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.213
# Плохие значения vif


model1 = lm(data = data2, salary ~ sex + wed3 + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.205
# Плохая p-статистика у wed3

model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1) 
# R^2 ~  0.205
# Везде хорошая p-статистика

#Заметим, что с увеличением current_pow немного увеличивается R^2, перейдём сразу к степени 0.9

current_pow = 0.9
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
# R^2 ~  0.2117
# Везде хорошая p-статистика

# Для степени 1 результат мы уже имеем

current_pow = 1.1
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
# R^2 ~  0.2118
# Везде хорошая p-статистика

current_pow = 1.2
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
# R^2 ~  0.2114
# Везде хорошая p-статистика

current_pow = 1.3
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
# R^2 ~  0.2108
# Везде хорошая p-статистика

current_pow = 1.4
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
# R^2 ~  0.21
# Везде хорошая p-статистика

current_pow = 1.5
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
# R^2 ~  0.209
# Везде хорошая p-статистика

# и далее R^2 уменьшается

current_pow = 1.7
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
# R^2 ~  0.206
# Везде хорошая p-статистика

current_pow = 1.9
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
# R^2 ~  0.203
# Везде хорошая p-статистика

current_pow = 2
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
# R^2 ~  0.15
# Плохая p-статистика у I(working_hours^current_pow)



# 3.Выделите наилучшие модели из построенных: по значимости параметров,включённых в зависимости, и по объяснённому с помощью построенных зависимостей разбросу adjusted R2 - R2adj.

# Сравним лучшие модели: 
current_pow = 0.9
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
#Multiple R-squared:  0.2117,	Adjusted R-squared:  0.2061 

current_pow = 1.1
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
#Multiple R-squared:  0.2118,	Adjusted R-squared:  0.2062 

current_pow = 1.2
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
#Multiple R-squared:  0.2114,	Adjusted R-squared:  0.2058

# Разброс R2 - R2_adj везде одинаковый, а R^2 больше для степени 1.1


# Итого, среди моделей без линейной зависимости параметров с хорошими показателями p-статистики у регрессоров лучшей по R^2 оказалась модель для степени 1.1:  
current_pow = 1.1
model1 = lm(data = data2, salary ~ sex + higher_educ + city_status + I(working_hours^current_pow) + I(age^current_pow))
vif(model1)
summary(model1)
# с R^2 ~ 0.2118


# 4. Сделайте вывод о том, какие индивиды получают наибольшую зарплату.

#Согласно этой модели больше всего зарабатывают молодые мужчины с высшим образованием, проживающие в городах, работающие большое число часов в неделю.
#Семейное положение, судя по всему, неважно


# 5. Оцените регрессии для подмножества индивидов: а) Городские жители, не состоявшие в браке; б )разведенные женщины, без высшего образования
current_pow = 1.1

#Городские жители, не состоявшие в браке
data3 = subset(data2, city_status  == 1)
data3 = subset(data3, wed3 == 1) 

model1 = lm(data = data3, salary ~ sex + higher_educ + I(working_hours^current_pow) + I(age^current_pow))
summary(model1)
# Параметры, кроме higher_educ - малозначимые. R^2 ~ 0.5189
# Наибольшая зарплата у мужчин с высшим образованием старшего возраста, работающих много

#разведенные женщины, без высшего образования
data3 = subset(data2, sex == 0)
data3 = subset(data3, wed2 == 1) 
data3 = subset(data3, higher_educ == 0)
model1 = lm(data = data3, salary ~ city_status + I(working_hours^current_pow) + I(age^current_pow))
summary(model1)
# почти все параметры значимые, R^2 ~ 0.2376
# Наибольшая зарплата у проживающих в городе, работающих много, молодого возраста.



