#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <roscorewidget.h>
#include <sensorwidget.h>
#include <slamwidget.h>
#include <QSettings>
#include <QMessageBox>
#include <QScrollArea>
#include <QMenuBar>
#include <QAction>
#include <QScreen>
#include "grouplaunchwidget.h"
#include "components/qtmaterialtoggle.h"
#include "components/qtmaterialscrollbar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private:
    void saveCurrentConfig();
    void loadConfig(const QString &setting_name);
    void modifyGroupLaunchWidgetSize();

    Ui::MainWindow *ui;

    QRect screen_size;
    int max_group_launch_widget_height;
    bool use_compact_layout = false;

    QMenuBar *main_menu;
    QMenu *menu_file;
    QAction *action_open_config;
    QAction *action_save_config;
    QAction *action_saveas_config;

    QFileDialog *dialog_open_config;
    QFileDialog *dialog_saveas_config;

    QLabel *label_compact;
    QtMaterialToggle *toggle_compact_layout;

    RoscoreWidget *roscore_widget;

    QLabel *label_name_sensor_group;
    QLabel *label_name_tool_group;

    GroupLaunchWidget *sensor_group_widget;
    GroupLaunchWidget *tool_group_widget;

    QScrollArea *scroll_area_sensor_group;
    QScrollArea *scroll_area_tool_group;

    QtMaterialScrollBar *scrollbar_sensor_group_hori;
    QtMaterialScrollBar *scrollbar_sensor_group_vert;
    QtMaterialScrollBar *scrollbar_tool_group_hori;
    QtMaterialScrollBar *scrollbar_tool_group_vert;

    QGridLayout *layout;

    std::vector<std::unordered_set<SensorWidget*>> sensor_widget_array;
    std::unordered_set<SlamWidget*> slam_widget_array;

    SensorWidget *camera_widget;
    SensorWidget *lidar_widget;
    SensorWidget *imu_widget;
    SlamWidget *slam_widget;

    QString default_setting_path;
    QString default_setting_name;
    QString current_setting_path;
    QString current_setting_name;

protected slots:
    void onNewLaunchWidgetAdded(LaunchWidget *wid);
    void onLaunchWidgetRemoved(LaunchWidget *wid);
    void onOpenConfigClicked();
    void onSaveAsConfigClicked();
    void onToggled(bool tog);
};
#endif // MAINWINDOW_H
