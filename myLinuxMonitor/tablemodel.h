/* FileName:    tablemodel.h
 * Author:      Hover
 * E-Mail:      hover@hust.edu.cn
 * GitHub:      HoverWings
 * Description: inherit the QItemDelegate and overload partial fun of the module
 *              to draw the process bar in the QTableView
 *              setData and maintain the data in this class which is convient for
 *              update the data and draw processbar dynamically
 */
#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setHorizontalHeader(const QStringList& headers);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setData(const QVector<QStringList>& data);
    QVector<QStringList>& DataVector() {return m_data;}
    ~TableModel(void);

signals:

public slots:

private:
    QStringList m_HorizontalHeader;
    QVector<QStringList> m_data;

};

#endif // TABLEMODEL_H
