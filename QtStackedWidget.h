#ifndef QTSTACKEDWIDGET_H
#define QTSTACKEDWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QStackedWidget>
#include <QTimer>
#include <QDebug>

class MoveAni
{
private:
    QLabel *lblGrab;
    QPropertyAnimation *pAniGrab;
    QPropertyAnimation *pAniCome;
    QParallelAnimationGroup *pGroup;

public:
    const static int ANI_DURATION = 500; // 设置动效时间 500ms

    enum MoveDirection {
        HORIZONTAL_MOVE,
        VERTICAL_MOVE
    };

    MoveAni(QWidget *pThis)
    {
        lblGrab = new QLabel(pThis);
        pAniGrab = new QPropertyAnimation();
        pAniCome = new QPropertyAnimation();
        pGroup = new QParallelAnimationGroup();
    }

    ~MoveAni()
    {
        delete lblGrab;
        lblGrab = Q_NULLPTR;
        delete pAniGrab;
        pAniGrab = Q_NULLPTR;
        delete pAniCome;
        pAniCome = Q_NULLPTR;
        delete pGroup;
        pGroup = Q_NULLPTR;
    }


    /*
     * 执行页面移动效果(核心方法)
     * direction: 移动方向，取自 MoveDirection 中的定义
     * pSW: 移动对象stackedWidget
     * isUpLeft: 是否 向左(横移) or 向上(纵移) 移动
     */
    void DoMove(int direction, QStackedWidget *pSW, bool isUpLeft)
    {
        qDebug() << "pSW->size() : " << pSW->size();

        lblGrab->resize(pSW->size());
        lblGrab->setPixmap(pSW->grab());
        lblGrab->show();

        int ratio = isUpLeft ? (-1) : 1;

        pAniGrab->setTargetObject(lblGrab);
        pAniGrab->setPropertyName("geometry");
        pAniGrab->setDuration(ANI_DURATION);
        pAniGrab->setStartValue(pSW->geometry());
        if (direction == HORIZONTAL_MOVE) {
            pAniGrab->setEndValue(QRect(-pSW->width() * ratio, pSW->y(), pSW->width(), pSW->height()));
        } else {
            pAniGrab->setEndValue(QRect(pSW->x(),  -pSW->height() * ratio, pSW->width(), pSW->height()));
        }


        pAniCome->setTargetObject(pSW);
        pAniCome->setPropertyName("geometry");
        pAniCome->setDuration(ANI_DURATION);
        if (direction == HORIZONTAL_MOVE) {
            pAniCome->setStartValue(QRect(pSW->width() * ratio, pSW->y(), pSW->width(), pSW->height()));
        } else {
            pAniCome->setStartValue(QRect(pSW->x(), pSW->height() * ratio, pSW->width(), pSW->height()));
        }
        pAniCome->setEndValue(pSW->geometry());

        pGroup->addAnimation(pAniGrab);
        pGroup->addAnimation(pAniCome);
        pGroup->start();
    }
};

namespace Ui {
    class QtStackedWidget;
}

class QtStackedWidget : public QDialog
{
    Q_OBJECT

public:
    explicit QtStackedWidget(QWidget *parent = 0);
    ~QtStackedWidget();

private:
    void EnableTitleButtons(bool en);

private:
    Ui::QtStackedWidget *ui;
    MoveAni *m_pageMoveAni;

private slots:
    void HandlePageMove();
    void on_radioBtnH_clicked();
    void on_radioBtnV_clicked();
};

#endif // QTSTACKEDWIDGET_H
