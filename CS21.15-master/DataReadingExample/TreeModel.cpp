#include "TreeModel.h"

TreeModel::TreeModel(const QList<GearSimData*> lines, QObject* parent)
    : QAbstractItemModel(parent)
{
    //rootItem = new TreeItem({ tr("Title"), tr("Summary"), tr("Test") });
    rootItem = new TreeItem("Header");
    setupModelData(lines, rootItem);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem* parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem* childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        TreeItem* item = static_cast<TreeItem*>(parent.internalPointer());
        return item->columnCount();
    }
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return (Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    
    return QVariant();
}

void TreeModel::setupModelData(const QList<GearSimData*> lines, TreeItem* parent) {
    //int arbitrary = lines.size() / 5; //This would divide the view into sections of 5 items each.
    int arbitrary = lines.size(); //This would divide the view into 1 section of all items.
    int counter = arbitrary;
    TreeItem* currentParent = parent;
    for (GearSimData* datum : lines) {
        //Create the sections
        while (counter >= arbitrary) {
            //TODO: make sections have some sort of autodetection or manual lookup
            TreeItem* node = new TreeItem("Section", parent);
            parent->appendChild(node);
            currentParent = node;
            counter = 0;
        }

        //Create the items under this current section
        TreeItem* node = new TreeItem(datum, currentParent);
        currentParent->appendChild(node);
        counter++;
    }
}

bool TreeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
    item->setData(index.column(), value);

    emit dataChanged(index, index);
    return true;
}