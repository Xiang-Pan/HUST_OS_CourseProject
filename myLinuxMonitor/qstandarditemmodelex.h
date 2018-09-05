#ifndef QSTANDARDITEMMODELEX_H
#define QSTANDARDITEMMODELEX_H

#include <QStandardItemModel>
#include <functional>
class QStandardItemModelEx
{
public:
     QStandardItemModelEx(){}
     ~QStandardItemModelEx(){}
     ///
     /// \brief callback_ergodicFun_ptr 回调函数指针，bool f(QStandardItem*),bool用于决定是否继续，如果为true就继续递归，如果为false就停止递归
     ///
     typedef std::function<bool(QStandardItem*)> callback_ergodicFun_ptr;

     //typedef void(*callback_ergodicFun_ptr)(QStandardItem*);
     static void ergodicAllItem(QStandardItemModel* model,callback_ergodicFun_ptr pFun);
     static bool ergodicItem(QStandardItem* item,callback_ergodicFun_ptr pFun);
 private:
};

#endif // QSTANDARDITEMMODELEX_H
