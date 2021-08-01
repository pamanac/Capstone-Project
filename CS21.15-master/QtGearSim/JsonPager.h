#ifndef JSON_PAGER_H
#define JSON_PAGER_H

#include <QLabel>
#include <QList>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <tuple>
#include <QPair>
#include <QTreeView>
#include <QTableView>
#include<QAbstractItemModel>

#include "QJsonModel.h"

class JsonPager : public QWidget {
private:
    class DisplayWidget : public QWidget {
    public:
        QLabel* label;
        QTreeView* view;
        QVBoxLayout* vLayout;

        DisplayWidget(QLabel* label, QTreeView* view, QWidget* parent = nullptr) : QWidget(parent) {
            this->label = label;
            this->view = view;
            vLayout = new QVBoxLayout();
            vLayout->addWidget(label, 0, Qt::AlignCenter);
            vLayout->addWidget(view, 1);
            setLayout(vLayout);
        }
    };

    class DisplayTableWidget : public QWidget {
    public:
        QLabel* propertiesLabel;
        QTreeView* propertiesView;
        QLabel* label;
        QTableView* view;
        QVBoxLayout* vLayout;

        DisplayTableWidget(QLabel* label, QLabel* propertiesLabel, QTableView* view, QTreeView* propertiesView, QWidget* parent = nullptr) : QWidget(parent) {
            this->label = label;
            this->view = view;
            this->propertiesLabel = propertiesLabel;
            this->propertiesView = propertiesView;
            vLayout = new QVBoxLayout();
            vLayout->addWidget(label, 0, Qt::AlignCenter);
            vLayout->addWidget(view, 4);
            if (propertiesLabel != nullptr) {
                vLayout->addWidget(propertiesLabel, 0, Qt::AlignCenter);
            }
            if (propertiesView != nullptr) {
                vLayout->addWidget(propertiesView, 1);
            }
            setLayout(vLayout);
        }
    };

    QStackedWidget* stack;
    //QList<QJsonModel*> models;
    QList<QAbstractItemModel*> models;

    QHBoxLayout* hLayout;
    QVBoxLayout* vLayout;

    QPushButton* back;
    QPushButton* next;

    QJsonObject getAndRemoveArrays(QJsonObject, QList<std::tuple<QString, QJsonValue>>&);

public:
    JsonPager(QWidget*);
    JsonPager(QJsonObject, QWidget* parent = nullptr);
};

#endif JSON_PAGER_H