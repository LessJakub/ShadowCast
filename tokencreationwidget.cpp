#include "tokencreationwidget.h"
#include "ui_tokencreationwidget.h"
#include <QColorDialog>
#include<iostream>

#include"tokencontrollerwidget.h"
class Group;

TokenCreationWidget::TokenCreationWidget(MapWidget *mapWidget,QWidget *parent) :
    QDialog(parent),
    mapWidget(mapWidget),
    ui(new Ui::TokenCreationWidget)

{
    ui->setupUi(this);
    tokensColor = QColorConstants::Blue;
    ui->tokenName->setText("New token");
    ui->tokenName->setFocus();

    for(std::map<int, Group*>::iterator itr = mapWidget->mapOfGroups.begin(); itr != mapWidget->mapOfGroups.end(); itr++)
    {
        ui->comboBox->addItem(itr->second->name,*itr->second->variantOfThis);
    }
    std::map<int, Group*>::iterator current = mapWidget->mapOfGroups.find(mapWidget->tokenController->chosenGroup);
    if (current != mapWidget->mapOfGroups.end())
    {
        ui->comboBox->setCurrentText(current->second->name);
    }
}

TokenCreationWidget::~TokenCreationWidget()
{
    delete ui;
}

Token* TokenCreationWidget::CreateToken()
{
    Token* tmp = new Token;
    tmp->SetName(ui->tokenName->text().toUtf8().constData());
    tmp->SetColour(tokensColor);
    tmp->alwaysVisable = ui->checkBox->isChecked();
    return tmp;
}

Item *TokenCreationWidget::CreateItem()
{
    Item* tmp = new Item;
    tmp->SetName(ui->tokenName->text().toUtf8().constData());
    tmp->SetColour(tokensColor);
    tmp->alwaysVisable = ui->checkBox->isChecked();
    return tmp;
}

void TokenCreationWidget::AddTokens()
{
    for(int i = 0; i < ui->numberOfTokens->value(); i++)
    {
        if(!ui->checkBox_2->isChecked())
        {
            Token* tmp = CreateToken();
            mapWidget->AddToken(tmp);
            std::map<int, Group*>::iterator itr = mapWidget->mapOfGroups.find(ui->comboBox->currentData().toInt());
            if (itr != mapWidget->mapOfGroups.end())
            {
                itr->second->AddToGroup(tmp);
            }
        }
        else
        {

            Item* tmp = CreateItem();
            mapWidget->AddToken(tmp);
        }

    }
    mapWidget->tokenController->ListTokens();
    mapWidget->tokenController->ListItems();
}

void TokenCreationWidget::on_pushButton_clicked()
{
   tokensColor = QColorDialog::getColor(tokensColor);
   ui->pushButton->setText(tokensColor.name());
}

void TokenCreationWidget::on_buttonBox_accepted()
{
    AddTokens();
}
