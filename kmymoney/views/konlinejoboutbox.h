/*
 * This file is part of KMyMoney, A Personal Finance Manager for KDE
 * Copyright (C) 2013 Christian Dávid <christian-david@web.de>
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

#ifndef KONLINEJOBOUTBOX_H
#define KONLINEJOBOUTBOX_H

#include <QWidget>
#include <QModelIndex>

#include "onlinejob.h"

namespace Ui
{
class KOnlineJobOutbox;
}

class KOnlineJobOutbox : public QWidget
{
  Q_OBJECT

public:
  explicit KOnlineJobOutbox(QWidget *parent = 0);
  ~KOnlineJobOutbox();

  QStringList selectedOnlineJobs() const;

  void showEvent(QShowEvent* event);

signals:
  void sendJobs(QList<onlineJob>);
  void editJob(QString);
  void newCreditTransfer();

  void aboutToShow();
  void showContextMenu(onlineJob);

protected:
  void contextMenuEvent(QContextMenuEvent*);

private slots:
  void updateNewCreditTransferButton();
  void updateButtonState() const;

private:
  Ui::KOnlineJobOutbox *ui;

private slots:
  void slotRemoveJob();

  /** @brief If any job is selected, send it. Send all valid jobs otherwise. */
  void slotSendJobs();

  /** @brief Send all sendable online jobs */
  void slotSendAllSendableJobs();

  /** @brief Send only the selected jobs */
  void slotSendSelectedJobs();

  void slotEditJob();
  void slotEditJob(const QModelIndex&);
};

#endif // KONLINEJOBOUTBOX_H
