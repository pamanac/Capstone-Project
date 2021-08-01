/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2011 SCHUTZ Sacha
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

 /*
  * The MIT License (MIT)
  *
  * Copyright (c) 2021 Oleg Kovalenko
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */

#include "QJsonModel.h"
#include <QFile>
#include <QDebug>


QJsonTreeItem::QJsonTreeItem(QJsonTreeItem* parent) : mParent{ parent }, mChilds{}, mKey{}, mValue{}, mType{} {}

QJsonTreeItem::~QJsonTreeItem() {
    qDeleteAll(mChilds);
}

void QJsonTreeItem::appendChild(QJsonTreeItem *item) {
    mChilds.append(item);
}

QJsonTreeItem *QJsonTreeItem::child(int row) {
    return mChilds.value(row);
}

QJsonTreeItem *QJsonTreeItem::parent() {
    return mParent;
}

int QJsonTreeItem::childCount() const {
    return mChilds.count();
}

int QJsonTreeItem::row() const {
    return mParent ? mParent->mChilds.indexOf(const_cast<QJsonTreeItem*>(this)) : 0;
}

void QJsonTreeItem::setKey(const QString &key) {
    mKey = key;
}

void QJsonTreeItem::setValue(const QString &value) {
    mValue = value;
}

void QJsonTreeItem::setType(const QJsonValue::Type &type) {
    mType = type;
}

QString QJsonTreeItem::key() const {
    return mKey;
}

QString QJsonTreeItem::value() const {
    return mValue;
}

QJsonValue::Type QJsonTreeItem::type() const {
    return mType;
}

QJsonTreeItem* QJsonModel::item(const QModelIndex& index) const {
    if (index.isValid()) {
        QJsonTreeItem* item = static_cast<QJsonTreeItem *>(index.internalPointer());
        if (item) {
            return item;
        }
    }
    return mRootItem;
}

QJsonModel::QJsonModel(QObject *parent) : QAbstractItemModel(parent), mRootItem{nullptr} {
    mHeaders.append("key");
    mHeaders.append("value");
}

QJsonModel::QJsonModel(const QString& fileName, QObject *parent) : QAbstractItemModel(parent), mRootItem{nullptr} {
    mHeaders.append("key");
    mHeaders.append("value");
    load(fileName);
}

QJsonModel::QJsonModel(QIODevice * device, QObject *parent) : QAbstractItemModel(parent), mRootItem{nullptr} {
    mHeaders.append("key");
    mHeaders.append("value");
    load(device);
}

QJsonModel::QJsonModel(const QByteArray& json, QObject *parent) : QAbstractItemModel(parent), mRootItem{nullptr} {
    mHeaders.append("key");
    mHeaders.append("value");
    loadJson(json);
}

QJsonModel::~QJsonModel() {
    delete mRootItem;
}

bool QJsonModel::load(const QString &fileName) {
    QFile file(fileName);
    bool success = false;
    if (file.open(QIODevice::ReadOnly)) {
        success = load(&file);
        file.close();
    }

    return success;
}

bool QJsonModel::load(QIODevice *device) {
    return loadJson(device->readAll());
}

bool QJsonModel::loadJson(const QByteArray &json) {
    const auto& jdoc = QJsonDocument::fromJson(json);
    return loadJson(jdoc);
}

bool QJsonModel::loadJson(const QJsonDocument& jdoc) {
    if (!jdoc.isNull()) {
        beginResetModel();
        if (mRootItem) {
            delete mRootItem;
        }
        if (jdoc.isObject()) {
            loadItem(jdoc.object(), "root", nullptr);
        }
        else if (jdoc.isArray()) {
            loadItem(jdoc.array(), "root", nullptr);
        }
        endResetModel();
        return true;
    }
    qDebug() << Q_FUNC_INFO << "cannot load json";
    return false;
}

// TODO: loadRoot vs loadItem; loadRoot has to work like this
//       but loadItem can call beginInsertRows for all keys of a value at once
// TODO: use QModelIndex instead of QJsonTreeItem* as parent argument type
void QJsonModel::loadItem(const QJsonValue& value, QString key, QJsonTreeItem* parent) {
    // get the index of the item if we can
    QModelIndex parentIdx = parent ? createIndex(0, 0, parent) : QModelIndex();

    // refer to beginInsertRows documentation on qt, the API is un-intuitive
    // in this case, to append a row to the end of the parentIdx,
    // pass the rowCount twice (integer index of first new entry and
    // last new entry, in this case they are the same, since there is one
    // entry)
    int first = rowCount(parentIdx);
    int last = first;
    beginInsertRows(parentIdx, first, last);

    // item will be a child of parent, need to finish filling it out first
    QJsonTreeItem* item = new QJsonTreeItem(parent);
    
    // set the key and type
    item->setKey(key);
    item->setType(value.type());

    // if the type is not nested, then set the value as a string
    bool isNested = value.isObject() || value.isArray();
    if (isNested == false) {
        item->setValue(value.toVariant().toString());
    }

    // do the row insertion
    if (parent == nullptr) {
        mRootItem = item;
    }
    else {
        parent->appendChild(item);
    }
    endInsertRows();

    // this seems like it should work, but it does not
    // TODO: is it possible to signal columns and rows
    //       for the same indices?
    // QModelIndex idx = createIndex(0, 0, item);
    // beginInsertColumns(idx, 0, 1);
    // endInsertColumns();

    // fill out the model with all the other keys, recursively
    if (value.isObject())
    {
        for (QString key : value.toObject().keys())
        {
            QJsonValue subValue = value.toObject().value(key);
            loadItem(subValue, key, item);
        }
    }
    else if (value.isArray())
    {
        int i = 0;
        for (QJsonValue subValue : value.toArray())
        {
            QString key = QString::number(i);
            loadItem(subValue, key, item);
            i++;
        }
    }
}

