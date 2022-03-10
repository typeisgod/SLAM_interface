#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include "launchwidget.h"
#include "sensorlaunchtableview.h"

class SensorWidget : public LaunchWidget
{
    Q_OBJECT
public:
    explicit SensorWidget(QWidget *parent = nullptr);
    virtual void saveCurrentConfig(QSettings *settings, const QString &group);
    virtual void loadConfig(QSettings *settings, const QString &group);
};

#endif // SENSORWIDGET_H
