# КМБО-01-20, Гребнев Никита, вариант 5 

# Практическое задание  2.2

library("lmtest")
library("GGally") 
library("car")

data = na.omit(swiss)
# модель из практического задания 2:
model = lm(Infant.Mortality ~ Fertility + Catholic + Agriculture, data) 
summary(model) 

# 1. Оценим доверительные интервалы для всех коэффициентов в модели (для p=95%)

# Число степеней свободы в моделе df =  43 - 4 = 39
t_critical = qt(0.975, df = 39) #t-критерий Стьюдента - общий для всех коэффициентов

# Стандартные ошибки коэффициентов (взяты из summary(model)):
Std_Error_Intercept = 2.3629
Std_Error_Fertility = 0.0361
Std_Error_Catholic = 0.0110
Std_Error_Agriculture = 0.0192

# Выведем соответствующие доверительные интервалы:
print(paste("Доверительный интервал свободного коэффициента: [", model$coefficients[1] - t_critical * Std_Error_Intercept, 
            ",", model$coefficients[1] + t_critical * Std_Error_Intercept, "]"))

print(paste("Доверительный интервал Fertility: [", model$coefficients[2] - t_critical * Std_Error_Fertility, 
            ",", model$coefficients[2] + t_critical * Std_Error_Fertility, "]"))

print(paste("Доверительный интервал Catholic: [", model$coefficients[3] - t_critical * Std_Error_Catholic, 
            ",", model$coefficients[3] + t_critical * Std_Error_Catholic, "]"))

print(paste("Доверительный интервал Agriculture: [", model$coefficients[4] - t_critical * Std_Error_Agriculture, 
            ",", model$coefficients[4] + t_critical * Std_Error_Agriculture, "]"))

  
# 2. Вывод о отвержении или невозможности отвергнуть статистическую гипотезу о том, что коэффициент равен 0:

# Доверительный интервал свободного коэффициента: [ 8.7855926699545 , 18.3444254198642 ] 
# 0 не попадает в доверительный интервал -> отвергаем статистическую гипотезу о том, что коэффициент равен 0

# Доверительный интервал Fertility: [ 0.0390550318940447 , 0.185093316320699 ]
# 0 не попадает в доверительный интервал -> отвергаем статистическую гипотезу о том, что коэффициент равен 0

# Доверительный интервал Catholic: [ -0.0184955828737981 , 0.0260036173670106 ]
# 0 попадает в доверительный интервал -> невозможно отвергнуть статистическую гипотезу о том, что коэффициент равен 0

# Доверительный интервал Agriculture: [ -0.0711707027421142 , 0.0065006285872974 ]
# 0 попадает в доверительный интервал -> невозможно отвергнуть статистическую гипотезу о том, что коэффициент равен 0


# 3. Доверительный интервал для одного прогноза (p = 95%, Fertility = 20, Catholic = 10, Agriculture = 10).

new.data = data.frame(Fertility = 20, Catholic = 10, Agriculture = 10)
predict(model, new.data, interval = "confidence")

# Доверительный интервал: [12.05615, 18.98521]









