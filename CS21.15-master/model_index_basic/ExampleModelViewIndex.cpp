#include <QApplication>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QString>
#include <QDebug>
#include "..\QtGearSim\ExampleBrowser.h"

int ExamplesNamespace::ExampleModelViewIndex(int argc, char *argv[])
{
    //QApplication app(argc, argv);

    // o
    // o
    // o
    // o
    auto list_model = new QStandardItemModel(4, 1);
    for (int r = 0; r < list_model->rowCount(); r++) {
        auto item = new QStandardItem(QString("%0").arg(r));
        list_model->setItem(r, 0, item);
    }

    // o o o o
    // o o o o
    // o o o o
    // o o o o
    auto table_model = new QStandardItemModel(4, 4);
    for (int r = 0; r < table_model->rowCount(); r++) {
        for (int c = 0; c < table_model->columnCount(); c++) {
            auto item = new QStandardItem(QString("%0, %1").arg(r).arg(c));
            table_model->setItem(r, c, item);
        }
    }

    // o
    // + o
    //   + o
    //     + o
    auto tree_model = new QStandardItemModel();
    auto from = tree_model->invisibleRootItem();
    for (int i = 0; i < 4; i++) {
        from->appendRow(new QStandardItem(QString("%0").arg(i)));
        from = tree_model->itemFromIndex(tree_model->sibling(0, 0, tree_model->indexFromItem(from)));
        // as far as i can tell, this is the general form of tree model construction,
        // to get from an item to an index back to an item,
        // because the entire model interface is based on indexes,
        // but construction requires items.
        // helper functions will make this better
    }

    auto list_index_root = list_model->index(0, 0);              // the root index
    auto list_index1 = list_model->index(2, 0);                  // use the root index as parent by default
    auto list_index2 = list_model->index(2, 0, QModelIndex());   // explicitly use the root index
                                                                 // (list_index1 == list_index2) is true

    auto list_index3 = list_model->index(1, 0, list_index_root); // explicitly use the root index by variable. does it work? NO.

    auto table_index1 = table_model->index(2, 2);
    auto table_index2 = table_model->index(1, 1, table_index1);   // offset (1,1) relative to table_index1. does it work? NO.

    auto tree_index1 = tree_model->index(0, 0, QModelIndex());
    auto tree_index2 = tree_model->index(0, 0, tree_index1);      // relative offsets are necessary for tree model. must work! YES.

    qDebug() << "Expected 0: " << list_model->data(list_index_root, Qt::DisplayRole).toString() << "\n"
        << "Expected 2: " << list_model->data(list_index1, Qt::DisplayRole).toString() << "\n"
        << "Expected 2: " << list_model->data(list_index2, Qt::DisplayRole).toString() << "\n"
        << "Expected 1: " << list_model->data(list_index3, Qt::DisplayRole).toString() << "\n"
        << "Expected 2, 2: " << table_model->data(table_index1, Qt::DisplayRole).toString() << "\n"
        << "Expected 3, 3: " << table_model->data(table_index2, Qt::DisplayRole).toString() << "\n"
        << "Expected 0: " << tree_model->data(tree_index1, Qt::DisplayRole).toString() << "\n"
        << "Expected 1: " << tree_model->data(tree_index2, Qt::DisplayRole).toString();

    //return app.exec();
    return 0;

    delete list_model;
    delete table_model;
    delete tree_model;
}