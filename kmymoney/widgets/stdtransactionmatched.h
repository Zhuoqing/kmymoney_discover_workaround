/***************************************************************************
                          stdtransactionmatched.h
                             -------------------
    begin                : Sat May 31 2008
    copyright            : (C) 2008 by Thomas Baumgart
    email                : Thomas Baumgart <ipwizard@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STDTRANSACTIONMATCHED_H
#define STDTRANSACTIONMATCHED_H

// ----------------------------------------------------------------------------
// QT Includes
#include <QBrush>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include <transaction.h>

namespace KMyMoneyTransactionForm
{
class TransactionForm;
} // namespace

namespace KMyMoneyRegister
{

class StdTransactionMatched : public StdTransaction
{
  static const int m_additionalRows = 3;

public:
  StdTransactionMatched(Register* parent, const MyMoneyTransaction& transaction, const MyMoneySplit& split, int uniqueId);
  virtual ~StdTransactionMatched() {}

  virtual const char* className() {
    return "StdTransactionMatched";
  }

  virtual bool paintRegisterCellSetup(QPainter *painter, QStyleOptionViewItemV4 &option, const QModelIndex &index);

  void registerCellText(QString& txt, Qt::Alignment& align, int row, int col, QPainter* painter = 0);

  /**
    * Provided for internal reasons. No API change. See RegisterItem::numRowsRegister(bool)
    */
  int numRowsRegister(bool expanded) const {
    return StdTransaction::numRowsRegister(expanded) + m_additionalRows;
  }

  /**
    * Provided for internal reasons. No API change. See RegisterItem::numRowsRegister()
    */
  int numRowsRegister() const {
    return StdTransaction::numRowsRegister();
  }
};

} // namespace

#endif
