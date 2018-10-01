#ifndef COMPARISON_H
#define COMPARISON_H

#include <QWidget>

namespace Ui {
class Comparison;
}

class Comparison : public QWidget
{
    Q_OBJECT

protected:
    void showEvent(QShowEvent *);

public:
    explicit Comparison(QWidget *parent = nullptr);
    ~Comparison();

    void set_image(QPixmap mi);

    void set_comparison_image(QPixmap mci);


private:
    Ui::Comparison *ui;
    QPixmap image, comparison_image;
};

#endif // COMPARISON_H
