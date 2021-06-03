#ifndef TOKENCONTROLLERWIDGET_H
#define TOKENCONTROLLERWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QComboBox>
#include<memory>
#include "mapwidget.h"

namespace Ui {
class TokenControllerWidget;
}

class TokenControllerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TokenControllerWidget(MapWidget*mapWidget, QWidget *parent = nullptr);
    ~TokenControllerWidget();

    

    //Lists all groups in mapwidget->mapOfGroups in this widgets combo box.
    void ListGroups();

    //Lists all tokens in currently choosen group in first list.
    void ListTokens();

    //Lists all items in mapWidget->mapOfItems in the second list.
    void ListItems();

    //Adds group to the combo box. Item added to the combo box is combination of groups name and it's id in QVariant format.
    void AddGroup(Group *group);

    //Clears combo box and list of tokens.
    void ClearGroups();

    //Pointer to mapWidget.
    MapWidget* mapWidget;
    //Id of currently chosen group.
    int chosenGroup;

    //Boolean value describing if items should be listed. It is set to false while creating new file.
    bool list;

private slots:
    //When item in combo box changes, value of the choosen group changes and ListTokens() method is called.
    void on_comboBox_currentIndexChanged(int index);

    //Lists tokens of current group when combo box is activated. 
    void on_comboBox_activated(int index);

    //Lists tokens of current group when combo box is highlighted. 
    void on_comboBox_highlighted(int index);

    //Deletes token when it is double clicked.
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    //Deletes item when it is double clicked.
    void on_listWidget_2_itemDoubleClicked(QListWidgetItem *item);

    //Sets cursor position to tokens position when it is clicked.
    void on_listWidget_itemClicked(QListWidgetItem *item);

    //Sets cursor position to items position when it is clicked.
    void on_listWidget_2_itemClicked(QListWidgetItem *item);

private:
    Ui::TokenControllerWidget *ui;
};

#endif // TOKENCONTROLLERWIDGET_H
