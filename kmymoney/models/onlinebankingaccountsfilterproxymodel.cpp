/*
 * This file is part of KMyMoney, A Personal Finance Manager for KDE
 * Copyright (C) 2014 Christian Dávid <christian-david@web.de>
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

#include "onlinebankingaccountsfilterproxymodel.h"

#include "accountsmodel.h"
#include "mymoney/onlinejobadministration.h"

OnlineBankingAccountsFilterProxyModel::OnlineBankingAccountsFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

bool OnlineBankingAccountsFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
  const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
  const QString accountId = sourceModel()->data(sourceIndex, AccountsModel::AccountIdRole).toString();
  if (accountId.isEmpty())
    return false;
  else if (onlineJobAdministration::instance()->isAnyJobSupported(accountId))
    return true;

  return filterAcceptsParent(sourceIndex);
}

Qt::ItemFlags OnlineBankingAccountsFilterProxyModel::flags(const QModelIndex& index) const
{
  const QString accountId = sourceModel()->data(mapToSource(index), AccountsModel::AccountIdRole).toString();
  if (onlineJobAdministration::instance()->isAnyJobSupported(accountId))
    return QSortFilterProxyModel::flags(index);
  return QSortFilterProxyModel::flags(index) & ~Qt::ItemIsSelectable;
}


bool OnlineBankingAccountsFilterProxyModel::filterAcceptsParent(const QModelIndex& index) const
{
  const int rowCount = sourceModel()->rowCount(index);
  for (int i = 0; i < rowCount; i++) {
    const QModelIndex childIndex = sourceModel()->index(i, 0, index);
    if (onlineJobAdministration::instance()->isAnyJobSupported(sourceModel()->data(childIndex, AccountsModel::AccountIdRole).toString()))
      return true;
    if (filterAcceptsParent(childIndex))
      return true;
  }
  return false;
}
