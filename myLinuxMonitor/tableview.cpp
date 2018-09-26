/* FileName:    tableview.cpp
 * Author:      Hover
 * E-Mail:      hover@hust.edu.cn
 * GitHub:      HoverWings
 * Description: tableview overload
 */
#include "tableview.h"

#include "tablemodel.h"
#include "progressbardelegate.h"

TableView::TableView(QWidget *parent) :
    QTableView(parent)
{
    iniData();
}

TableView::~TableView()
{
    delete m_model;
}

void TableView::iniData()
{
    m_model = new TableModel();
    this->setModel(m_model);
    m_progressBarDelegate = new ProgressBarDelegate(this);
    this->setItemDelegate(m_progressBarDelegate);
}
