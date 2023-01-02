#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
  ui->setupUi(this);
  m0001 = new M0001();
}

Widget::~Widget()
{
  delete m0001;
  delete ui;
}
