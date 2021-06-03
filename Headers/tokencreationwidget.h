#ifndef TOKENCREATIONWIDGET_H
#define TOKENCREATIONWIDGET_H

#include <QDialog>
#include <QColor>

#include"item.h"

class MapWidget;

namespace Ui {
class TokenCreationWidget;
}

class TokenCreationWidget : public QDialog
{
    Q_OBJECT

public:
    explicit TokenCreationWidget(MapWidget* mapWidget, QWidget *parent = nullptr);
    ~TokenCreationWidget();

    //Colour of token that will be created.
    QColor tokensColor;

    //Pointer to map widget. Required to add token to the group and to the scene.
    MapWidget* mapWidget;

    //Creates token with currently chosen parameters
    Token* CreateToken();

    //Creates item with chosen parameters.
    Item* CreateItem();

    //Creates selected number of tokens, then adds them to map Widget.
    void AddTokens();
private slots:
    //Opens colour dialog when button is clicked and changes it's name to value of chosen colour.
    void on_pushButton_clicked();

    //When dialog is accepted tokens with selected parameters and in selected number will be created.
    void on_buttonBox_accepted();


private:
    Ui::TokenCreationWidget *ui;
};

#endif // TOKENCREATIONWIDGET_H
