#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QPushButton>
#include <QKeySequence>
#include <iostream>
#include <time.h>
#include "square.h"
#include <QRadioButton>
#include <QButtonGroup>

class ParaFrame : public QMainWindow {
    Q_OBJECT
    QRadioButton *radiodefault, *radiocustom;
public:
    ParaFrame(QWidget *parent = NULL){
        QButtonGroup choice(this);
        choice.addButton(radiodefault);
        choice.addButton(radiocustom);
        QPushButton go("GO!");
        QObject::connect(go, SIGNAL(clicked()), this, SLOT(buttonClicked()));

    }
};

#endif // PARAMETERS_H
