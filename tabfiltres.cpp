#include "tabfiltres.h"

#include <QtGui>


TabFiltres::TabFiltres(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *TabFiltres::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setMinimum(-99);
    editor->setMaximum(99);
    editor->setValue(0);
    editor->setMaximumSize(40,40);
    editor->setMinimumSize(40,40);
    editor->setAlignment(Qt::AlignCenter);
    editor->setButtonSymbols(QAbstractSpinBox::NoButtons);


    return editor;
}
