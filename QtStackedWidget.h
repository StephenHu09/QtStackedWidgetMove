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
    QWidget *pWidget;
    QLabel *lblGrab;
    QParallelAnimationGroup *pGroup;

public:
    const static int ANI_DURATION = 500; // 设置动效时间 500ms

    enum MoveDirection {
        HORIZONTAL_MOVE,
        VERTICAL_MOVE
    };

    MoveAni(QWidget *parentObj)
    {
        pWidget = parentObj;
        lblGrab = Q_NULLPTR;
        pGroup = new QParallelAnimationGroup();
    }

    ~MoveAni()
    {
        MoveAniClear();
        delete pGroup;
        pGroup = Q_NULLPTR;
    }


    /*
     * 执行页面移动效果(核心方法)
     * direction: 移动方向，取自 MoveDirection 中的定义
     * isUpLeft: 是否 向左(横移) or 向上(纵移) 移动
     * pSW : 移动对象 QStackedWidget
     * index: pSW 对象待切换的页面索引号
     */
    void DoMove(int direction, bool isUpLeft, QStackedWidget *pSW, int index)
    {
        qDebug() << "pSW->size() : " << pSW->size();

        lblGrab = new QLabel(pWidget);
        lblGrab->resize(pSW->size());
        QPixmap pixmap = pSW->grab();
        pixmap.scaled(lblGrab->width(), lblGrab->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        lblGrab->setPixmap(pixmap);
        lblGrab->show();

        int ratio = isUpLeft ? (-1) : 1;

        QPropertyAnimation *pAniGrab = new QPropertyAnimation();
        pAniGrab->setTargetObject(lblGrab);
        pAniGrab->setPropertyName("geometry");
        pAniGrab->setDuration(ANI_DURATION);
        pAniGrab->setStartValue(pSW->geometry());
        if (direction == HORIZONTAL_MOVE) {
            pAniGrab->setEndValue(QRect(-pSW->width() * ratio, pSW->y(), pSW->width(), pSW->height()));
        } else {
            pAniGrab->setEndValue(QRect(pSW->x(),  (-1) * pSW->height() * ratio, pSW->width(), pSW->height()));
        }

        QPropertyAnimation *pAniCome = new QPropertyAnimation();
        pAniCome->setTargetObject(pSW);
        pAniCome->setPropertyName("geometry");
        pAniCome->setDuration(ANI_DURATION);
        if (direction == HORIZONTAL_MOVE) {
            pAniCome->setStartValue(QRect(pSW->width() * ratio, pSW->y(), pSW->width(), pSW->height()));
        } else {
            pAniCome->setStartValue(QRect(pSW->x(), pSW->height() * ratio, pSW->width(), pSW->height()));
        }
        pAniCome->setEndValue(pSW->geometry());

        /* setCurrentIndex 动作应放在动画start之前， 放在pAniCome位置设定好之后，可以避免画面闪现的问题 */
        pSW->setCurrentIndex(index);

        pGroup->addAnimation(pAniGrab);
        pGroup->addAnimation(pAniCome);
        pGroup->start();
    }

    void MoveAniClear()
    {
        if (lblGrab != Q_NULLPTR) {
            delete lblGrab;
            lblGrab = Q_NULLPTR;
        }

        pGroup->clear();
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
    void HandleArrowPageMove();
    void on_radioBtnH_clicked();
    void on_radioBtnV_clicked();
};

#endif // QTSTACKEDWIDGET_H
