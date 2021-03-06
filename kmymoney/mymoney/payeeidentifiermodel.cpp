/*
 * This file is part of KMyMoney, A Personal Finance Manager for KDE
 * Copyright (C) 2015  Christian David <christian-david@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "payeeidentifiermodel.h"

#include <algorithm>

#include <klocalizedstring.h>
#include <QDebug>

#include "mymoneyfile.h"

payeeIdentifierModel::payeeIdentifierModel(QObject* parent)
    : QAbstractItemModel(parent),
    m_payeeIdentifierIds(),
    m_typeFilter()
{

}

void payeeIdentifierModel::setTypeFilter(QStringList filter)
{
  m_typeFilter = filter;
  loadData();
}

void payeeIdentifierModel::setTypeFilter(QString type)
{
  setTypeFilter(QStringList(type));
}


void payeeIdentifierModel::loadData()
{
  beginResetModel();

  const QList<MyMoneyPayee> payees = MyMoneyFile::instance()->payeeList();
  m_payeeIdentifierIds.clear();
  m_payeeIdentifierIds.reserve(payees.count());
  Q_FOREACH(const MyMoneyPayee& payee, payees) {
    m_payeeIdentifierIds.append(payee.id());
  }
  endResetModel();
}

MyMoneyPayee payeeIdentifierModel::payeeByIndex(const QModelIndex& index) const
{
  if (index.isValid() && index.row() >= 0 && index.row() < m_payeeIdentifierIds.count()) {
    try {
      return MyMoneyFile::instance()->payee(m_payeeIdentifierIds.at(index.row()));
    } catch (MyMoneyException&) {
    }
  }

  return MyMoneyPayee();
}

QVariant payeeIdentifierModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  const bool isPayeeIdentifier = index.parent().isValid();
  if (role == payeeIdentifierModel::isPayeeIdentifier)
    return isPayeeIdentifier;

  const MyMoneyPayee payee = (isPayeeIdentifier) ? payeeByIndex(index.parent()) : payeeByIndex(index);


  if (role == payeeName || (!isPayeeIdentifier && role == Qt::DisplayRole)) {
    // Return data for MyMoneyPayee
    return payee.name();
  } else if (isPayeeIdentifier) {
    // Return data for payeeIdentifier
    if (index.row() >= 0 && static_cast<unsigned int>(index.row()) < payee.payeeIdentifierCount()) {
      ::payeeIdentifier ident = payee.payeeIdentifier(index.row());

      if (role == payeeIdentifier) {
        return QVariant::fromValue< ::payeeIdentifier >(ident);
      } else if (ident.isNull()) {
        return QVariant();
      } else if (role == payeeIdentifierType) {
        return ident.iid();
      } else if (role == Qt::DisplayRole) {
        // The custom delegates won't ask for this role
        return QVariant::fromValue(i18n("The plugin to show this information could not be found."));
      }
    }
  }

  return QVariant();
}

Qt::ItemFlags payeeIdentifierModel::flags(const QModelIndex &index) const
{
  Q_UNUSED(index)
#if 0
  if (!index.parent().isValid()) {
    if (payeeByIndex(index).payeeIdentifierCount() > 0)
      return Qt::ItemIsEnabled;
  }
#endif

  return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

/**
 * @intenal The internalId of QModelIndex is set to the row of the parent or -1 if there is no
 * parent.
 *
 * @todo Qt5: the type of the internal id changed!
 */
QModelIndex payeeIdentifierModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid())
    return createIndex(row, column, parent.row());
  return createIndex(row, column, -1);
}

int payeeIdentifierModel::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return 1;
}

int payeeIdentifierModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid()) {
    if (parent.internalId() != -1)
      return 0;
    return payeeByIndex(parent).payeeIdentifierCount();
  }
  return m_payeeIdentifierIds.count();
}

QModelIndex payeeIdentifierModel::parent(const QModelIndex& child) const
{
  if (child.internalId() != -1)
    return createIndex(child.internalId(), 0, -1);
  return QModelIndex();
}
