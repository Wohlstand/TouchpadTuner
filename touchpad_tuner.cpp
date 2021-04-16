#include "touchpad_tuner.h"
#include "ui_touchpad_tuner.h"
#include <QGraphicsRectItem>
#include <QFileDialog>
#include <QSettings>
#include <QtDebug>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include "edit_scene.h"

TouchpadTuner::TouchpadTuner(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TouchpadTuner)
{
    ui->setupUi(this);
    m_scene = new EditScene(ui->graphicsView);
    ui->graphicsView->setScene(m_scene);
    m_scene->setSceneRect(0, 0, ui->width->value(), ui->height->value());
    QObject::connect(m_scene, &QGraphicsScene::selectionChanged, this, &TouchpadTuner::selectionChanged);
    QObject::connect(m_scene, &EditScene::onMouseRelease, [this](QGraphicsSceneMouseEvent*)->void
    {
        if(!m_current)
            return;
        currentSyncFields();
        currentSyncRaw();
    });
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
        auto *r = m_scene->addRect(0, 0, k.x2 - k.x1, k.y2 - k.y1, QPen(Qt::black), QBrush(Qt::green));
        r->setPos(k.x1, k.y1);
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
    ui->itemx->setValue(m_current->x());
    ui->itemx->blockSignals(false);

    ui->itemy->blockSignals(true);
    ui->itemy->setValue(m_current->y());
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

    auto &k = touchKeysMapChanged[m_current->data(0).toInt()];
    auto r = m_current->rect();
    k.x1 = qRound(m_current->x());
    k.y1 = qRound(m_current->y());
    k.x2 = qRound(m_current->x() + r.width());
    k.y2 = qRound(m_current->y() + r.height());

    qDebug() << "X1:" << k.x1 << "Y1:" <<k.y1
             << "X2:" << k.x2 << "Y2:" <<k.y2;
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
        ui->itemx->setValue(m_current->x());
        ui->itemx->blockSignals(false);

        ui->itemy->blockSignals(true);
        ui->itemy->setValue(m_current->y());
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
        m_current->setPos(arg1, m_current->y());
        currentSyncRaw();
    }
}

void TouchpadTuner::on_itemy_valueChanged(int arg1)
{
    if(m_current)
    {
        m_current->setPos(m_current->x(), arg1);
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
        m_current->setPos(m_current->x(), m_current->y() - 1);
        currentSyncFields();
        currentSyncRaw();
    }
}

void TouchpadTuner::on_adjustDown_clicked()
{
    if(m_current)
    {
        m_current->setPos(m_current->x(), m_current->y() + 1);
        currentSyncFields();
        currentSyncRaw();
    }
}

void TouchpadTuner::on_adjustLeft_clicked()
{
    if(m_current)
    {
        m_current->setPos(m_current->x() - 1, m_current->y());
        currentSyncFields();
        currentSyncRaw();
    }
}

void TouchpadTuner::on_adjustRight_clicked()
{
    if(m_current)
    {
        m_current->setPos(m_current->x() + 1, m_current->y());
        currentSyncFields();
        currentSyncRaw();
    }
}

void TouchpadTuner::on_save_clicked()
{
    QString o = QFileDialog::getSaveFileName(this, tr("Load touchpad settings"), m_lastFile, "Touchpad (*.ptpad);;All (*.*)");
    if(o.isEmpty())
        return;

    QSettings f(o, QSettings::IniFormat);

    f.beginGroup("setup");
    for(int i = key_BEGIN; i < key_END; ++i)
    {
        auto &s = touchKeysMapChanged[i];
        f.setValue(QString("%1-x1").arg(i), s.x1);
        f.setValue(QString("%1-y1").arg(i), s.y1);
        f.setValue(QString("%1-x2").arg(i), s.x2);
        f.setValue(QString("%1-y2").arg(i), s.y2);
    }
    f.endGroup();

    f.sync();

    memcpy(&touchKeysMap, &touchKeysMapChanged, sizeof(touchKeysMap));
    m_lastFile = o;
}

void TouchpadTuner::on_load_clicked()
{
    QString o = QFileDialog::getOpenFileName(this, tr("Load touchpad settings"), m_lastFile, "Touchpad (*.ptpad);;All (*.*)");
    if(o.isEmpty())
        return;

    QSettings f(o, QSettings::IniFormat);

    f.beginGroup("setup");
    for(int i = key_BEGIN; i < key_END; ++i)
    {
        auto &s = touchKeysMap[i];
        s.x1 = f.value(QString("%1-x1").arg(i), 0.f).toFloat();
        s.y1 = f.value(QString("%1-y1").arg(i), 0.f).toFloat();
        s.x2 = f.value(QString("%1-x2").arg(i), 0.f).toFloat();
        s.y2 = f.value(QString("%1-y2").arg(i), 0.f).toFloat();
        s.cmd = static_cast<commands>(i);
    }
    f.endGroup();

    initScene();
    m_lastFile = o;
}

void TouchpadTuner::on_exportToCpp_clicked()
{
    QString clName = "TouchScreenController";
    QStringList keyNames =
    {
        "key_start",
        "key_left",
        "key_right",
        "key_up",
        "key_down",
        "key_upleft",
        "key_upright",
        "key_downleft",
        "key_downright",
        "key_run",
        "key_jump",
        "key_altrun",
        "key_altjump",
        "key_drop",
        "key_holdRun",
        "key_toggleKeysView",
    };

    QString output;

    output +=
            "{\n"
            "    /* Note that order of keys must match the TouchScreenController::commands enum!!! */\n";

    for(int i = key_BEGIN; i < key_END; ++i)
    {
        auto &s = touchKeysMap[i];
        output += QString("    {%1.0f, %2.0f, %3.0f, %4.0f, %5:%6},\n")
                .arg(s.x1)
                .arg(s.y1)
                .arg(s.x2)
                .arg(s.y2)
                .arg(clName)
                .arg(keyNames[i]);
    }

    output += "};";

    QMessageBox::information(this, "Exported array", output);
}
