// Minimal example: create a FrameWgt with a simple central widget
#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "framewgt.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // Create a simple central widget (demo content)
    QWidget *center = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(center);
    lay->setContentsMargins(20, 20, 20, 20);
    lay->setSpacing(10);
    QLabel *lab = new QLabel("Hello FrameWgt example");
    lab->setAlignment(Qt::AlignCenter);
    QPushButton *btn = new QPushButton("A demo button");
    lay->addWidget(lab);
    lay->addWidget(btn);
    center->setLayout(lay);
    center->setMinimumSize(360, 240);

    // Create FrameWgt with the center widget
    FrameWgt *w = new FrameWgt(center);
    w->setTitleText("FrameWgt Demo");
    w->setTitleBarColor(Qt::white);
    w->setBackgroundColor(QColor(255,255,255,255));
    w->setRadius(12);
    w->setBlurRadius(12);
    w->show();

    return app.exec();
}