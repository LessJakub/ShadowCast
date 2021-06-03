#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>
#include <memory>
#include "group.h"
#include "battlescene.h"
#include "groupcreationwindow.h"
#include "tokencreationwidget.h"
#include "tokencontrollerwidget.h"

class MapWidget;
//class Scene;

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget * parent = nullptr);
    ~MainWindow();
    //Pointer to token controller which manages the groups and widgets
    TokenControllerWidget* tokenController;
protected:

private slots:

    //Resets the application, deleting mapWidget and tokenController and creates new ones
    void newFile();

    //Calls method clearImage of mapWidget if mapWidget is not a nullptr. Delets images of walls and background and resets each tile of FoV to nonwall
    void clearImage();

    //calls switch drawing method of mapWidget if mapWidget is not a nullptr. Switches between drawing and erasing
    void switchDrawing();

    //call switchWalls method of mapWidget if mapWidget is not a nullptr. 
    void switchWalls();

    //calls switchShadows method of mapWidget if mapWidgt is not a nullptr.
    void switchShadwos();

    //call switchBackground method of mapWidget if mapWidget is not nullptr.
    void switchBackground();

    //Creates file dialog and gets name of file to load. Afterwards calls loadfromfile method of mapWidget.
    void open();

    //Creates file dialog and gets name of background file to be loaeded. Afterwards calls loadBackground method of mapWidget
    void openBackground();

    //Creates file dialog and gets name of file to be saved.
    void save();

    //Opens color dialog, after color is choosen sets pens color in mapWidget
    void penColor();

    //Opens dialog, that allows user to chose width of the pen (int). Sets choosen value in mapWidget
    void penWidth();

    //Opens create group window. Links mapWidget to created dialog
    void openCreateGroup();

    //Opens create token window. Links mapWidget to created dialog
    void openCreateToken();
private:
    //Creates actions linked to menubar of the program.
    void createActions();

    //Creates menu bar.
    void createMenus();

    //Updates menu bar and actions in to it.
    void updateMenus();

    //Pointer to mapWidget. Used in creation of token/group creators and tokenController
    MapWidget* mapWidget;

    //QGraphicsScene with poiter to mapWidget
    battleScene* scene;

    //Pointer to group creation window.
    GroupCreationWindow* groupCreation;
    //Pointer to token creation widget.
    TokenCreationWidget* tokenCreation;

    //File menu linked to menu bar.
    QMenu *fileMenu;

    //Options menu linked to menu bar.
    QMenu *optionMenu;

    //Save action, linked to save() method of this class.
    QAction *saveAct;

    //Load action, linked to open() method of this class.
    QAction *loadAct;

    //New File action linked to newFile() method of this class.
    QAction *newFileAct;

    //Exit action, linked to close() method of this class.
    QAction *exitAct;

    //Pen color action, linked to penColor() method of this calss.
    QAction *penColorAct;

    //Pen width action, linked to penWidth() method of this class.
    QAction *penWidthAct;

    //Clear screen action, linked toc clearImage() method of this class.
    QAction *clearScreenAct;

    //Load background action, linked toc openBackground() method of this class.
    QAction *loadBackgroundAct;

    //Switch background action, linked toc switchBackground() method of this class.
    QAction *switchBackgroundAct;

    //Switch shadows action, linked toc switchShadwos() method of this class.
    QAction *switchShadowsAct;

    //Switch walls action, linked toc switchWalls() method of this class.
    QAction *switchWallsAct;

    //Switch drawing (switches between drawing and erasing) action, linked toc switchDrawing() method of this class.
    QAction *switchDrawingAct;

    //Open group creation window action, linked toc openCreateGroup() method of this class.
    QAction *openGroupCreationAct;

    //Open token creaton window action, linked toc openCreateToken() method of this class.
    QAction *openTokenCreationAct;

    Ui::MainWindow *ui;
};

#endif
