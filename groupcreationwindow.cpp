#include "groupcreationwindow.h"
#include "ui_groupcreationwindow.h"

#include <QString>
#include<iostream>

GroupCreationWindow::GroupCreationWindow(MapWidget *mapWidget, QWidget *parent) :
    QDialog(parent),
    mapWidget(mapWidget),
    ui(new Ui::GroupCreationWindow)

{
    ui->setupUi(this);
    //createdGroup.reset(new Group("New group"));
    createdGroup = new Group;
    ui->lineEdit->setFocus();
    ui->lineEdit->setText(createdGroup->name);
    createdGroup->alwaysVisable = false;
}

GroupCreationWindow::~GroupCreationWindow()
{
    delete ui;
}

void GroupCreationWindow::on_buttonBox_accepted()
{
    mapWidget->AddGroup(createdGroup);
}

void GroupCreationWindow::on_lineEdit_textChanged(const QString &arg1)
{
    createdGroup->SetName(arg1);
}

void GroupCreationWindow::on_buttonBox_rejected()
{
    //delete createdGroup;
    close();
}

void GroupCreationWindow::on_checkBox_stateChanged(int arg1)
{
    createdGroup->alwaysVisable=arg1;
}
