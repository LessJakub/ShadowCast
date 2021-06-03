#include <QtWidgets>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mapwidget.h"
#include "token.h"
#include "tokencontrollerwidget.h"

#include<QFile>
#include<QDataStream>

#include<iostream>

MainWindow::MainWindow(QWidget * parent)
    :QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    //scene.reset(new battleScene);
    scene = new battleScene;
    ui->graphicsView->setScene(scene);

    //mapWidget.reset(new MapWidget);
    mapWidget = new MapWidget;
    scene->addWidget(mapWidget);
    //mapWidget->getPtrScene(scene);
    mapWidget->scene = scene;
    

    //scene->SetMapWidgetPtr(mapWidget);
    scene->mapWidget = mapWidget;
    
    //tokenController.reset(new TokenControllerWidget(mapWidget));
    tokenController = new TokenControllerWidget(mapWidget);
    ui->horizontalLayout_2->addWidget(tokenController);

    //mapWidget->getPtrTokCtl(tokenController);
    mapWidget->tokenController = tokenController;

    createActions();
    createMenus();

    setWindowTitle(tr("Shadowcasting"));
}

void MainWindow::newFile()
{
    delete tokenController;
    delete mapWidget;
    mapWidget = nullptr;
    tokenController = nullptr;
    scene->clear();
    mapWidget = new MapWidget;
    mapWidget->scene = scene;
    scene->addWidget(mapWidget);
    tokenController = new TokenControllerWidget(mapWidget);
    mapWidget->tokenController = tokenController;
    ui->horizontalLayout_2->addWidget(tokenController);

    updateMenus();
    return;
    
}

MainWindow::~MainWindow()
{

}

void MainWindow::clearImage()
{

    if(mapWidget)
    {
        mapWidget->clearImage();
    }

}

void MainWindow::switchDrawing()
{
    if (mapWidget)
    {
        mapWidget->switchDrawing();
    } 
}

void MainWindow::switchWalls()
{
    if (mapWidget)
    {
        mapWidget->switchDrawingWalls();
    }
}

void MainWindow::switchShadwos()
{
    if (mapWidget)
    {
        mapWidget->switchDrawingShadows();
    }
}

void MainWindow::switchBackground()
{
    if (mapWidget)
    {
        mapWidget->switchDrawingBackground();
    }
}


void MainWindow::openBackground()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath());
    if(!fileName.isEmpty() && mapWidget)
    {
        mapWidget->LoadBackground(fileName);
    }

}

void MainWindow::save()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                   QDir::currentPath() + "/untitled.sc",
                                   tr("ShadowCast files (*.sc);;All Files (*)"));
    if(!fileName.isEmpty() && mapWidget)
    {
        mapWidget->SaveToFile(fileName);
    }
    return;

}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),
                                                    QDir::currentPath(),
                                                    tr("ShadowCast files (*.sc);;All Files (*)"));


    if(!fileName.isEmpty() && mapWidget)
    {
        newFile();
        mapWidget->LoadFromFile(fileName);
    }
    return;
}

void MainWindow::penColor()
{

    QColor newColor = QColorDialog::getColor(mapWidget->penColor());

    if (newColor.isValid() && mapWidget)
        mapWidget->setPenColor(newColor);

}

void MainWindow::penWidth()
{

    bool ok;

    int newWidth = QInputDialog::getInt(this, tr("Drawing"),
                   tr("Select pen width:"), mapWidget->penWidth(), 1, 50, 1, &ok);
    if (ok && mapWidget)
        mapWidget->setPenWidth(newWidth);

}

void MainWindow::createActions()
{
    //Change all mapWiget to this, mapWiget changes to null after new file is valled
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()),
            this, SLOT(clearImage()));


    switchDrawingAct = new QAction(tr("&Drawing"), this);
    switchDrawingAct->setCheckable(true);
    switchDrawingAct->setChecked(true);
    connect(switchDrawingAct, SIGNAL(triggered()),
            this, SLOT(switchDrawing()));

    openGroupCreationAct = new QAction(tr("&Create group"), this);
    connect(openGroupCreationAct, SIGNAL(triggered()),
            this, SLOT(openCreateGroup()));

    openTokenCreationAct = new QAction(tr("&Create token"), this);
    connect(openTokenCreationAct, SIGNAL(triggered()),
            this, SLOT(openCreateToken()));


    loadBackgroundAct = new QAction(tr("Load background image"), this);
    connect(loadBackgroundAct, SIGNAL(triggered()),
            this, SLOT(openBackground()));

    switchWallsAct = new QAction(tr("Draw walls"), this);
    switchWallsAct->setCheckable(true);
    switchWallsAct->setChecked(true);
    connect(switchWallsAct, SIGNAL(triggered()),
            this, SLOT(switchWalls()));

    newFileAct = new QAction(tr("New file"), this);
    connect(newFileAct,SIGNAL(triggered()),
            this, SLOT(newFile()));

    loadAct = new QAction(tr("Load file"), this);
    connect(loadAct,SIGNAL(triggered()),
            this, SLOT(open()));

    saveAct = new QAction(tr("Save file"), this);
    connect(saveAct,SIGNAL(triggered()),
            this, SLOT(save()));

    switchShadowsAct = new QAction(tr("Draw shadows"), this);
    switchShadowsAct->setCheckable(true);
    switchShadowsAct->setChecked(true);
    connect(switchShadowsAct, SIGNAL(triggered()),
            this, SLOT(switchShadwos()));

    switchBackgroundAct = new QAction(tr("Draw background"), this);
    switchBackgroundAct->setCheckable(true);
    switchBackgroundAct->setChecked(true);
    connect(switchBackgroundAct, SIGNAL(triggered()),
            this, SLOT(switchBackground()));
}

// Create the menubar
void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(newFileAct);
    fileMenu->addAction(loadAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(loadBackgroundAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);
    optionMenu->addAction(switchDrawingAct);
    optionMenu->addAction(switchBackgroundAct);
    optionMenu->addAction(switchWallsAct);
    optionMenu->addAction(switchShadowsAct);
    optionMenu->addSeparator();
    optionMenu->addAction(openGroupCreationAct);
    optionMenu->addAction(openTokenCreationAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
}

void MainWindow::updateMenus()
{
    if (mapWidget)
    {
        switchBackgroundAct->setCheckable(mapWidget->drawingBackground);
        switchBackgroundAct->setChecked(mapWidget->drawingBackground);

        switchShadowsAct->setCheckable(mapWidget->drawingShadows);
        switchShadowsAct->setChecked(mapWidget->drawingShadows);

        switchWallsAct->setCheckable(mapWidget->drawingWalls);
        switchWallsAct->setChecked(mapWidget->drawingWalls);

        switchDrawingAct->setCheckable(mapWidget->drawingEnabled);
        switchDrawingAct->setChecked(mapWidget->drawingEnabled);
    }
}


void MainWindow::openCreateGroup()
{
    if (mapWidget)
    {
        //groupCreation.reset(new GroupCreationWindow(mapWidget));
        //groupCreation.reset(new GroupCreationWindow(mapWidget));
        groupCreation = new GroupCreationWindow(mapWidget);
        groupCreation->show();
    } 
}

void MainWindow::openCreateToken()
{
    if (mapWidget)
    {
        //tokenCreation.reset(new TokenCreationWidget(mapWidget));
        //tokenCreation.reset(new TokenCreationWidget(mapWidget));
        tokenCreation = new TokenCreationWidget(mapWidget);
        if(tokenCreation)
            tokenCreation->show();
    }
}
