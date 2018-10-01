#include "comparison.h"
#include "ui_comparison.h"
#include <QPixmap>
#include <QObject>
#include <QScrollBar>

Comparison::Comparison(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Comparison)
{
    ui->setupUi(this);
}

Comparison::~Comparison()
{
    delete ui;
}

void Comparison::set_image(QPixmap mi){
    image = mi;
}

void Comparison::set_comparison_image(QPixmap mci){
    comparison_image = mci;
}

void Comparison::showEvent(QShowEvent *){
    QGraphicsScene *scene;
    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatioByExpanding);

    QGraphicsScene *scene2;
    scene2 = new QGraphicsScene(this);
    scene2->addPixmap(comparison_image);
    scene2->setSceneRect(comparison_image.rect());
    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView_2->fitInView(scene2->sceneRect(),Qt::KeepAspectRatioByExpanding);

    Comparison::connect(ui->graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)), ui->graphicsView_2->horizontalScrollBar(), SLOT(setValue(int)));
    Comparison::connect(ui->graphicsView_2->horizontalScrollBar(), SIGNAL(valueChanged(int)), ui->graphicsView->horizontalScrollBar(), SLOT(setValue(int)));
}
