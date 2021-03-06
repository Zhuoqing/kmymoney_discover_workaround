/*
 * This file is part of KMyMoney, A Personal Finance Manager f*or KDE
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

#ifndef ONLINEPLUGINEXTENDED_H
#define ONLINEPLUGINEXTENDED_H

#include "kmymoneyplugin.h"
#include <QtCore/QObject>
#include <QtCore/QList>

#include "mymoney/onlinejob.h"
#include "onlinetasks/interfaces/tasks/ionlinetasksettings.h"

class onlineTask;
class IonlineJobEdit;

namespace KMyMoneyPlugin
{

/**
 * @brief Interface between KMyMoney and Online Banking plugins for executing transactions
 *
 * This interface is under active development and will change often! Do not use it at the moment!
 *
 * @author Christian David (christian-david@web.de)
 */
class KMM_PLUGIN_EXPORT OnlinePluginExtended : public Plugin, public OnlinePlugin
{
  Q_OBJECT

public:
  OnlinePluginExtended(QObject* parent, const char* name);

  /**
   * @brief List onlineJobs supported by an account
   *
   * KMyMoney will use this function to ask the online plugin which online jobs it supports.
   * Later changes can be made public using the jobAvailable signals.
   *
   * @return A QStringList with supported onlineTask::name()s as values.
   */
  virtual QStringList availableJobs(QString accountId) = 0;

  /**
   * @brief Get settings for onlineTask
   *
   * @see onlineTask::settings
   */
  virtual IonlineTaskSettings::ptr settings(QString accountId, QString taskName) = 0;

  /**
   * @brief Send onlineJobs to bank
   *
   * @param jobs Do not delete the onlineJob objects. You can edit them but expect them to be deleted after
   * you returned from this function.
   */
  virtual void sendOnlineJob(QList<onlineJob>& jobs) = 0;


signals:
  /**
   * @brief Emit to make onlineJob available
   *
   * In case a onlineJob got available during runtime, emit one of these signals.
   */
  void jobAvailable(QString accountId, QString);
  void jobAvailable(QString accountId, QStringList);
  void jobUnavailable(QString accountId, QString);
  //void jobUnavailable( QString accountId );
};

class KMM_PLUGIN_EXPORT onlineTaskFactory
{
public:
  virtual onlineTask* createOnlineTask(const QString& taskId) const = 0;

  // Make g++ happy
  virtual ~onlineTaskFactory() {}
};

} // namespace KMyMoneyPlugin

Q_DECLARE_INTERFACE(KMyMoneyPlugin::OnlinePluginExtended, "org.kmymoney.plugin.onlinepluginextended");
Q_DECLARE_INTERFACE(KMyMoneyPlugin::onlineTaskFactory, "org.kmymoney.plugin.onlinetaskfactory");

#endif // ONLINEPLUGINEXTENDED_H
