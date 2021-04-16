#include "edit_scene.h"
#include <QGraphicsSceneMouseEvent>

EditScene::EditScene(QObject *parent) :
    QGraphicsScene(parent)
{}

EditScene::~EditScene()
{}

void EditScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene::mouseReleaseEvent(e);
    emit onMouseRelease(e);
}
