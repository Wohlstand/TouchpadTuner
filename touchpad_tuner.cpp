#include "touchpad_tuner.h"
#include "ui_touchpad_tuner.h"
#include <QGraphicsRectItem>

TouchpadTuner::TouchpadTuner(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TouchpadTuner)
{
    ui->setupUi(this);
    m_scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(m_scene);
    m_scene->setSceneRect(0, 0, ui->width->value(), ui->height->value());
    QObject::connect(m_scene, &QGraphicsScene::selectionChanged, this, &TouchpadTuner::selectionChanged);
    initScene();
}

TouchpadTuner::~TouchpadTuner()
{
    delete ui;
}

void TouchpadTuner::initScene()
{
    m_scene->clear();
    m_back = m_scene->addRect(0, 0, ui->width->value(), ui->height->value(), QPen(Qt::black), QBrush(Qt::lightGray));
    m_back->setZValue(-30);
    syncSize();

    memcpy(&touchKeysMapChanged, &touchKeysMap, sizeof(touchKeysMap));

    for(int i = key_BEGIN; i < key_END; ++i)
    {
        auto &k = touchKeysMapChanged[i];
        auto *r = m_scene->addRect(k.x1, k.y1, k.x2 - k.x1, k.y2 - k.y1, QPen(Qt::black), QBrush(Qt::green));
        r->setZValue(10);
        r->setFlag(QGraphicsItem::ItemIsMovable, true);
        r->setFlag(QGraphicsItem::ItemIsSelectable, true);
        r->setData(0, i);
    }
}

void TouchpadTuner::syncSize()
{
    ui->graphicsView->resize(ui->width->value() + 20, ui->height->value() + 20);
    m_scene->setSceneRect(0, 0, ui->width->value(), ui->height->value());
    m_back->setRect(0, 0, ui->width->value(), ui->height->value());
}

void TouchpadTuner::on_width_valueChanged(int)
{
    syncSize();
}

void TouchpadTuner::on_height_valueChanged(int)
{
    syncSize();
}

void TouchpadTuner::on_reset_clicked()
{
    initScene();
}

void TouchpadTuner::currentSyncFields()
{
    ui->itemx->setDisabled(true);
    ui->itemy->setDisabled(true);
    ui->itemwidth->setDisabled(true);
    ui->itemheight->setDisabled(true);

    if(!m_current)
        return;

    auto rekt = m_current->rect();

    ui->itemx->blockSignals(true);
    ui->itemx->setValue(rekt.x());
    ui->itemx->blockSignals(false);

    ui->itemy->blockSignals(true);
    ui->itemy->setValue(rekt.y());
    ui->itemy->blockSignals(false);

    ui->itemwidth->blockSignals(true);
    ui->itemwidth->setValue(rekt.width());
    ui->itemwidth->blockSignals(false);

    ui->itemheight->blockSignals(true);
    ui->itemheight->setValue(rekt.height());
    ui->itemheight->blockSignals(false);

    ui->itemx->setDisabled(false);
    ui->itemy->setDisabled(false);
    ui->itemwidth->setDisabled(false);
    ui->itemheight->setDisabled(false);
}

void TouchpadTuner::currentSyncRaw()
{
    if(!m_current)
        return;
    touchKeysMapChanged[m_current->data(0).toInt()].x1 = qRound(m_current->x());
    touchKeysMapChanged[m_current->data(0).toInt()].x2 = qRound(m_current->x() + m_current->rect().width());
    touchKeysMapChanged[m_current->data(0).toInt()].y1 = qRound(m_current->y());
    touchKeysMapChanged[m_current->data(0).toInt()].y2 = qRound(m_current->y() + m_current->rect().height());
}

