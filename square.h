#ifndef SQUARE_H
#define SQUARE_H

#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QPushButton>
#include <QKeySequence>
#include <iostream>

class Square : public QPushButton{
    Q_OBJECT
    int x, y;
    char counter = '0';
    bool bomb = false, revealed = false, flagged = false, cascaded = false, clicked = false;
public:
    Square(QWidget *parent, int x, int y): QPushButton(" ",parent), x(x), y(y){
        QObject::connect(this, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    }
    bool isABomb(){
        return bomb;
    }
    void makeBomb(){
        bomb = true;
    }
    void plusplus(){
        counter++;
    }
    void reveal(){
        revealed = true;
    }
    bool isCascaded(){
        return cascaded;
    }
    void cascade(){
        cascaded = true;
    }
    bool isRevealed(){
        return revealed;
    }
    bool isFlagged(){
        return flagged;
    }
    bool isClicked(){
        return clicked;
    }
    void unclick(){
        clicked = false;
    }
    void flipFlag(){
        flagged = !flagged;
    }
    void refresh(){
        if(flagged) setText("F");
        else if(revealed){
            if(bomb) setText("X");
            else setText(QString(QChar::fromLatin1(counter)));
        }
    }
    char getCounter(){
        return counter;
    }


public slots:
    void buttonClicked() {
        clicked = true;
    }
};

#endif // SQUARE_H
