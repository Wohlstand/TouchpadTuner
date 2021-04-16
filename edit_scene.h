#ifndef EDITSCENE_H
#define EDITSCENE_H

#include <QGraphicsScene>

class EditScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit EditScene(QObject *parent = nullptr);
    virtual ~EditScene();

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

signals:
    void onMouseRelease(QGraphicsSceneMouseEvent *e);
};

#endif // EDITSCENE_H
