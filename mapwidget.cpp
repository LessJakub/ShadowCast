#include <QtWidgets>
#include <exception>
#include <QMessageBox>
#include <QGraphicsScene>

#include"battlescene.h"
#include "mapwidget.h"
#include<iostream>
#include<QColor>
#include"token.h""
#include<memory>
#include<thread>
#include "tokencontrollerwidget.h"

class exceptionMessage : public std::exception
{
    QString errorMessage;
    QString detailedMessage;
public:
    exceptionMessage()
    {
        errorMessage = "File cannot be opend or does not exist";
    }
    exceptionMessage(QString text) : errorMessage(text)
    {

    }
    void setText(QString text)
    {
        errorMessage = text;
    }
    void setDetailedText(QString text)
    {
        detailedMessage = text;
    }
    void setMesessageBox(QMessageBox *messageBox)
    {
        messageBox->setText(errorMessage);
        messageBox->setIcon(QMessageBox::Critical);
        if(!detailedMessage.isEmpty())
        {
            messageBox->setDetailedText(detailedMessage);
        }
        messageBox->exec();
    }
} excMessage;


MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
{
    modified = false;
    drawing = false;
    drawingWalls = true;
    drawingEnabled = true;
    drawingShadows = true;
    drawingBackground = true;
    myPenWidth = 2;
    myPenColor = Qt::blue;
    tokenController = nullptr;

    setAutoFillBackground(false);

    fov.reset(new FieldOfView(this->width(), this->height()));
    walls = new QImage(this->width(), this->height(), QImage::Format_ARGB32);
    walls->fill(qRgba(0, 0, 0, 0));
    background = new QImage(this->width(), this->height(), QImage::Format_ARGB32);
    background->fill(QColorConstants::LightGray);
    defaultGroup = new Group("Default group");
    AddGroup(defaultGroup);
}

MapWidget::~MapWidget()
{
    fov.reset();
    //defaultGroup.reset();
    for (std::map<int, Group*>::iterator itr = mapOfGroups.begin();
        itr != mapOfGroups.end(); itr++)
    {
        delete itr->second;
    }
    for (std::map<int, Item*>::iterator itr = mapOfItems.begin();
        itr != mapOfItems.end(); itr++)
    {
        delete itr->second;
    }
    mapOfItems.clear();
    mapOfGroups.clear();
    delete walls;
    delete background;
}

void MapWidget::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void MapWidget::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void MapWidget::LoadBackground(QString fileName)
{
    QImage newBackground;

    if(!newBackground.load(fileName))
        return;
    this->resize(newBackground.size());
    *background = newBackground;
    clearImage();
    update();
}

void MapWidget::AddGroup(Group *group)
{
    if(group)
    {
        mapOfGroups.insert(std::pair<int, Group*>(group->groupId, group));
        if (tokenController)
        {
            tokenController->AddGroup(group);
        }
        fovChanged();
    }
}

void MapWidget::AddToken(Token* token)
{
    if(token)
    {
       token->getPtrFov(fov);
       scene->addItem(token);
       fovChanged();
    }
}

void MapWidget::AddToken(Item *item)
{
    if(item)
    {
        item->getPtrFov(fov);
        scene->addItem(item);
        mapOfItems.insert(std::pair<int,Item*>(item->id,item));
    }
}

void MapWidget::RemoveToken(Token *token)
{
    scene->removeItem(token);
}

void MapWidget::RemoveToken(Item *token)
{
    scene->removeItem(token);
}


void MapWidget::SaveToFile(QString fileName)
{
    try
    {
        if (fileName.isEmpty())
        {
            exceptionMessage exc("Problem while saving");
            exc.setDetailedText("File name is empty");
            throw exc;
        }

        QFile fileToSave(fileName);
        if (!fileToSave.open(QIODevice::WriteOnly))
        {
            exceptionMessage exc("Problem while saving");
            exc.setDetailedText("Can't open the file");
            throw exc;
        }

        QDataStream out(&fileToSave);
        out.setVersion(5);

        out << (qint32)FileTag;
        out << (qint32)fov->AllTiles[0].size();
        out << (qint32)fov->AllTiles.size();

        //modyfied, I added qint32
        out << *background;
        out << (qint32)FileTag;
        out << *walls;
        out << (qint32)FileTag;

        for (std::map<int, Item*>::iterator itrI = mapOfItems.begin();
            itrI != mapOfItems.end(); itrI++)
        {
            itrI->second->applyPreviousState();
        }


        for (int y = 0; y < fov->AllTiles.size(); y++)
        {
            for (int x = 0; x < fov->AllTiles[0].size(); x++)
            {
                if (fov->AllTiles[y][x]->IsWall())
                {
                    out << (qint32)x;
                    out << (qint32)y;
                }

            }
        }
        out << (qint32)FileTag;
        out << (qint32)FileTag;

        out << (qint32)mapOfGroups.size();
        for (std::map<int, Group*>::iterator itrG = mapOfGroups.begin();
            itrG != mapOfGroups.end(); itrG++)
        {
            out << (qint32)FileTag;
            out << *itrG->second;
            out << (qint32)itrG->second->tokensInGroup.size();
            for (std::map<int, Token*>::iterator itrT = itrG->second->tokensInGroup.begin();
                itrT != itrG->second->tokensInGroup.end(); itrT++)
            {
                out << (qint32)FileTag;
                out << *itrT->second;
            }
        }

        out << (qint32)FileTag;
        out << (qint32)mapOfItems.size();
        for (std::map<int, Item*>::iterator itrI = mapOfItems.begin();
            itrI != mapOfItems.end(); itrI++)
        {
            out << (qint32)FileTag;
            out << *itrI->second;
        }
    }
    catch (exceptionMessage& e)
    {
        QMessageBox fileLoadingError;
        e.setMesessageBox(&fileLoadingError);
    }
}