QVariant QJsonModel::data(const QModelIndex &idx, int role) const {
    if (item(idx) != mRootItem && !idx.isValid()) {
        return QVariant();
    }

    QJsonTreeItem* item = this->item(idx);
    /*
    if (role == Qt::CheckStateRole) // this shows the checkbox
    {
        bool aBool = item->toBool();

        if (aBool)
            return Qt::Checked;
        else
            return Qt::Unchecked;
    }*/

    if (role == Qt::DisplayRole) {
        if (idx.column() == 0) {
            return QString("%1").arg(item->key());
        }
        if (idx.column() == 1) {
            return QString("%1").arg(item->value());
        }
    }
    else if (Qt::EditRole == role) {
        if (idx.column() == 1) {
            return QString("%1").arg(item->value());
        }
    }
    return QVariant();

}

bool QJsonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int col = index.column();
    if (Qt::EditRole == role) {
        if (col == 1) {
            QJsonTreeItem *item = static_cast<QJsonTreeItem*>(index.internalPointer());
            item->setValue(value.toString());
            emit dataChanged(index, index, {Qt::EditRole});
            return true;
        }
    }
    return false;
}



QVariant QJsonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }   
    if (orientation == Qt::Horizontal) {
        return mHeaders.value(section);
    }
    return QVariant();
}

QModelIndex QJsonModel::index(int row, int column, const QModelIndex &parent) const {

    bool rowIsValid = row < rowCount(parent);
    bool columnIsValid = column == 0 || column == 1;
    if (parent.isValid() && !rowIsValid || !columnIsValid) {
        return QModelIndex();
    }

    QJsonTreeItem *parentItem;

    if (!parent.isValid()) {
        parentItem = mRootItem;
    }
    else {
        parentItem = static_cast<QJsonTreeItem*>(parent.internalPointer());
    }

    QJsonTreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }
    else {
        return QModelIndex();
    }
}

QModelIndex QJsonModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }
    QJsonTreeItem *item = static_cast<QJsonTreeItem*>(index.internalPointer());
    if (item == mRootItem) {
        return QModelIndex();
    }
    QJsonTreeItem* parentItem = item->parent();
    if (parentItem == nullptr) {
        return QModelIndex();
    }
    return createIndex(parentItem->row(), 0, parentItem);
}

int QJsonModel::rowCount(const QModelIndex &idx) const {
    QJsonTreeItem* item = this->item(idx);
    return item ? item->childCount() : 0;
}

int QJsonModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 2;
}

Qt::ItemFlags QJsonModel::flags(const QModelIndex &idx) const {
    int column = idx.column();
    QJsonTreeItem* item = this->item(idx);

    bool isArray = QJsonValue::Array == item->type();
    bool isObject = QJsonValue::Object == item->type();

    if ((column == 1) && !(isArray || isObject)) {
        return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable | QAbstractItemModel::flags(idx);
    }
    else {
        //return QAbstractItemModel::flags(index);
        Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(idx);
        if (idx.isValid()) {
            return defaultFlags | Qt::ItemIsUserCheckable;
        }
        return defaultFlags;
    }
}

QJsonDocument QJsonModel::json() const {
    QJsonValue value = genJson(mRootItem);
    QJsonDocument jdoc;

    if (value.isObject()) {
        jdoc = QJsonDocument(value.toObject());
    }
    else {
        jdoc = QJsonDocument(value.toArray());
    }
    return jdoc;
}

QJsonValue QJsonModel::genJson(QJsonTreeItem* item) const {
    int numChildren = item->childCount();

    if (item->type() == QJsonValue::Object) {
        QJsonObject obj;
        for (int i = 0; i < numChildren; i++) {
            QJsonTreeItem* child = item->child(i);
            QString key = child->key();
            obj.insert(key, genJson(child));
        }
        return  obj;
    }
    else if (item->type() == QJsonValue::Array) {
        QJsonArray arr;
        for (int i = 0; i < numChildren; i++) {
            QJsonTreeItem* child = item->child(i);
            arr.append(genJson(child));
        }
        return arr;
    }
    else {
        QJsonValue value = QJsonValue(item->value());
        return value;
    }
}