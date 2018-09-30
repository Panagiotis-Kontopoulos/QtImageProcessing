#ifndef ODDSPINBOX_H
#define ODDSPINBOX_H

#include <QMainWindow>
#include <QSpinBox>

class OddSpinBox: public QSpinBox {
    Q_OBJECT
public:
    OddSpinBox(QWidget *p);
protected:
    int valueFromText(const QString &text) const;
};

#endif // ODDSPINBOX_H
