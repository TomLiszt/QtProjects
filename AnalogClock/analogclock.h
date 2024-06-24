#ifndef ANALOGCLOCK_H
#define ANALOGCLOCK_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class AnalogClock; }
QT_END_NAMESPACE

class AnalogClock : public QWidget
{
    Q_OBJECT

public:
    AnalogClock(QWidget *parent = nullptr);
    ~AnalogClock();

private:
    Ui::AnalogClock *ui;

protected:
    void paintEvent(QPaintEvent *event) override;
};
#endif // ANALOGCLOCK_H
