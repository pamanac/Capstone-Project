#include "TreeItem.h"

TreeItem::TreeItem(GearSimData* data, TreeItem* parent)
    : m_itemData(data), m_parentItem(parent), sectionName("")
{}

TreeItem::TreeItem(const QString sectionName, TreeItem* parentItem)
    : m_itemData(nullptr), m_parentItem(parentItem), sectionName(sectionName)
{}


TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem* item)
{
    m_childItems.append(item);
}

TreeItem* TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::columnCount() const {
    //if (m_parentItem)
        return 4;
    //return 1; //section name
}

bool TreeItem::isLeaf() const {
    return m_childItems.count() == 0;
}

TreeItem* TreeItem::parentItem()
{
    return m_parentItem;
}

QVariant TreeItem::data(int column) const {
    if (this->isLeaf() && m_itemData != nullptr) {
        switch (column) {
        case 0: //Full name
            return m_itemData->getFullName();
        case 1: //short name
            return m_itemData->shortName;
        case 2:
            return m_itemData->value;
        default:
            return m_itemData->units;
        }
    }
    if (m_parentItem == nullptr) {
        switch (column) {
        case 0: //Full name
            return "Full Name";
        case 1: //short name
            return "Short Name";
        case 2:
            return "Value";
        default:
            return "Units";
        }
    }
    if(column == 0)
        return "Section";
    return "";
}


void TreeItem::setData(int column, QVariant value) {
    switch (column) {
    case 0: //Full name
        m_itemData->fullName = value.toString();
    case 1: //short name
        m_itemData->shortName = value.toString();
    case 3:
        m_itemData->units = value.toString();
    default:
        m_itemData->value = value.toFloat();
    }
}

