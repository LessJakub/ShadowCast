#ifndef GROUPCREATIONWINDOW_H
#define GROUPCREATIONWINDOW_H

#include <QDialog>
#include "mapwidget.h"

namespace Ui {
class GroupCreationWindow;
}

class GroupCreationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GroupCreationWindow(MapWidget *mapWidget, QWidget *parent = nullptr);
    ~GroupCreationWindow();

    //Pointer to mapWidget, used for adding the tokens to the scene
    MapWidget *mapWidget;

    //Group pointer to group created
    Group* createdGroup;
private slots:
    //When accept button is clicked, group will be created and 
    void on_buttonBox_accepted();

    //Changes name of the group created when value in text edit changes
    void on_lineEdit_textChanged(const QString &arg1);

    //Closes window when dialog is rejected
    void on_buttonBox_rejected();

    //changes visibility of group
    void on_checkBox_stateChanged(int arg1);

private:


    Ui::GroupCreationWindow *ui;
};

#endif // GROUPCREATIONWINDOW_H
