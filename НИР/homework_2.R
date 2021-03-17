# КМБО-01-20, Гребнев Никита, вариант 5 

# Практическое задание  2

library("lmtest")
library("GGally") 
library("car")  # без этого не работает функция vif()

# При чтении избавляемся от записей с недостающими данными.
data = na.omit(swiss)

# Выводим данные
data

# Infant.Mortality ~ Fertility, Catholic, Agriculture

# 1. Проверим отсутствие зависимости между регрессорами перед построением модели
pairwise_comparison_1 = lm(Fertility~Catholic, data)
summary(pairwise_comparison_1) # R^2 < 25% - зависимости нет

pairwise_comparison_1 = lm(Fertility~Agriculture, data)
summary(pairwise_comparison_1) # R^2 < 15% - зависимости нет

pairwise_comparison_1 = lm(Catholic~Agriculture, data)
summary(pairwise_comparison_1) # R^2 < 20% - зависимости нет

# Можно использовать регрессоры вместе



# 2. Построим линейную модель и оценим её
model = lm(Infant.Mortality ~ Fertility + Catholic + Agriculture, data)
summary(model) 
# R^2 ~ 0.22, p-значение у Catholic и Agriculture высокие (мало звёздочек) - модель плохая по обоим параметрам.

#Уберём из модели регрессор Catholic, как наименее значимый, и проверим, как изменится R^2 
model = lm(Infant.Mortality ~ Fertility + Agriculture, data)
summary(model) 
# R^2 ~ 0.22 - R^2 практически не изменился

# Попробуем убрать ещё один регрессор
model = lm(Infant.Mortality ~ Fertility, data)
summary(model) #R^2 ~ 0.17 - изменился сильно, регрессор Agriculture лучше не убирать


# В дальнейшем будем работать с моделью:
model = lm(Infant.Mortality ~ Fertility + Agriculture, data)


# 3. Попробуем ввести в модель логарифмы регрессоров, предварительно проверяя, что нет линейной зависимости


model = lm(I(log(Infant.Mortality)) ~ I(log(Fertility)) + I(log(Agriculture)), data)     
vif(model) # линейной зависимости нет.
summary(model) # R^2 ~ 0.17, p-статистика хорошая для I(log(Fertility))

model = lm(Infant.Mortality ~ I(log(Fertility)) + I(log(Agriculture)), data)     
vif(model) # линейной зависимости нет.
summary(model) # R^2 ~ 0.19, p-статистика хорошая для I(log(Fertility))

model = lm(Infant.Mortality ~ I(log(Fertility)) + Agriculture, data)     
vif(model) # линейной зависимости нет.
summary(model) # R^2 ~ 0.21, p-статистика хорошая для I(log(Fertility))

model = lm(Infant.Mortality ~ Fertility + I(log(Agriculture)), data)     
vif(model) # линейной зависимости нет.
summary(model) # R^2 ~ 0.19, p-статистика хорошая для I(log(Fertility))


# Наилучшей из них будет следующая модель: 
model = lm(Infant.Mortality ~ I(log(Fertility)) + Agriculture, data)  # R^2 ~ 0.21
# но при этом модели с логарифмами имею R^2 меньше, чем у модель без логарифмов


# 4. Попробуем ввести в модель всевозможные произведения пар регрессоров, предварительно проверяя, что нет линейной зависимости

model = lm(Infant.Mortality ~ Fertility + Agriculture + I(Fertility^2) + I(Agriculture^2) + I(Fertility*Agriculture), data) 
vif(model) # есть линейная зависимость, уберём регрессоры с максимальным VIF

model = lm(Infant.Mortality ~ Fertility + Agriculture + I(Agriculture^2) + I(Fertility*Agriculture), data) 
vif(model) # есть линейная зависимость, уберём регрессоры с максимальным VIF

model = lm(Infant.Mortality ~ Fertility + Agriculture + I(Agriculture^2), data) 
vif(model) # есть линейная зависимость, уберём регрессоры с максимальным VIF

model = lm(Infant.Mortality ~ Fertility + I(Agriculture^2), data) 
vif(model) # линейной зависимости нет
summary(model) # R^2 ~ 0.24, p-статистика хорошая для Fertility и неплохая для Agriculture^2


#Наилучшая модель:
model = lm(Infant.Mortality ~ Fertility + I(Agriculture^2), data) 













