#ifndef TOUCHPADTUNER_H
#define TOUCHPADTUNER_H

#include <QMainWindow>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class TouchpadTuner; }
class QGraphicsRectItem;
QT_END_NAMESPACE

class EditScene;

class TouchpadTuner : public QMainWindow
{
    Q_OBJECT
    EditScene *m_scene;
    QGraphicsRectItem *m_back;
    QGraphicsRectItem *m_current = nullptr;
    QString m_lastFile;

    enum commands
    {
        key_BEGIN = 0,
        key_start = 0,
        key_left,
        key_right,
        key_up,
        key_down,
        key_upleft,
        key_upright,
        key_downleft,
        key_downright,
        key_run,
        key_jump,
        key_altrun,
        key_altjump,
        key_drop,
        key_holdRun,
        key_toggleKeysView,
        key_END
    };

    struct KeyPos
    {
        float x1;
        float y1;
        float x2;
        float y2;
        commands cmd;
    };

    KeyPos touchKeysMap[key_END] =
    {
        /* Note that order of keys must match the commands enum!!! */
        {542.0f, 537.0f, 693.0f,  587.0f, key_start},
        {1.0f, 410.0f, 83.0f,  492.0f, key_left},
        {165.0f, 410.0f, 247.0f,  492.0f, key_right},
        {83.0f, 328.0f, 165.0f,  410.0f, key_up},
        {83.0f, 492.0f, 165.0f,  574.0f, key_down},
        {1.0f, 328.0f, 83.0f,  410.0f, key_upleft},
        {165.0f, 328.0f, 247.0f,  410.0f, key_upright},
        {1.0f, 492.0f, 83.0f,  574.0f, key_downleft},
        {165.0f, 492.0f, 247.0f,  574.0f, key_downright},
        {770.0f, 396.0f, 877.0f,  487.0f, key_run},
        {888.0f, 431.0f, 995.0f,  522.0f, key_jump},
        {780.0f, 290.0f, 887.0f,  381.0f, key_altrun},
        {898.0f, 325.0f, 1005.0f,  416.0f, key_altjump},
        {331.0f, 537.0f, 482.0f,  587.0f, key_drop},
        {807.0f, 150.0f, 914.0f,  180.0f, key_holdRun},
        {10.0f, 10.0f, 70.0f,  70.0f, key_toggleKeysView},
    };

    KeyPos touchKeysMapChanged[key_END];

public:
    TouchpadTuner(QWidget *parent = nullptr);
    ~TouchpadTuner();

    void initScene();
    void syncSize();

private slots:
    void on_width_valueChanged(int arg1);
    void on_height_valueChanged(int arg1);

    void on_reset_clicked();

    void currentSyncFields();
    void currentSyncRaw();

    void selectionChanged();
    void on_itemx_valueChanged(int arg1);
    void on_itemy_valueChanged(int arg1);
    void on_itemwidth_valueChanged(int arg1);
    void on_itemheight_valueChanged(int arg1);

    void on_scaleDown_clicked();
    void on_scaleUp_clicked();
    void on_adjustUp_clicked();
    void on_adjustDown_clicked();
    void on_adjustLeft_clicked();
    void on_adjustRight_clicked();

    void on_save_clicked();

    void on_load_clicked();

    void on_exportToCpp_clicked();

private:
    Ui::TouchpadTuner *ui;
};
#endif // TOUCHPADTUNER_H
