#include "analogclock.h"
#include "ui_analogclock.h"

AnalogClock::AnalogClock(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnalogClock)
{
    ui->setupUi(this);

    // 实例化一个计时器，绑定给 QTimer 指针 timer
    QTimer* timer = new QTimer(this);
    // connect 链接槽函数，信号源 timer 发出信号 timeout，对象 AnalogClock 使用函数 update()处理（其实是调用 update() 之后，自动触发 paintEvent() 重新绘制页面）
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&AnalogClock::update));
    // 设置时间间隔为 1000 毫秒，开始计时
    timer->start(1000);

    // 设置 Widget 标题
    setWindowTitle(tr("Analog Clock"));
    // 重置 Widget 窗口为 200 像素的正方形
    resize(200, 200);
}

AnalogClock::~AnalogClock()
{
    delete ui;
}

void AnalogClock::paintEvent(QPaintEvent *) {
    static const QPoint hourHand[4] = {
        QPoint(5, 14),
        QPoint(-5, 14),
        QPoint(-4, -71),
        QPoint(4, -71)
    };
    static const QPoint minuteHand[4] = {
        QPoint(4, 14),
        QPoint(-4, 14),
        QPoint(-3, -89),
        QPoint(3, -89)
    };
    static const QPoint secondsHand[4] = {
        QPoint(1, 14),
        QPoint(-1, 14),
        QPoint(-1, -89),
        QPoint(1, -89)
    };

    const QColor hourColor(palette().color(QPalette::Text));
    const QColor minuteColor(palette().color(QPalette::Text));
    const QColor secondsColor(palette().color(QPalette::Highlight)); // Qt 6.7 文档中是 QPalete::Accent

    int side = qMin(width(), height());

    //
    QPainter painter(this);
    QTime time = QTime::currentTime();

    // Antialiasing 抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    // translate 平移
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    // Qt::NoPen 可以理解为“无边框”
    painter.setPen(Qt::NoPen);
    // 将笔刷换成时针样式
    // 这里 ide 没有提示错，虽然 setBrush 没有 QColor 的版本，但可以提供 QColor 给编译器，自动生成 QBrush 入参：QBrush(const QColor& color, Qt::BrushStyle style = Qt::SoldPattern)，其中第二个参数有缺省值，所以不需要填
    painter.setBrush(hourColor);

    // 绘制时针
    // 先使用 save() 将 painter 当前的状态压入栈
    painter.save();
    // 已知每小时偏转 30°，计算当前的小时（算上分钟，会存在小数）的偏转角度
    painter.rotate(30.0 * (time.hour() + time.minute() / 60.0));
    // 绘制四个顶点的凸多边形，hourHand 也是先自动生成为 QPoint 类，然后再自动进一步生成 QPointF 作为入参
    painter.drawConvexPolygon(hourHand, 4);
    // 将更新后的 painter 状态弹出栈
    painter.restore();

    // 绘制表盘上 12 个小时节点
    // 为什么这里不需要 save-restore 呢？因为本来就需要 12 个节点，而不像上面小时指针无论何时都只有一根
    for (int i = 0; i < 12; ++i) {
        painter.drawRect(73, -3, 16, 6);
        painter.rotate(30.0);
    }

    // 将笔刷换成分针样式
    painter.setBrush(minuteColor);

    // 同理绘制分针
    painter.save();
    painter.rotate(6.0 * time.minute());
    painter.drawConvexPolygon(minuteHand, 4);
    painter.restore();

    // 切换笔刷为秒针
    painter.setBrush(secondsColor);

    // 同理绘制秒针
    painter.save();
    painter.rotate(6.0 * time.second());
    painter.drawConvexPolygon(secondsHand, 4);
    // 由于秒针太细，需要做一个显眼的圆球来标识（与现实中的一些秒针样式相同）
    // 指定一个初始点位，和 drawConvexPolygon() 一样，根据 rotate 计算偏转角
    painter.drawEllipse(-3, -3, 6, 6);
    painter.drawEllipse(-5, -68, 10, 10);
    painter.restore();

    //
    painter.setPen(minuteColor);

    // 绘制 60 个分钟节点
    for (int j = 0; j < 60; ++j) {
        painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }


}
