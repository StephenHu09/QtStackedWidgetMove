#include "QtStackedWidget.h"
#include "ui_QtStackedWidget.h"
#include <QFile>

void LoadStyleSheet(QWidget *obj, const QString &qssFile)
{
    if (obj == 0) {
        return;
    }

    QFile file(qssFile);
    if (file.exists()) {
        if (file.open(QFile::ReadOnly) == false) {
            qDebug() << "qss file: " << qssFile << " open failed";
        }
        QString styleSheet = QLatin1String(file.readAll());
        obj->setStyleSheet(styleSheet);
        file.close();
    } else {
        qDebug() << "qss file is not exist: " << qssFile;
    }
}

QtStackedWidget::QtStackedWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtStackedWidget)
{
    ui->setupUi(this);

    ui->tabWidget->setTabText(0, "Test");
    ui->tabWidget->setTabText(1, "Tab2");

    connect(ui->pushButton1, &QPushButton::clicked, this, &QtStackedWidget::HandlePageMove);
    connect(ui->pushButton2, &QPushButton::clicked, this, &QtStackedWidget::HandlePageMove);
    connect(ui->pushButton3, &QPushButton::clicked, this, &QtStackedWidget::HandlePageMove);
    connect(ui->pushButton4, &QPushButton::clicked, this, &QtStackedWidget::HandlePageMove);
    connect(ui->pushButton5, &QPushButton::clicked, this, &QtStackedWidget::HandlePageMove);

    connect(ui->tBtnLeft, &QPushButton::clicked, this, &QtStackedWidget::HandleArrowPageMove);
    connect(ui->tBtnRight, &QPushButton::clicked, this, &QtStackedWidget::HandleArrowPageMove);

    ui->pageBtn1->setStyleSheet("QPushButton{background:rgb(105, 201, 177);}");
    ui->pageBtn2->setStyleSheet("QPushButton{background:rgb(105, 177, 201);}");
    ui->pageBtn3->setStyleSheet("QPushButton{background:rgb(201, 177, 105);}");
    ui->pageBtn4->setStyleSheet("QPushButton{background:rgb(105, 129, 201);}");
    ui->pageBtn5->setStyleSheet("QPushButton{background:rgb(201, 105, 177);}");

    QList<QPushButton *> btns = ui->titleBar->findChildren<QPushButton *>();
    foreach (QPushButton *item, btns) {
        item->setChecked(false);
        item->setCheckable(false);
    }

    ui->tabWidget->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
    ui->radioBtnH->setChecked(true);

    // m_pageMoveAni动效对象需要与待移动对象(ui->stackedWidget) 的父控件保持一致，否则有位移偏差
    // 不添加widgetMove控件，纵向切换会有问题
    m_pageMoveAni = new MoveAni(ui->widgetMove);

    LoadStyleSheet(this, ":/res/style.qss");
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

    QList<QToolButton *> tbtns = ui->widgetArrow->findChildren<QToolButton *>();
    foreach (QToolButton *item, tbtns) {
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
            m_pageMoveAni->DoMove(type, true, ui->stackedWidget, pageIndex);
        }
    }

    if (btn == ui->pushButton2) {
        int pageIndex = 1;
        if (ui->stackedWidget->currentIndex() < pageIndex) {
            m_pageMoveAni->DoMove(type, false, ui->stackedWidget, pageIndex);
        } else if (ui->stackedWidget->currentIndex() > pageIndex) {
            m_pageMoveAni->DoMove(type, true, ui->stackedWidget, pageIndex);
        }
    }

    if (btn == ui->pushButton3) {
        int pageIndex = 2;
        if (ui->stackedWidget->currentIndex() < pageIndex) {
            m_pageMoveAni->DoMove(type, false, ui->stackedWidget, pageIndex);
        } else if (ui->stackedWidget->currentIndex() > pageIndex) {
            m_pageMoveAni->DoMove(type, true, ui->stackedWidget, pageIndex);
        }
    }

    if (btn == ui->pushButton4) {
        int pageIndex = 3;
        if (ui->stackedWidget->currentIndex() < pageIndex) {
            m_pageMoveAni->DoMove(type, false, ui->stackedWidget, pageIndex);
        } else if (ui->stackedWidget->currentIndex() > pageIndex) {
            m_pageMoveAni->DoMove(type, true, ui->stackedWidget, pageIndex);
        }
    }

    if (btn == ui->pushButton5) {
        int pageIndex = 4;
        if (ui->stackedWidget->currentIndex() < pageIndex) {
            m_pageMoveAni->DoMove(type, false, ui->stackedWidget, pageIndex);
        }
    }

    // 页面切换动画时，禁用按钮
    EnableTitleButtons(false);
    QTimer::singleShot(MoveAni::ANI_DURATION, this, [&](){
        m_pageMoveAni->MoveAniClear();
        EnableTitleButtons(true);
    });
}

void QtStackedWidget::HandleArrowPageMove()
{
    int type;
    if (ui->radioBtnH->isChecked()) {
        type = MoveAni::HORIZONTAL_MOVE;
    } else {
        type = MoveAni::VERTICAL_MOVE;
    }

    bool switchLoop = false;
    if (ui->checkBoxLoop->checkState() == Qt::Checked) {
        switchLoop = true;
    }

    int curPageIndex = ui->stackedWidget->currentIndex();
    int maxIndex = ui->stackedWidget->count() - 1;
    bool isMoveStart = false;

    QToolButton *btn = reinterpret_cast<QToolButton *>(sender());
    if (btn == ui->tBtnLeft) {
//        ui->stackedWidget->setCurrentIndex(curPageIndex);
        if (curPageIndex > 0) {
            m_pageMoveAni->DoMove(type, true, ui->stackedWidget, curPageIndex - 1);
            isMoveStart = true;
        }
        if (curPageIndex == 0 && switchLoop) {
            m_pageMoveAni->DoMove(type, true, ui->stackedWidget, maxIndex);
            isMoveStart = true;
        }
    }

    if (btn == ui->tBtnRight) {
//        ui->stackedWidget->setCurrentIndex(curPageIndex);
        if (curPageIndex < maxIndex) {
            m_pageMoveAni->DoMove(type, false, ui->stackedWidget, curPageIndex + 1);
            isMoveStart = true;
        }
        if (curPageIndex == maxIndex && switchLoop) {
            m_pageMoveAni->DoMove(type, false, ui->stackedWidget, 0);
            isMoveStart = true;
        }
    }

    if (!isMoveStart) {
        return;
    }

    // 页面切换动画时，禁用按钮
    EnableTitleButtons(false);
    QTimer::singleShot(MoveAni::ANI_DURATION, this, [&](){
        m_pageMoveAni->MoveAniClear();
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
