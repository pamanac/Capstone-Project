#ifndef TAB_PROXY_MODEL_H
#define TAB_PROXY_MODEL_H

#include <QSortFilterProxyModel>
#include <QWidget>

#include "QJsonModel.h"

class TabProxyModel : public QSortFilterProxyModel {
public:
    QString primaryText;
    QString secondaryText;

    TabProxyModel() = delete;
    TabProxyModel(QString primaryText, QString secondaryText, QWidget* parent = nullptr);

protected:
    bool filterAcceptsRow(int row, QModelIndex const& parent) const override;
};

#endif