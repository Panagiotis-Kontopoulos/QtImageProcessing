#include "oddspinbox.h"

OddSpinBox::OddSpinBox(QWidget *p): QSpinBox(p) {
    setMinimum(1);
    setSingleStep(2);
}

int OddSpinBox::valueFromText(const QString &text) const {
    int value = text.toInt();
    if (value % 2 == 0)
        --value;
    value = qMax(value, minimum());
    value = qMin(value, maximum());
    return value;
}
