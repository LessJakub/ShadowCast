#include "tokencontrollerwidget.h"
#include "ui_tokencontrollerwidget.h"
#include <iostream>
#include <QListWidget>
#include <QComboBox>
#include<map>
#include"group.h"
#include"item.h"

TokenControllerWidget::TokenControllerWidget(MapWidget *mapWidget, QWidget *parent) :
    QWidget(parent),
    mapWidget(mapWidget),
    ui(new Ui::TokenControllerWidget)

{
    ui->setupUi(this);
    ui->comboBox->setDuplicatesEnabled(false);
    list = true;
    ListGroups();
}

TokenControllerWidget::~TokenControllerWidget()
{
    mapWidget = nullptr;
    delete ui;
}



void TokenControllerWidget::ListGroups()
{
    if(list)
    {
        for(std::map<int, Group*>::iterator itr = mapWidget->mapOfGroups.begin(); itr != mapWidget->mapOfGroups.end(); itr++)
        {
            ui->comboBox->addItem(itr->second->name,*itr->second->variantOfThis);
            chosenGroup = itr->second->variantOfThis->toInt();
        }

    }
}

void TokenControllerWidget::ListTokens()
{
    if(list)
    {
    ui->listWidget->clear();
    std::map<int, Group*>::iterator itrToChoosenGroup = mapWidget->mapOfGroups.find(ui->comboBox->currentData().toInt());
    if (itrToChoosenGroup == mapWidget->mapOfGroups.end()) return;

    for(std::map<int, Token*>::iterator itr = itrToChoosenGroup->second->tokensInGroup.begin();
        itr != itrToChoosenGroup->second->tokensInGroup.end(); itr++)
    {

         ui->listWidget->addItem(itr->second->name);
         ui->listWidget->item(ui->listWidget->count() - 1)->setData(0x0100,QVariant(itr->second->id));
    }
    update();
    }
    ListItems();
}

void TokenControllerWidget::ListItems()
{
    if(list)
    {
    ui->listWidget_2->clear();

    for(std::map<int, Item*>::iterator itr = mapWidget->mapOfItems.begin();
        itr != mapWidget->mapOfItems.end(); itr++)
    {

         ui->listWidget_2->addItem(itr->second->name);
         ui->listWidget_2->item(ui->listWidget_2->count() - 1)->setData(0x0100,QVariant(itr->second->id));
    }
    update();
    }
}

void TokenControllerWidget::AddGroup(Group*group)
{
    ui->comboBox->addItem(group->name, *group->variantOfThis);
}

void TokenControllerWidget::ClearGroups()
{
    //chosenGroup = 0;
    ui->comboBox->clear();
    ui->listWidget->clear();
}

void TokenControllerWidget::on_comboBox_currentIndexChanged(int index)
{
   chosenGroup = ui->comboBox->currentData().toInt();
   ListTokens();
   mapWidget->fovChanged();
}

void TokenControllerWidget::on_comboBox_activated(int index)
{
    ListTokens();
}

void TokenControllerWidget::on_comboBox_highlighted(int index)
{
    ListTokens();
}

void TokenControllerWidget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    std::map<int, Group*>::iterator itrToChoosenGroup = mapWidget->mapOfGroups.find(ui->comboBox->currentData().toInt());
    if (itrToChoosenGroup == mapWidget->mapOfGroups.end()) return;

    int choosenId = item->data(0x0100).toInt();

    std::map<int, Token*>::iterator tmp = itrToChoosenGroup->second->tokensInGroup.find(choosenId);
    if (tmp == itrToChoosenGroup->second->tokensInGroup.end()) return;

    QPointF posOnScreen = mapWidget->mapToGlobal(tmp->second->pos());

    cursor().setPos(posOnScreen.x(),posOnScreen.y());
}

void TokenControllerWidget::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{

    std::map<int, Group*>::iterator itrToChoosenGroup = mapWidget->mapOfGroups.find(ui->comboBox->currentData().toInt());
    if (itrToChoosenGroup == mapWidget->mapOfGroups.end()) return;

    int choosenId = item->data(0x0100).toInt();

    std::map<int, Token*>::iterator tmp = itrToChoosenGroup->second->tokensInGroup.find(choosenId);
    if (tmp == itrToChoosenGroup->second->tokensInGroup.end()) return;

    mapWidget->RemoveToken(tmp->second);
    itrToChoosenGroup->second->tokensInGroup.erase(tmp);
    ListTokens();
    mapWidget->fovChanged();
}

void TokenControllerWidget::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    int choosenId = item->data(0x0100).toInt();

    std::map<int, Item*>::iterator tmp = mapWidget->mapOfItems.find(choosenId);
    if (tmp == mapWidget->mapOfItems.end()) return;

    QPointF posOnScreen = mapWidget->mapToGlobal(tmp->second->pos());
    tmp->second->setVisible(true);
    cursor().setPos(posOnScreen.x(),posOnScreen.y());
}

void TokenControllerWidget::on_listWidget_2_itemDoubleClicked(QListWidgetItem *item)
{
    int choosenId = item->data(0x0100).toInt();

    std::map<int, Item*>::iterator tmp = mapWidget->mapOfItems.find(choosenId);
    if(tmp == mapWidget->mapOfItems.end()) return;

    tmp->second->applyPreviousState();
    mapWidget->RemoveToken(tmp->second);
    mapWidget->mapOfItems.erase(tmp);
    ListTokens();
    mapWidget->fovChanged();
}




