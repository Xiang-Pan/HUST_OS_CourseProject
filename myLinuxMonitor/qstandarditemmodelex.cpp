#include "qstandarditemmodelex.h"
void QStandardItemModelEx::ergodicAllItem(QStandardItemModel* model,callback_ergodicFun_ptr pFun)
{
    int rows = model->rowCount();
    int column = model->columnCount();
    for (int i=0;i<rows;++i)
    {
        for(int j=0;j<column;++j)
        {
            QStandardItem* item = model->item(i,j);
            if (item)
            {
                if(!ergodicItem(item,pFun))
                    return;
            }
        }
    }
}

bool QStandardItemModelEx::ergodicItem(QStandardItem* item,callback_ergodicFun_ptr pFun)
{
    int rows = item->rowCount();
    int column = item->columnCount();
    for (int i=0;i<rows;++i)
    {
        for(int j=0;j<column;++j)
        {
            QStandardItem* childItem = item->child(i,j);
            if (childItem)
            {
                if(!ergodicItem(childItem,pFun))
                    return false;
            }
        }
    }
    return pFun(item);
}
