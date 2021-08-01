#include "TDelegate.h"
#include "qcheckbox.h"

TDelegate::TDelegate(QObject* parent) :
    QStyledItemDelegate(parent) {}

QWidget* TDelegate::createEditor(QWidget* parent,
    const QStyleOptionViewItem&/* option */,
    const QModelIndex&/* index */) const
{
    QCheckBox* checkbox = new QCheckBox ("MyText", parent);
    //QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
    //Ensures large values can be input, which GearSim definitely uses.
    //editor->setMaximum(10000000000);
    //editor->setDecimals(10);
    return checkbox;
}


void TDelegate::setEditorData(QWidget* editor,
    const QModelIndex& index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QCheckBox* checkbox = static_cast<QCheckBox*>(editor);
    //spinBox->setValue(value);
    checkbox->setText(value);
    //QTextEdit* edit = static_cast<QTextEdit*>(editor);
    //edit->setText("Dummy");
}

void TDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
    const QModelIndex& index) const
{
    //QCheckBox* checkBox = static_cast<QCheckBox*>(editor);
    //double value = spinBox->value();
    //QVariant value = 0;
    //model->setData(index, value, Qt::EditRole);
}

void TDelegate::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option,
    const QModelIndex&/* index */) const
{
    editor->setGeometry(option.rect);
}