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

#ifndef ONLINEJOBTYPEDTEST_H
#define ONLINEJOBTYPEDTEST_H

#include "QtCore/QObject"

#define KMM_MYMONEY_UNIT_TESTABLE friend class onlineJobTypedTest;

class onlineJobTypedTest : public QObject
{
  Q_OBJECT
private slots:
  void initTestCase();
  void cleanupTestCase();

  void copyContructor();
  void constructWithIncompatibleType();
  void constructWithNull();
  void copyByAssignment();
  void constructWithManadtoryDynamicCast();
};

#endif // ONLINEJOBTYPEDTEST_H
