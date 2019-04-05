#include "QPainterWidget.h"
#include <QDebug>
#include <QtMath>
#include <QWindow>
#include <QtGui>


QPainterWidget::QPainterWidget(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumSize(0,0);

    //this->setFrameStyle(54); //(QFrame::Box | QFrame::Sunken);
    //this->setLineWidth(1);

    //QPixmap* pixmap = new QPixmap( 200, 200 );
    //pixmap->fill( Qt::white );
    //QPainter painter( pixmap );
    //painter->setRenderHint( QPainter::Antialiasing );

    shape = Polygon;
    antialiased = false;
    transformed = false;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(false);


    this->startTimer(16,Qt::PreciseTimer);

    for(int j=0; j<chM;  j++)
    for(int i=0; i<rail; i++)
    {
        buf[j][i] = 0;
    }
    pnt = 0;
    time = 0;

//    fr = false;
//    rcnt = 0;

//    timer   = new QTimer();
//    timer->start(100);
//    connect(timer,SIGNAL(timeout()), this,SLOT(interrupt()) );
}

void QPainterWidget::interrupt(int32_t* data)
{
    //qDebug() << rcnt;
//    rcnt = 0;

    //double  z = sin( double(time)/100.0 * 2.0 * M_PI ) * 100.0;
    //int     r = int(z);

//    qDebug() << data[1] << data[3];

    buf[0][pnt] = data[0];
    buf[1][pnt] = data[1];
//    buf[2][pnt] = data[2];
//    buf[3][pnt] = data[3];
    pnt++; pnt %= rail;

    time++;

}

void QPainterWidget::timerEvent(QTimerEvent*)
{
//    fr = true;

//    rcnt++;
    update();
}


QSize QPainterWidget::sizeHint() const
{
    return QSize(256, 192);
}

QSize QPainterWidget::minimumSizeHint() const
{
    return QSize(73, 73);
}

void QPainterWidget::setShape(Shape shape)
{
    this->shape = shape;
//    update();
}

void QPainterWidget::setPen(const QPen &pen)
{
    this->pen = pen;
//    update();
}

void QPainterWidget::setBrush(const QBrush &brush)
{
    this->brush = brush;
//    update();
}

void QPainterWidget::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
//    update();
}

void QPainterWidget::setTransformed(bool transformed)
{
    this->transformed = transformed;
//    update();
}

void QPainterWidget::resizeEvent(QResizeEvent*)
{
    qDebug() << width() << height();
}

bool QPainterWidget::event(QEvent *ev)
{
    if (ev->type() == QEvent::UpdateRequest)
    {
//        rcnt++;
        return true;
    }
    return QWidget::event(ev);

}

//void QPainterWidget::animate() // !! этот метод не работает !!!
//{
//     rcnt++;
//     update();
//}

void QPainterWidget::paintEvent(QPaintEvent* )
{
    //if(fr)
    {
    painter.begin(this);

// painter.setViewport( 50,50,200,200);

    painter.setRenderHint(QPainter::Antialiasing, false);

    int sy = height() / (chM+1);
    int dx = width() - 1;

    for(int gt = 0; gt<chM; gt++)
    {
        int pz = pnt - dx;
        if (pz<0) pz += rail;

        int y;
        int lx = 0;
        int ly = buf[gt][pz];
        pz++; pz%=rail;

        for(int x=1; x < dx; x++)
        {
            y = buf[gt][pz];
            int ty = sy*(gt+1);

            if(gt%2==0) { painter.setPen(QPen(Qt::red, 1,Qt::SolidLine)); }
            else        { painter.setPen(QPen(Qt::blue,1,Qt::SolidLine)); }

            painter.drawLine(lx,ly + ty ,x,y + ty);
            ly = y;
            lx = x;
            pz++; pz%=rail;
        }
    }

    QRect rect(10, 20, 20, 20);
    painter.setPen(QPen(Qt::green, 1,Qt::SolidLine));
//    painter.drawEllipse(rect);

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::gray, 1,Qt::SolidLine));
    for(int y=10; y < height(); y+=20)
    {
        painter.drawLine(0,y, width()-1,y);
    }

    painter.end();
    }

    //fr = false;
    //update();
}



