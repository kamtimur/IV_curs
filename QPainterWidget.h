#ifndef QPAINTERWIDGET_H
#define QPAINTERWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QTimer>

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPixmap>

class QPainterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPainterWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public:
    enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                 Chord, Pie, Path, Text, Pixmap };

signals:

public slots:
    void setShape(Shape shape);
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setTransformed(bool transformed);

    void interrupt(int32_t* data);

protected:
    void paintEvent(QPaintEvent *)          override;
    void timerEvent(QTimerEvent *event)     override;
    virtual void resizeEvent(QResizeEvent *event)   override;

    bool event(QEvent *event) override;

//public slots:
//    void animate();


private:
    QPainter painter; // Создаём новый объект рисовальщика

    Shape shape;
    QPen pen;
    QBrush brush;
    bool antialiased;
    bool transformed;
    QPixmap pixmap;


protected:
    static const int chM    = 8;
    static const int rail   = 2000;
    int32_t buf[chM][rail];
    int pnt;
    int time;

    QTimer* timer;
//    bool    fr;
//    int     rcnt;
};


#endif // QPAINTERWIDGET_H
