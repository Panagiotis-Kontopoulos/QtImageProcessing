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

    void set_similarity_label(QString similarity);

private:
    Ui::Comparison *ui;
    QPixmap image, comparison_image;
    QString similarity_label;
};

#endif // COMPARISON_H
