/***************************************************************************
                         kofxdirectconnectdlg.cpp
                             -------------------
    begin                : Sat Nov 13 2004
    copyright            : (C) 2002 by Ace Jones
    email                : acejones@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kofxdirectconnectdlg.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QLabel>
#include <QDir>
#include <QFile>
#include <QTextStream>

// ----------------------------------------------------------------------------
// KDE Includes

#include <kurl.h>
#include <kjob.h>
#include <kio/job.h>
#include <kio/jobclasses.h>
#include <kio/jobuidelegate.h>
#include <kdebug.h>
#include <ktemporaryfile.h>
#include <kprogressdialog.h>
#include <kmessagebox.h>
#include <klocale.h>

// ----------------------------------------------------------------------------
// Project Includes

#include <mymoneyinstitution.h>
#include <mymoneyfile.h>
#include "mymoneyofxconnector.h"
#include "../ofxpartner.h"

class KOfxDirectConnectDlg::Private
{
public:
  Private() : m_firstData(true) {}
  QFile    m_fpTrace;
  bool     m_firstData;
};

KOfxDirectConnectDlg::KOfxDirectConnectDlg(const MyMoneyAccount& account, QWidget *parent) :
    KOfxDirectConnectDlgDecl(parent),
    d(new Private),
    m_tmpfile(0),
    m_connector(account),
    m_job(0)
{
}

KOfxDirectConnectDlg::~KOfxDirectConnectDlg()
{
  if (d->m_fpTrace.isOpen()) {
    d->m_fpTrace.close();
  }
  delete m_tmpfile;
  delete d;
}

bool KOfxDirectConnectDlg::init()
{
  show();

  QByteArray request = m_connector.statementRequest();
  if (request.isEmpty()) {
    hide();
    return false;
  }

  // For debugging, dump out the request
#if 0
  QFile g("request.ofx");
  g.open(QIODevice::WriteOnly);
  QTextStream(&g) << m_connector.url() << "\n" << QString(request);
  g.close();
#endif

  QDir homeDir(QDir::home());
  if (homeDir.exists("ofxlog.txt")) {
    d->m_fpTrace.setFileName(QString("%1/ofxlog.txt").arg(QDir::homePath()));
    d->m_fpTrace.open(QIODevice::WriteOnly | QIODevice::Append);
  }

  if (d->m_fpTrace.isOpen()) {
    QByteArray data = m_connector.url().toUtf8();
    d->m_fpTrace.write("url: ", 5);
    d->m_fpTrace.write(data, strlen(data));
    d->m_fpTrace.write("\n", 1);
    d->m_fpTrace.write("request:\n", 9);
    QByteArray trcData(request);  // make local copy
    trcData.replace('\r', "");
    d->m_fpTrace.write(trcData, trcData.size());
    d->m_fpTrace.write("\n", 1);
    d->m_fpTrace.write("response:\n", 10);
  }

  KTemporaryFile tmpDownload;
  if (tmpDownload.open() && OfxPythonHttpsRequest("POST", m_connector.url(), request, QMap<QString, QString>(), tmpDownload.fileName())) {
    qDebug("OfxPython Emit statementReady signal with '%s'", qPrintable(tmpDownload.fileName()));
    emit statementReady(tmpDownload.fileName());
    qDebug("OfxPython Return from signal statementReady() processing");
    hide();
    return false;
  }

  qDebug("creating job");
  m_job = KIO::http_post(m_connector.url(), request, KIO::HideProgressInfo);

  // open the temp file. We come around here twice if init() is called twice
  if (m_tmpfile) {
    kDebug(0) << "Already connected, using " << m_tmpfile->fileName();
    delete m_tmpfile; //delete otherwise we mem leak
  }
  m_tmpfile = new KTemporaryFile();
  // for debugging purposes one might want to leave the temp file around
  // in order to achieve this, please uncomment the next line
  // m_tmpfile->setAutoRemove(false);
  if (!m_tmpfile->open()) {
    qWarning("Unable to open tempfile '%s' for download.", qPrintable(m_tmpfile->fileName()));
    return false;
  }

  m_job->addMetaData("content-type", "Content-type: application/x-ofx");

  connect(m_job, SIGNAL(result(KJob*)), this, SLOT(slotOfxFinished(KJob*)));
  connect(m_job, SIGNAL(data(KIO::Job*,QByteArray)), this, SLOT(slotOfxData(KIO::Job*,QByteArray)));

  setStatus(QString("Contacting %1...").arg(m_connector.url()));
  kProgress1->setMaximum(3);
  kProgress1->setValue(1);
  return true;
}

void KOfxDirectConnectDlg::setStatus(const QString& _status)
{
  textLabel1->setText(_status);
  kDebug(0) << "STATUS:" << _status;
}

void KOfxDirectConnectDlg::setDetails(const QString& _details)
{
  kDebug(0) << "DETAILS: " << _details;
}

void KOfxDirectConnectDlg::slotOfxData(KIO::Job*, const QByteArray& _ba)
{
  qDebug("Got %d bytes of data", _ba.size());
  if (d->m_firstData) {
    setStatus("Connection established, retrieving data...");
    setDetails(QString("Downloading data to %1...").arg(m_tmpfile->fileName()));
    kProgress1->setValue(kProgress1->value() + 1);
    d->m_firstData = false;
  }
  m_tmpfile->write(_ba);

  setDetails(QString("Got %1 bytes").arg(_ba.size()));

  if (d->m_fpTrace.isOpen()) {
    QByteArray trcData(_ba);
    trcData.replace('\r', "");
    d->m_fpTrace.write(trcData, trcData.size());
  }
}

void KOfxDirectConnectDlg::slotOfxFinished(KJob* /* e */)
{
  qDebug("Job finished");
  kProgress1->setValue(kProgress1->value() + 1);
  setStatus("Completed.");

  if (d->m_fpTrace.isOpen()) {
    d->m_fpTrace.write("\nCompleted\n\n\n\n", 14);
  }

  int error = m_job->error();

  if (m_tmpfile) {
    qDebug("Closing tempfile");
    m_tmpfile->close();
  }
  qDebug("Tempfile closed");

  if (error) {
    qDebug("Show error message");
    m_job->ui()->setWindow(0);
    m_job->ui()->showErrorMessage();
  } else if (m_job->isErrorPage()) {
    qDebug("Process error page");
    QString details;
    if (m_tmpfile) {
      QFile f(m_tmpfile->fileName());
      if (f.open(QIODevice::ReadOnly)) {
        QTextStream stream(&f);
        QString line;
        while (!stream.atEnd()) {
          details += stream.readLine(); // line of text excluding '\n'
        }
        f.close();

        kDebug(0) << "The HTTP request failed: " << details;
      }
    }
    KMessageBox::detailedSorry(this, i18n("The HTTP request failed."), details, i18nc("The HTTP request failed", "Failed"));
  } else if (m_tmpfile) {
    qDebug("Emit statementReady signal with '%s'", qPrintable(m_tmpfile->fileName()));
    emit statementReady(m_tmpfile->fileName());
    qDebug("Return from signal statementReady() processing");
  }
  delete m_tmpfile;
  m_tmpfile = 0;
  hide();
  qDebug("Finishing slotOfxFinished");
}

void KOfxDirectConnectDlg::reject()
{
  if (m_job)
    m_job->kill();
  if (m_tmpfile) {
    m_tmpfile->close();
    delete m_tmpfile;
    m_tmpfile = 0;
  }
  QDialog::reject();
}
