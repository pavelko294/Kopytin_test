#ifndef CRUTCHES_TPP
#define CRUTCHES_TPP

#include "crutches.h"

template<typename T>
T ExtractFromVar(const QVariant &var)
{
    Q_ASSERT(var.isValid());

    const int neededType {qMetaTypeId<T>()};
    const int actualType {var.userType()};
    if(neededType != actualType)
        qFatal(qPrintable(BadMetaTypeString(neededType, actualType)));

    Q_ASSERT(var.canConvert<T>());
    return var.value<T>();
}

template<typename T>
QList<T> ToList(const QSet<T> &set)
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
    return set.toList();
#else // Note: Since Qt 5.14, range constructors are available for Qt's generic container classes and should be used in place of this method. © ман QSet::toList
    return QList<T>(set.begin(), set.end());
#endif
}

#endif // CRUTCHES_TPP
