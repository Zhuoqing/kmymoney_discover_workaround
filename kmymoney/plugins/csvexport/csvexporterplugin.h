/***************************************************************************
                             csvexporterplugin.h
                             -------------------
    begin                : Wed Mar 20 2013
    copyright            : (C) 2013 by Allan Anderson
    email                : agander93@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CSVEXPORTERPLUGIN_H
#define CSVEXPORTERPLUGIN_H

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// QT Includes

#include <QList>
#include <QStringList>

// Project Includes

#include "kmymoneyplugin.h"

class CsvExportDlg;
class CsvWriter;

class CsvExporterPlugin : public KMyMoneyPlugin::Plugin
{
  Q_OBJECT

public:
  explicit CsvExporterPlugin(QObject* parent, const QVariantList&);
  ~CsvExporterPlugin();

  KAction*          m_action;
  bool              okToWriteFile(const KUrl& url);
  CsvExportDlg*     exporterDialog() {
    return m_dlg;
  }

private:
  CsvExportDlg*     m_dlg;

protected slots:
  void slotCsvExport();

protected:
  void createActions();
};

#endif
