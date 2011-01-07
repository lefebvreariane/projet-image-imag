#ifndef LABELIMAGE_H
#define LABELIMAGE_H

#include <QWidget>
#include <QLabel>


class LabelImage : public QLabel
{
    Q_OBJECT
public:
    LabelImage();
    void mousePressEvent(QMouseEvent *event);
    int X0,Y0,X1,Y1;


signals:
    void clic();
public slots:

private:


};

#endif // LABELIMAGE_H