void MapWidget::LoadFromFile(QString fileName)
{
    try
    {
        QFile loadFile(fileName);
        if(!loadFile.open(QFile::ReadOnly))
        {
            exceptionMessage exc;
            throw exc;
        }

        QDataStream in(&loadFile);
        in.setVersion(5);

        qint32 fileCheck;
        in >> fileCheck;
        if (fileCheck != FileTag)
        {
            exceptionMessage exc("Error while loading a file");
            exc.setDetailedText("File is corupted or in not supported format");
            throw exc;
        }
        qint32 sizeX, sizeY;
        in >> sizeX;
        in >> sizeY;
        resize(sizeX, sizeY);

        QImage loadedBackground;
        in >> loadedBackground;
        *background = loadedBackground;
        in >> fileCheck;
        if (fileCheck != FileTag)
        {
            exceptionMessage exc("Error while loading a file");
            exc.setDetailedText("Error while loading background");
            throw exc;
        }
        QImage loadedWalls;
        in >> loadedWalls;
        *walls = loadedWalls;
        in >> fileCheck;
        if (fileCheck != FileTag)
        {
            exceptionMessage exc("Error while loading a file");
            exc.setDetailedText("Error while loading walls texture");
            throw exc;
        }
        qint32 x, y;
        while (true)
        {
            in >> x;
            in >> y;
            if ((x == FileTag || y == FileTag) || loadFile.atEnd())
            {
                if (loadFile.atEnd())
                {
                    exceptionMessage exc("Error while loading a file");
                    exc.setDetailedText("Error while loading walls, file ended unexpectedly");
                    throw exc;
                }
                else
                {
                    break;
                } 
            }
            fov->AllTiles[y][x]->wall = true;
        }
        tokenController->list = false;
        mapOfGroups.clear();
        tokenController->ClearGroups();

        qint32 numberOfGroups;
        in >> numberOfGroups;
        for (int g = 0; g < numberOfGroups; g++)
        {
            in >> fileCheck;
            if(fileCheck != FileTag)
            {
                exceptionMessage exc("Error while loading a file");
                exc.setDetailedText("Error while loading groups");
                throw exc;
            }
            //std::shared_ptr<Group> tmpG;
            //tmpG.reset(new Group);
            Group* tmpG = new Group;
            in >> *tmpG;
            tmpG->variantOfThis->setValue(tmpG->groupId);
            AddGroup(tmpG);
            qint32 numberOfTokens;
            in >> numberOfTokens;
            for (int t = 0; t < numberOfTokens; t++)
            {
                in >> fileCheck;
                if(fileCheck != FileTag)
                {
                    exceptionMessage exc("Error while loading a file");
                    exc.setDetailedText("Error while loading tokens");
                    throw exc;
                }
                //std::shared_ptr<Token> tmpT;
                //tmpT.reset(new Token);
                Token* tmpT = new Token;
                in >> *tmpT;
                AddToken(tmpT);
                tmpG->AddToGroup(tmpT);
            }

        }
        in >> fileCheck;
        if (fileCheck != FileTag)
        {
            exceptionMessage exc("Error while loading a file");
            exc.setDetailedText("Error while loading items");
            throw exc;
        }
        qint32 numberOfItems;
        in >> numberOfItems;
        for (int i = 0; i < numberOfItems; i++)
        {
            in >> fileCheck;
            if (fileCheck != FileTag)
            {
                exceptionMessage exc("Error while loading a file");
                exc.setDetailedText("Error while loading items");
                throw exc;
            }
            Item* tmp = new Item;
            in >> *tmp;
            AddToken(tmp);
        }



        tokenController->list = true;
        tokenController->ListTokens();
        for (std::map<int, Item*>::iterator itrI = mapOfItems.begin();
            itrI != mapOfItems.end(); itrI++)
        {
            itrI->second->moveWalls();
        }
    }
    catch (exceptionMessage& e)
    {
        QMessageBox fileLoadingError;
        e.setMesessageBox(&fileLoadingError);
    }   
}

void MapWidget::fovChanged()
{
    if (fov)
    {
        fov->fovChanged();
    }
}

void MapWidget::clearImage()
{
    walls->fill(qRgba(0,0,0,0));
    fov->HideWalls();
    modified = true;
    fovChanged();
    update();
}

