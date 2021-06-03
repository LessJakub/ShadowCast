#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

//#include "battlescene.h"
#include "FieldOfView.h"
#include "group.h"
#include "item.h"

class TokenControllerWidget;
class battleScene;

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    MapWidget(QWidget *parent = 0);
    ~MapWidget();

    //Sets colour of pen to newColor
    void setPenColor(const QColor &newColor);

    //Sets pen width to newWidth
    void setPenWidth(int newWidth);

    //Loads background from fileName
    void LoadBackground(QString fileName);

    
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

    //Adds group to mapOfGroups
    void AddGroup(Group* group);

    //Adds token to scene and gives it a reference to FoV
    void AddToken(Token* token);

    //Adds item to scene and mapOfItems, gives it a reference to FoV
    void AddToken(Item * item);

    //Removes token from the scene
    void RemoveToken(Token * token);

    //Removes Item from the scene
    void RemoveToken(Item * token);
    
    //Saves project to file with fileName
    void SaveToFile(QString fileName);

    //Clears project and loads project from fileName
    void LoadFromFile(QString fileName);

    //Changes attribute "changed" of fov
    void fovChanged();

    //default group created in constructor of this object
    Group* defaultGroup;

    //map with every group in the project
    std::map<int, Group*> mapOfGroups;

    //map with every item in the project
    std::map<int, Item*> mapOfItems;

    //pointer to scene
    battleScene* scene;

    //Pointer to tokenController
    TokenControllerWidget* tokenController;

    //Clears current image, resets walls
    void clearImage();

    //Switchs drawing to erasing
    void switchDrawing();

    //switches drawing walls
    void switchDrawingWalls();

    //switches drawing shadows and scanig
    void switchDrawingShadows();

    //switches drawing background
    void switchDrawingBackground();

    //If true drawing mode is enabled, if false erasing is enabled
    bool drawingEnabled;
    
    //Curently drawing/erasing
    bool drawing;

    //if true walls will be displayed, if false walls wont be displayed
    bool drawingWalls;

    //if true shadows are displayed
    bool drawingShadows;

    //if true background is displayed
    bool drawingBackground;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    //Draws textures and calls scaning for choosen group
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    enum {FileTag = 0x98c58f26 };

    //Draws line
    void drawLineTo(const QPoint &endPoint);

    //Resets and resizes fov
    void resizeMap();

    //Checks what item is visable currently
    void checkVisibleItems();

    //check what token is visable currently
    void checkVisibleTokens();


    bool modified;


    int myPenWidth;
    QColor myPenColor;

    QImage *background;
    QImage * walls;

    QPoint lastPoint;

    //Pointer to field of view
    std::shared_ptr<FieldOfView> fov;
};

#endif
