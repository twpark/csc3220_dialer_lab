#include "myaddressbookmodel.h"

#include <QFile>
#include <QMessageBox>
#include <iostream>
#include <QTextStream>

MyAddressBookModel::MyAddressBookModel(QObject *parent)
    :QAbstractTableModel (parent)
{

}

int MyAddressBookModel::rowCount(const QModelIndex &parent) const
{
    return filteredIndex.size();
}

int MyAddressBookModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant MyAddressBookModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case 0: // firstname
            return firstNames.at(filteredIndex[index.row()]);
        case 1:
            return lastNames.at(filteredIndex[index.row()]);
        case 2:
            return phoneNumbers.at(filteredIndex[index.row()]);
        }
//        return QString("Row%1, Column%2")
//                .arg(index.row())
//                .arg(index.column());
    }

    return QVariant();
}

void MyAddressBookModel::openFile(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    QTextStream in(&file);

    firstNames.clear();
    lastNames.clear();
    phoneNumbers.clear();

    for(int i = 0; !in.atEnd(); i++) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (i == 0) continue;

        for(int j = 0; j < fields.length(); j++) {
            std::cout << "[" << j << "]" << fields[j].toStdString();
        }
        std::cout << std::endl;

        firstNames.push_back(fields[0]);
        lastNames.push_back(fields[1]);
        phoneNumbers.push_back(fields[7]);

        filteredIndex.push_back(i);
    }

    file.close();

    emit layoutChanged();
}

QString MyAddressBookModel::getPhoneNumber(int index)
{
    return phoneNumbers.at(filteredIndex[index]);
}

void MyAddressBookModel::setFilterString(QString fStr)
{
    // clear filtered index and then I will rebuild the index.
    filteredIndex.clear();

    // check if phone numbers are starting with fStr.
    for (int i = 0; i < phoneNumbers.size(); i++) {
        if (phoneNumbers[i].startsWith(fStr)) {
            filteredIndex.push_back(i);
            //std::cout << phoneNumbers[i].toStdString() << std::endl;
        }
    }

    emit layoutChanged();
}