void TouchpadTuner::selectionChanged()
{
    m_current = nullptr;
    auto l = m_scene->selectedItems();

    ui->itemx->setDisabled(true);
    ui->itemy->setDisabled(true);
    ui->itemwidth->setDisabled(true);
    ui->itemheight->setDisabled(true);

    for(auto *i : l)
    {
        auto *r = qgraphicsitem_cast<QGraphicsRectItem*>(i);
        if(!r)
            continue;
        m_current = r;
        auto rekt = r->rect();
        ui->itemx->blockSignals(true);
        ui->itemx->setValue(rekt.x());
        ui->itemx->blockSignals(false);

        ui->itemy->blockSignals(true);
        ui->itemy->setValue(rekt.y());
        ui->itemy->blockSignals(false);

        ui->itemwidth->blockSignals(true);
        ui->itemwidth->setValue(rekt.width());
        ui->itemwidth->blockSignals(false);

        ui->itemheight->blockSignals(true);
        ui->itemheight->setValue(rekt.height());
        ui->itemheight->blockSignals(false);

        ui->itemx->setDisabled(false);
        ui->itemy->setDisabled(false);
        ui->itemwidth->setDisabled(false);
        ui->itemheight->setDisabled(false);
        break;
    }
}


void TouchpadTuner::on_itemx_valueChanged(int arg1)
{
    if(m_current)
    {
        auto r = m_current->rect();
        r.moveTo(arg1, r.y());
        m_current->setRect(r);
        currentSyncRaw();
    }
}

void TouchpadTuner::on_itemy_valueChanged(int arg1)
{
    if(m_current)
    {
        auto r = m_current->rect();
        r.moveTo(r.x(), arg1);
        m_current->setRect(r);
        currentSyncRaw();
    }
}

void TouchpadTuner::on_itemwidth_valueChanged(int arg1)
{
    if(m_current)
    {
        auto r = m_current->rect();
        r.setWidth(arg1);
        m_current->setRect(r);
        currentSyncRaw();
    }
}

void TouchpadTuner::on_itemheight_valueChanged(int arg1)
{
    if(m_current)
    {
        auto r = m_current->rect();
        r.setHeight(arg1);
        m_current->setRect(r);
        currentSyncRaw();
    }
}

void TouchpadTuner::on_scaleDown_clicked()
{
    if(m_current)
    {
        auto r = m_current->rect();
        r.setWidth(r.width() - 1);
        r.setHeight(r.height() - 1);
        m_current->setRect(r);
        currentSyncFields();
        currentSyncRaw();
    }
}

void TouchpadTuner::on_scaleUp_clicked()
{
    if(m_current)
    {
        auto r = m_current->rect();
        r.setWidth(r.width() + 1);
        r.setHeight(r.height() + 1);
        m_current->setRect(r);
        currentSyncFields();
        currentSyncRaw();
    }
}

void TouchpadTuner::on_adjustUp_clicked()
{
    if(m_current)
    {
        auto r = m_current->rect();
        r.moveTo(r.x(), r.y() - 1);
        m_current->setRect(r);
        currentSyncFields();
        currentSyncRaw();
    }
}

void TouchpadTuner::on_adjustDown_clicked()
{
    if(m_current)
    {
        auto r = m_current->rect();
        r.moveTo(r.x(), r.y() + 1);
        m_current->setRect(r);
        currentSyncFields();
        currentSyncRaw();
    }
}

void TouchpadTuner::on_adjustLeft_clicked()
{
    if(m_current)
    {
        auto r = m_current->rect();
        r.moveTo(r.x() - 1, r.y());
        m_current->setRect(r);
        currentSyncFields();
        currentSyncRaw();
    }
}

void TouchpadTuner::on_adjustRight_clicked()
{
    if(m_current)
    {
        auto r = m_current->rect();
        r.moveTo(r.x() + 1, r.y());
        m_current->setRect(r);
        currentSyncFields();
        currentSyncRaw();
    }
}

void TouchpadTuner::on_save_clicked()
{

}

void TouchpadTuner::on_load_clicked()
{

}
