#КМБО-01-20, Гребнев Никита, вариант 5 

#практическое задание  1
library("lmtest")
library("GGally")

data = swiss
#выводим данные
data

#для многострочных комментариев будут использоваться кавычки ""
"Пункт 1. Оцените среднее значение, дисперсию и СКО переменных, указанных во втором и 
третьем столбце (Agriculture и Examination)"
#среднее значение:
print(paste("среднее значение Agriculture =",  mean(data$Agriculture)))
print(paste("среднее значение Examination =",  mean(data$Examination)))
#дисперсия
print(paste("дисперсия Agriculture =",  var(data$Agriculture)))
print(paste("дисперсия Examination =",  var(data$Examination)))
#СКО
print(paste("СКО Agriculture =",  sd(data$Agriculture)))
print(paste("СКО Examination =",  sd(data$Examination)))

"Пункт 2. Постройте зависимости вида y = a + bx, где y – объясняемая переменная, x –
регрессор."
model1 = lm(Agriculture~Education, data)
model2 = lm(Agriculture~Catholic, data)

model1 # y = 67.243 - 1.511x
model2 # y = 41.6728 + 0.2184x

"Пункт 3. Оцените, насколько «хороша» модель по коэффициенту детерминации R^2"
summary(model1) #R^2 = 0.409. R^2 относительно низкий, модель плохая. (Модель объясняет 40% колебаний Agriculture)
summary(model2) #R^2 = 0.1609 - Так как R^2 очень низкий, модель очень плоха. (Модель объясняет только 16% колебаний Agriculture)

"Пункт 4. Оцените, есть ли взаимосвязь между объясняемой переменной и объясняющей
переменной"
# В model1 у параметров по 3 звёздочки. Взаимосвязть x и y очень сильная.
# В model2 у параметров 2 и 3 звёздочки. Взаимосвясь достаточно сильная.


"Вывод"
# В model1 причинно-следственная связь между поведением переменных есть, но она нелинейная и/или требует дополнительных регрессоров
# В model2 причинно-следственная связь между поведением переменных есть, но она нелинейная и/или требует дополнительных регрессоров

