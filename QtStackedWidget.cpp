#include "QtStackedWidget.h"
#include "ui_QtStackedWidget.h"

QtStackedWidget::QtStackedWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtStackedWidget)
{
    ui->setupUi(this);

    ui->tabWidget->setTabText(0, "Tab1_页面移动");
    ui->tabWidget->setTabText(1, "Tab2");
    ui->tabWidget->setTabText(2, "Tab3");

    connect(ui->pushButton1, &QPushButton::clicked, this, &QtStackedWidget::HandlePageMove);
    connect(ui->pushButton2, &QPushButton::clicked, this, &QtStackedWidget::HandlePageMove);
    connect(ui->pushButton3, &QPushButton::clicked, this, &QtStackedWidget::HandlePageMove);
    connect(ui->pushButton4, &QPushButton::clicked, this, &QtStackedWidget::HandlePageMove);
    connect(ui->pushButton5, &QPushButton::clicked, this, &QtStackedWidget::HandlePageMove);

    ui->pageBtn1->setStyleSheet("QPushButton{background:rgb(105, 201, 177);}");
    ui->pageBtn2->setStyleSheet("QPushButton{background:rgb(105, 177, 201);}");
    ui->pageBtn3->setStyleSheet("QPushButton{background:rgb(201, 177, 105);}");
    ui->pageBtn4->setStyleSheet("QPushButton{background:rgb(105, 129, 201);}");
    ui->pageBtn5->setStyleSheet("QPushButton{background:rgb(201, 105, 177);}");

    ui->stackedWidget->setCurrentIndex(0);
    ui->radioBtnH->setChecked(true);

    // m_pageMoveAni动效对象需要与待移动对象(ui->stackedWidget) 的父控件保持一致，否则有位移偏差
    // 不添加widgetMove控件，纵向切换会有问题
    m_pageMoveAni = new MoveAni(ui->widgetMove);
}

QtStackedWidget::~QtStackedWidget()
{
    delete m_pageMoveAni;
    m_pageMoveAni = nullptr;
    delete ui;
}

void QtStackedWidget::EnableTitleButtons(bool en)
{
    QList<QPushButton *> btns = ui->titleBar->findChildren<QPushButton *>();
    foreach (QPushButton *item, btns) {
        item->setEnabled(en);
    }
}

void QtStackedWidget::HandlePageMove()
{
    int type;
    if (ui->radioBtnH->isChecked()) {
        type = MoveAni::HORIZONTAL_MOVE;
    } else {
        type = MoveAni::VERTICAL_MOVE;
    }

    QPushButton *btn = reinterpret_cast<QPushButton *>(sender());
    if (btn == ui->pushButton1) {
        int pageIndex = 0;
        if (ui->stackedWidget->currentIndex() > pageIndex) {
            m_pageMoveAni->DoMove(type, ui->stackedWidget, true);
        }
        ui->stackedWidget->setCurrentIndex(pageIndex);
    }

    if (btn == ui->pushButton2) {
        int pageIndex = 1;
        if (ui->stackedWidget->currentIndex() < pageIndex) {
            m_pageMoveAni->DoMove(type, ui->stackedWidget, false);
        } else if (ui->stackedWidget->currentIndex() > pageIndex) {
            m_pageMoveAni->DoMove(type, ui->stackedWidget, true);
        }
        ui->stackedWidget->setCurrentIndex(pageIndex);
    }

    if (btn == ui->pushButton3) {
        int pageIndex = 2;
        if (ui->stackedWidget->currentIndex() < pageIndex) {
            m_pageMoveAni->DoMove(type, ui->stackedWidget, false);
        } else if (ui->stackedWidget->currentIndex() > pageIndex) {
            m_pageMoveAni->DoMove(type, ui->stackedWidget, true);
        }
        ui->stackedWidget->setCurrentIndex(pageIndex);
    }

    if (btn == ui->pushButton4) {
        int pageIndex = 3;
        if (ui->stackedWidget->currentIndex() < pageIndex) {
            m_pageMoveAni->DoMove(type, ui->stackedWidget, false);
        } else if (ui->stackedWidget->currentIndex() > pageIndex) {
            m_pageMoveAni->DoMove(type, ui->stackedWidget, true);
        }
        ui->stackedWidget->setCurrentIndex(pageIndex);
    }

    if (btn == ui->pushButton5) {
        int pageIndex = 4;
        if (ui->stackedWidget->currentIndex() < pageIndex) {
            m_pageMoveAni->DoMove(type, ui->stackedWidget, false);
        }
        ui->stackedWidget->setCurrentIndex(pageIndex);
    }

    // 页面切换动画时，禁用按钮
    EnableTitleButtons(false);
    QTimer::singleShot(MoveAni::ANI_DURATION, this, [&](){
        EnableTitleButtons(true);
    });
}

void QtStackedWidget::on_radioBtnH_clicked()
{
    qDebug() << "radioBtnH check =" << ui->radioBtnH->isChecked();
    qDebug() << "radioBtnV check =" << ui->radioBtnV->isChecked();
}

void QtStackedWidget::on_radioBtnV_clicked()
{
    qDebug() << "radioBtnH check =" << ui->radioBtnH->isChecked();
    qDebug() << "radioBtnV check =" << ui->radioBtnV->isChecked();
}
