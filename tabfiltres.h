#ifndef TABFILTRES_H
#define TABFILTRES_H
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>

class TabFiltres : public QItemDelegate
{
    Q_OBJECT

public:
    TabFiltres(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
};

#endif
