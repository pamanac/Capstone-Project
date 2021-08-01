#pragma once
#include <QApplication>
#include <QSplitter>
#include <QFileSystemModel>
#include <QDir>
#include <QTreeView>
#include <QListView>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include "..\QtGearSim\ExampleBrowser.h"

int ExamplesNamespace::ExampleModelViewBasic(int argc, char *argv[]) {
    //QApplication app(argc, argv);
    QSplitter* splitter = new QSplitter(); // is a QFrame, QWidget. Allows widgets to be added to a bounding box,
                                           // and they can be resized relative to one another within the
                                           // bounding box

    QFileSystemModel* fs_model = new QFileSystemModel();                                           // model interface to file system
    QObject::connect(fs_model, &QFileSystemModel::directoryLoaded, [=] (QString const& directory) {
                                                                                                   // fs model load is asynch
        auto root = fs_model->index(directory);
        auto num_rows = fs_model->rowCount(root);                                                  // can be used to iterate the tree
        for (int i = 0; i < num_rows; i++) {
            auto index = fs_model->index(i, 0, root);
            qDebug() << fs_model->data(index, Qt::DisplayRole).toString() << "\n";
        }
    });
    fs_model->setRootPath(QDir::currentPath());          // project root directory

    QTreeView* tree_view = new QTreeView(/*parent*/ splitter);     // view owned by splitter frame/widget
    tree_view->setModel(fs_model);                                 // view the file system model as a tree
    tree_view->setRootIndex(fs_model->index(QDir::currentPath())); // specific way to set the root index when using file system model

    QListView* list_view = new QListView(/*parent*/ splitter);     // another view owned by splitter
    list_view->setModel(fs_model);                                 // shares model with the tree view
    list_view->setRootIndex(fs_model->index(QDir::currentPath())); // every view needs to initialize itself
                                                                   // views that share a model should be able to reuse init code
    splitter->setWindowTitle("two views one model");
    splitter->show();

    //return app.exec();
    return 0;
}
