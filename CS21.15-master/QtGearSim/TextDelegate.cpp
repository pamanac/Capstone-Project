#include "TextDelegate.h"
#include <QDoubleSpinBox>
#include <qtextedit.h>
#include <qtreeview.h>
#include <qapplication.h>
#include <QLineEdit>
#include <QCheckBox>
#include "ExampleBrowser.h"

TextDelegate::TextDelegate(QObject* parent) :
    QStyledItemDelegate(parent) {}

QWidget* TextDelegate::createEditor(QWidget* parent,
    const QStyleOptionViewItem&/* option */,
    const QModelIndex&/* index */) const
{
    QCheckBox* editor = new QCheckBox(parent);
    //editor->setMaximumSize();
    editor->setObjectName("Editor");
    editor->setStyleSheet(ExampleBrowser::returnStyleSheet(":/data/Resources/Delegate.qss"));
    //editor->setMaxLength(2000);
    //editor->setReadOnly(true);
    //Ensures large values can be input, which GearSim definitely uses.
    return editor;
}


void TextDelegate::setEditorData(QWidget* editor,
    const QModelIndex& index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    int counter = 0;
    for (counter = 0; counter < value.size(); counter++)
        if (value.at(counter) == '[')
            break;
    value.truncate(counter-1);
    
    QCheckBox* checkBox = static_cast<QCheckBox*>(editor);
    checkBox->setText(value);
}

void TextDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
    const QModelIndex& index) const
{
    //QDoubleSpinBox* spinBox = static_cast<QDoubleSpinBox*>(editor);
    //spinBox->interpretText();
    //double value = spinBox->value();
    //QVariant value = 0;
   // model->setData(index, value, Qt::EditRole);
}

void TextDelegate::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option,
    const QModelIndex&/* index */) const
{
    editor->setGeometry(option.rect);
}