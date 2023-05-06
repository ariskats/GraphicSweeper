#ifndef MYWIDGET_H
#define MYWIDGET_H

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

class MineFrame : public QMainWindow {
    Q_OBJECT
    int columnTotal, rowTotal, mineTotal;
    QMenuBar* bar = menuBar();
    QAction* revealing;
    QAction* flagging;
    QAction* victory;
    QAction* defeat;
    std::vector<Square*> squareVector;
    std::vector<std::vector<Square*>> squareVectorVector;
    bool revealingMode = true, gameOver = false;
public:
  MineFrame(QWidget *parent = NULL, int columnTotal = 30, int rowTotal = 16, int mineTotal = 99) : QMainWindow(parent), columnTotal(columnTotal), rowTotal(rowTotal), mineTotal(mineTotal) {
    this->resize(30*columnTotal, 30*rowTotal+20);
    this->setWindowTitle("Minesweeper by Aris Katsanevakis");
    createActions();
    bar->addAction(revealing);

    srand (time(nullptr));

    for(int i = 0; i<columnTotal ; i++){                // create the 2D vector
        for(int i2 = 0; i2<rowTotal; i2++){
            Square* button = new Square(this,i,i2);
            button->setGeometry(30*i, 30*i2+20, 30, 30);
            QObject::connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
            squareVector.push_back(button);
        }
        squareVectorVector.push_back(squareVector);
        squareVector.clear();
    }
    for(int i = mineTotal; i>0; i--){					// decides where the mines are
        int x=random(0,columnTotal), y=random(0,rowTotal);
        if(!squareVectorVector[x][y]->isABomb()) squareVectorVector[x][y]->makeBomb();
        else i++;
    }

    for(unsigned int x=0; x < columnTotal; x++ ){				// calculates the neighboring bomb counters
        for(unsigned int y = 0; y < rowTotal; y++){
            if(squareVectorVector[x][y]->isABomb()){
                if(x>0){								// one check for each neihgbour square
                    squareVectorVector[x-1][y]->plusplus();
                    if(y>0) squareVectorVector[x-1][y-1]->plusplus();
                    if(y<rowTotal-1) squareVectorVector[x-1][y+1]->plusplus();
                }
                if(y>0){
                    squareVectorVector[x][y-1]->plusplus();
                    if(x<columnTotal-1) squareVectorVector[x+1][y-1]->plusplus();
                }
                if(x<columnTotal-1){
                    squareVectorVector[x+1][y]->plusplus();
                    if(y<rowTotal-1) squareVectorVector[x+1][y+1]->plusplus();
                }
                if(y<rowTotal-1) squareVectorVector[x][y+1]->plusplus();
            }
        }
    }

    QObject::connect(revealing, SIGNAL(triggered()), this, SLOT(changeToFlag()));
    QObject::connect(flagging, SIGNAL(triggered()), this, SLOT(changeToReveal()));
  }

public slots:
    void buttonClicked() {
        // square action occurs first
        if(!gameOver){
            Square* square = NULL;
            bool repeat = false;;
            for(unsigned int x = 0; x < columnTotal; x++)               // finds which square was clicked
                for(unsigned int y = 0; y < rowTotal; y++)
                    if(squareVectorVector[x][y]->isClicked()){
                        square = squareVectorVector[x][y];
                        square->unclick();
                        break;
                    }
            if(revealingMode==false){
                if(!square->isRevealed()) square->flipFlag();
            }
            else{
                if(!square->isFlagged() && !square->isRevealed()){
                    square->reveal();
                    repeat = true;
                }
                while(repeat){
                    repeat = false;
                    for(unsigned int x = 0; x < columnTotal; x++)           // reveals squares if they have no neihboring bombs
                        for(unsigned int y = 0; y < rowTotal; y++)
                            if(squareVectorVector[x][y]->getCounter()=='0' && squareVectorVector[x][y]->isRevealed() && !squareVectorVector[x][y]->isCascaded()){
                                squareVectorVector[x][y]->cascade();
                                repeat = true;
                                if(x<columnTotal-1){
                                    squareVectorVector[x+1][y]->reveal();
                                    if(y<rowTotal-1) squareVectorVector[x+1][y+1]->reveal();
                                    if(y>1) squareVectorVector[x+1][y-1]->reveal();
                                }
                                if(x>1){
                                    squareVectorVector[x-1][y]->reveal();
                                    if(y<rowTotal-1) squareVectorVector[x-1][y+1]->reveal();
                                    if(y>1) squareVectorVector[x-1][y-1]->reveal();
                                }
                                if(y<rowTotal-1) squareVectorVector[x][y+1]->reveal();
                                if(y>1) squareVectorVector[x][y-1]->reveal();
                            }
                }
            }
            for(unsigned int x = 0; x < columnTotal; x++)
                for(unsigned int y = 0; y < rowTotal; y++)
                    squareVectorVector[x][y]->refresh();
            checkVictory();
        }

    }
    void changeToFlag() {                   // change mode, shortcut = F
        bar->clear();
        bar->addAction(flagging);
        revealingMode = false;
    }
    void changeToReveal() {                 // change mode, shortcut = R
        bar->clear();
        bar->addAction(revealing);
        revealingMode = true;
    }

private:
    void checkVictory(){
        int flagCounter;
        for(unsigned int x = 0; x < columnTotal; x++)
            for(unsigned int y = 0; y < rowTotal; y++){
                if(squareVectorVector[x][y]->isFlagged()){
                    if(squareVectorVector[x][y]->isABomb()) flagCounter++;
                    else{
                        flagCounter = 0;
                        break;
                    }
                }
            }
        if(flagCounter == mineTotal){
            bar->clear();
            bar->addAction(victory);
            gameOver = true;
            revealAll();
        }
        for(unsigned int x = 0; x < columnTotal; x++)
            for(unsigned int y = 0; y < rowTotal; y++)
                if(squareVectorVector[x][y]->text() == "X"){
                    bar->clear();
                    bar->addAction(defeat);
                    gameOver = true;;
                    revealAll();
                }
    }
    void revealAll(){
        for(unsigned int x = 0; x < columnTotal; x++)
            for(unsigned int y = 0; y < rowTotal; y++){
                squareVectorVector[x][y]->reveal();
                squareVectorVector[x][y]->refresh();
            }
    }
    int random(int min, int max){
        return rand() % (max-min) + min;
    }
    void createActions() {
        revealing = new QAction("Revealing", this);
        revealing->setShortcut(QKeySequence(Qt::Key_F));
        flagging = new QAction("Flagging", this);
        flagging->setShortcut(QKeySequence(Qt::Key_R));
        defeat = new QAction("YOU LOSE...", this);
        victory = new QAction("YOU WIN!", this);
    }
    void createMenus() {
        QMenuBar* bar = menuBar();
        QMenu* fileMenu = bar->addMenu(tr("Counter"));
        bar->addAction(revealing);
        fileMenu->addAction(flagging);
    }
};

#endif // MYWIDGET_H
