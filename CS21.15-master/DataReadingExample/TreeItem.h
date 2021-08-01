#pragma once
#include <qvector.h>
#include <qvariant.h>
#include "GearSimData.h"

class TreeItem
{
public:
    explicit TreeItem(GearSimData* data, TreeItem* parentItem = nullptr);
    explicit TreeItem(const QString sectionName, TreeItem* parentItem = nullptr);
    ~TreeItem();

    void appendChild(TreeItem* child);

    TreeItem* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem* parentItem();
    bool isLeaf() const;

    void setData(int column, QVariant value);

private:
    QVector<TreeItem*> m_childItems;
    QString sectionName;
    GearSimData* m_itemData;
    TreeItem* m_parentItem;
};