void MapWidget::switchDrawing()
{
    if(drawingEnabled)
    {
        drawingEnabled = false;
    }
    else
    {
        drawingEnabled = true;
    }
}

void MapWidget::switchDrawingWalls()
{
    if(drawingWalls)
    {
        drawingWalls = false;
    }
    else
    {
        drawingWalls = true;
    }
}

void MapWidget::switchDrawingShadows()
{
    fovChanged();
    if(drawingShadows)
    {
        drawingShadows = false;
    }
    else
    {
        drawingShadows = true;
    }
}

void MapWidget::switchDrawingBackground()
{
    if(drawingBackground)
    {
        drawingBackground = false;
    }
    else
    {
        drawingBackground = true;
    }
}


void MapWidget::mousePressEvent(QMouseEvent *event)
{
    //if (drawingEnabled && (event->button() == Qt::LeftButton))
    if ((event->button() == Qt::LeftButton))
    {
        lastPoint = event->pos();
        drawing = true;
    }
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    //if (drawingEnabled && (event->buttons() & Qt::LeftButton) && drawing)
    if ((event->buttons() & Qt::LeftButton) && drawing)
        drawLineTo(event->pos());
}

void MapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //if (drawingEnabled && (event->button() == Qt::LeftButton && drawing))
    if ((event->button() == Qt::LeftButton && drawing))
    {
        drawLineTo(event->pos());
        drawing = false;
    }
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect = event->rect();
     
     if(fov->changed && tokenController && drawingShadows)
     {
         fov->HideTiles();
         std::map<int, Group*>::iterator chosenGroup = mapOfGroups.find(tokenController->chosenGroup);
         if (chosenGroup != mapOfGroups.end())
         {
             fov->Scan(chosenGroup->second);
             checkVisibleTokens();
             checkVisibleItems();

         }
     }
     else if(!drawingShadows)
     {
        fov->RevealTiles();
        checkVisibleTokens();
        checkVisibleItems();
     }
    if(drawingBackground)
    {
        painter.drawImage(rect, *background, rect);
    }

    if(drawingWalls)
    {
        
       painter.drawImage(rect, *walls, rect);
    }
    if(drawingShadows)
    {
        painter.drawImage(rect, *fov->shadows, rect);
    }
    
    update();
}

void MapWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeMap();
}

void MapWidget::drawLineTo(const QPoint &endPoint)
{
    if(drawingEnabled)
    {
        QPainter painter(walls);
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        painter.drawLine(lastPoint, endPoint);
        if (fov->InBoundries(lastPoint.x(), lastPoint.y()) && fov->InBoundries(endPoint.x(), endPoint.y()))
        {
            fov->SetWallLine(true,fov->AllTiles[lastPoint.y()][lastPoint.x()], fov->AllTiles[endPoint.y()][endPoint.x()], myPenWidth);
        }
        

        modified = true;

        int rad = (myPenWidth / 2) + 2;

        update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

        lastPoint = endPoint;
        fovChanged();
    }
    else
    {
        QPainter painter(walls);
        painter.setCompositionMode(QPainter::CompositionMode_Clear); 
        painter.setPen(QPen(QColor(0,0,0,0), myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        painter.drawLine(lastPoint, endPoint);

        if (fov->InBoundries(lastPoint.x(), lastPoint.y()) && fov->InBoundries(endPoint.x(), endPoint.y()))
        {
            fov->SetWallLine(false, fov->AllTiles[lastPoint.y()][lastPoint.x()], fov->AllTiles[endPoint.y()][endPoint.x()], myPenWidth);
        }
        modified = true;

        int rad = (myPenWidth / 2) + 2;

        update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

        lastPoint = endPoint;
        fovChanged();
    }
}

void MapWidget::resizeMap()
{
    fov.reset(new FieldOfView(this->width(), this->height()));
    delete walls;
    walls = new QImage(this->width(), this->height(), QImage::Format_ARGB32);
    walls->fill(qRgba(0, 0, 0, 0));
}

void MapWidget::checkVisibleItems()
{
    for (std::map<int, Item*>::iterator itr = mapOfItems.begin();
        itr != mapOfItems.end(); itr++)
    {
        itr->second->checkIfVisible();
    }
}

void MapWidget::checkVisibleTokens()
{
    for(std::map<int, Group*>::iterator itrG = mapOfGroups.begin(); itrG
        != mapOfGroups.end(); itrG ++)
    {
        if(!itrG->second->alwaysVisable)
        {
            for(std::map<int, Token*>::iterator itrT = itrG->second->tokensInGroup.begin();
                itrT != itrG->second->tokensInGroup.end(); itrT++)
            {
                if(itrG->first != tokenController->chosenGroup)
                {
                    if(!itrT->second->alwaysVisable)
                    {
                        if(fov->AllTiles[itrT->second->pos().y()][itrT->second->pos().x()]->reveald)
                        {
                            itrT->second->setVisible(true);
                        }
                        else
                        {
                            itrT->second->setVisible(false);
                        }
                    }
                    else
                    {
                        itrT->second->setVisible(true);
                    }
                }
                else
                {
                    itrT->second->setVisible(true);
                }
            }
        }
    }
}



