#include "launchtableview.h"
#include <iostream>

LaunchTableView::LaunchTableView(QWidget *parent) : QTableView(parent)
{
    model_ = new QStandardItemModel(this);
    select_model_ = new QItemSelectionModel(model_);

    this->setModel(model_);
    this->setSelectionModel(select_model_);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->setItemDelegate(new FileDelegate(this, this));

    connect(model_, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));
}

QString LaunchTableView::getIncompleteRowsInQString(QString seperator) {
    QString ret;
    for(auto it = incomplete_rows.begin(); it != incomplete_rows.end(); it++) {
        if(it != incomplete_rows.begin())
            ret += seperator;
        ret += QString::number(*it + 1);
    }
    return ret;
}

bool LaunchTableView::lookupIncompleteRow(int row, std::list<int>::iterator &it) {
    it = incomplete_rows.begin();

    for(; it != incomplete_rows.end(); it++) {
        if((*it) >= row) {
            break;
        }
    }

    if(it == incomplete_rows.end() || (*it) != row)
        return false;
    else
        return true;
}

void LaunchTableView::clearIncompleteRows() {
    while (incomplete_rows.size()) {
        deleteRow(*(incomplete_rows.begin()));
    }
}

void LaunchTableView::addIncompleteRow(int row, bool row_reduce) {
    std::list<int>::iterator it;
    lookupIncompleteRow(row, it);
    addIncompleteRow(row, row_reduce, it);
}

void LaunchTableView::addIncompleteRow(int row, bool row_reduce, std::list<int>::iterator &it) {
    std::list<int>::iterator it_new = incomplete_rows.insert(it, row);

    if(row_reduce) {
        it_new++;
        for(; it_new != incomplete_rows.end(); it_new++) {
            (*it_new) = (*it_new) + 1;
        }
    }
}

void LaunchTableView::deleteIncompleteRow(int row, bool row_reduce) {
    std::list<int>::iterator it;
    lookupIncompleteRow(row, it);
    deleteIncompleteRow(row, row_reduce, it);
}

void LaunchTableView::deleteIncompleteRow(int row, bool row_reduce, std::list<int>::iterator &it) {
    it = incomplete_rows.erase(it);

    if(row_reduce) {
        for(; it != incomplete_rows.end(); it++) {
            (*it) = (*it) - 1;
        }
    }
}

void LaunchTableView::addRow(int row, QList<QStandardItem*> &items) {
    if(row > model_->rowCount())
        row = model_->rowCount();

    model_->insertRow(row, items);

    if(items.size() && !items.front()->text().isEmpty()) {
        key_set.insert(items.front()->text().toStdString());
    }

    if(isRowIncomplete(row)) {
        addIncompleteRow(row, true);
    }
}

void LaunchTableView::deleteRow(int row) {
    if(row < model_->rowCount()) {
        if(isRowIncomplete(row)) {
            deleteIncompleteRow(row, true);
        }

        QString key = model_->data(model_->index(row, 0)).toString();
        key_set.erase(key.toStdString());

        model_->removeRow(row);
    }
}

void LaunchTableView::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
    for(int row = topLeft.row(); row <= bottomRight.row(); row++) {
        if(isRowIncomplete(row)) {
            std::list<int>::iterator it;
            if(!lookupIncompleteRow(row, it)) {
                addIncompleteRow(row, false, it);
            }
        }
        else {
            std::list<int>::iterator it;
            if(lookupIncompleteRow(row, it)) {
                deleteIncompleteRow(row, false, it);
            }
        }
    }
}

bool LaunchTableView::isKeyReapted(const QString &key) {
    return key_set.count(key.toStdString()) > 0;
}

bool LaunchTableView::deleteKey(const QString &key) {
    key_set.erase(key.toStdString());
    return true;
}

bool LaunchTableView::addKey(const QString &key) {
    key_set.insert(key.toStdString());
    return true;
}

QStringList LaunchTableView::getAllKeys() {
    QStringList ret;
    for(int i = 0; i < model_->rowCount(); i++) {
        ret << model_->data(model_->index(i, 0)).toString();
    }
    return ret;
}

void LaunchTableView::notifyUpdate() {
    emit launchTableUpdate();
}

bool LaunchTableView::existIncompleteRows() {
    return incomplete_rows.size() > 0;
}

bool LaunchTableView::isRowIncomplete(int row) {
    if(row >= model_->rowCount())
        return true;

    for(int col = 0; col < model_->columnCount(); col++) {
        if(model_->data(model_->index(row, col)).toString().size() == 0)
            return true;
    }

    return false;
}
