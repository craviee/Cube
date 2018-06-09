#include "cameraarea.h"
#include "ui_cameraarea.h"

cameraArea::cameraArea(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cameraArea)
{
    ui->setupUi(this);
}

cameraArea::~cameraArea()
{
    delete ui;
}
