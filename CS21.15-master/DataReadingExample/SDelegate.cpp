#include "SDelegate.h"
#include <QDoubleSpinBox>
#include <qtextedit.h>
#include <qtreeview.h>
#include "TreeItem.h"
#include <qapplication.h>

SDelegate::SDelegate(QObject* parent) :
    QStyledItemDelegate(parent) {}

QWidget* SDelegate::createEditor(QWidget* parent,
    const QStyleOptionViewItem&/* option */,
    const QModelIndex&/* index */) const
{
    QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
    //Ensures large values can be input, which GearSim definitely uses.
    editor->setMaximum(10000000000);
    editor->setDecimals(6);
    return editor;
}


void SDelegate::setEditorData(QWidget* editor,
    const QModelIndex& index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox* spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
    //QTextEdit* edit = static_cast<QTextEdit*>(editor);
    //edit->setText("Dummy");
}

void SDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
    const QModelIndex& index) const
{
    QDoubleSpinBox* spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();
    //QVariant value = 0;
    model->setData(index, value, Qt::EditRole);
}

void SDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}