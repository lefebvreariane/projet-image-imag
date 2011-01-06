#ifndef LABELIMAGE_H
#define LABELIMAGE_H

#include <QWidget>
#include <QLabel>


class LabelImage : public QLabel
{
    Q_OBJECT
public:
    void mousePressEvent(QMouseEvent *event);
    int get_clic_X();
    int get_clic_Y();


signals:

public slots:

private:


};

#endif // LABELIMAGE_H
