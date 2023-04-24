#include "crutches.h"

#include <QComboBox>

QString MetaTypeString(int val)
{
    return QString("%1 (%2)").arg(QMetaType::typeName(val)).arg(val);
}

QString BadMetaTypeString(int neededType, int actualType)
{
    return QString("needed %1, got %2")
            .arg(MetaTypeString(neededType))
            .arg(MetaTypeString(actualType));
}

QVariant ComboBoxCurrentData(QComboBox *comboBox)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)) // "This property was introduced in Qt 5.2" © ман QComboBox::currentData
    return comboBox->currentData();
#else
    return comboBox->itemData(comboBox->currentIndex());
#endif
}
