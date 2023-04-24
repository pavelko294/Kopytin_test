#ifndef CRUTCHES_H
#define CRUTCHES_H

#include <QVariant>

class QComboBox;

// Вспомогательное барахло

// Имена метатипов
QString MetaTypeString(int val);
QString BadMetaTypeString(int neededType, int actualType);

// Вариант, привязанный к текущему элементу comboBox
QVariant ComboBoxCurrentData(QComboBox *comboBox);

// Извлечь значение из варианта (с проверками)
template<typename T>
T ExtractFromVar(const QVariant &var);

// Костыль для Qt 5.14+
template<typename T>
QList<T> ToList(const QSet<T> &set);

#include "crutches.tpp"

#endif // CRUTCHES_H
