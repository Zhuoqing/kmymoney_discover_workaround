/***************************************************************************
                          importinterface.h
                             -------------------
    begin                : Mon Apr 14 2008
    copyright            : (C) 2008 Thomas Baumgart
    email                : ipwizard@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef IMPORTINTERFACE_H
#define IMPORTINTERFACE_H

// ----------------------------------------------------------------------------
// QT Includes

#include <QObject>
#include <QString>

// ----------------------------------------------------------------------------
// KDE Includes

#include <kfile.h>
#include <kurl.h>

// ----------------------------------------------------------------------------
// Project Includes

#include <kmm_plugin_export.h>

namespace KMyMoneyPlugin
{

/**
  * This abstract class represents the ImportInterface to
  * add new importers to KMyMoney.
  */
class KMM_PLUGIN_EXPORT ImportInterface : public QObject
{
  Q_OBJECT

public:
  explicit ImportInterface(QObject* parent, const char* name = 0);
  ~ImportInterface() {}

  /**
   * This method is provided by KMyMoney to select a file to
   * be imported. A caption for the dialog can be provided via
   * @a title, a specific directory to be started with as @a path.
   * Which files are selectable is controlled via the contents
   * of @a mask. @a mode controls the behavior of the dialog. In case
   * the importer requires additional information, it can provide
   * a widget to ask for them. If none are required, pass 0.
   *
   * @note In case you create a widget and pass it to selectFile()
   * you are responsible to delete the widget. It will not be deleted
   * automatically during the destruction of the dialog.
   */
  virtual KUrl selectFile(const QString& title, const QString& path, const QString& mask, KFile::Mode mode, QWidget *widget) const = 0;

signals:
};

} // namespace
#endif
