/***************************************************************************
                          knewfiledlg.h
                             -------------------
    copyright            : (C) 2000 by Michael Edwardes
    email                : mte@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KNEWFILEDLG_H
#define KNEWFILEDLG_H

// ----------------------------------------------------------------------------
// QT Includes
#include <QDialog>

// ----------------------------------------------------------------------------
// KDE Includes
#include <klocale.h>
#include <kpushbutton.h>

// ----------------------------------------------------------------------------
// Project Includes

struct ContactData;

// This dialog lets the user create/edit a file.
// Use the second constructor to edit a file.
class KNewFileDlg : public QDialog
{
  Q_OBJECT
public:
  explicit KNewFileDlg(QWidget *parent = 0, const QString& title = QString());
  explicit KNewFileDlg(QString userName, QString userStreet,
                       QString userTown, QString userCounty, QString userPostcode, QString userTelephone,
                       QString userEmail, QWidget *parent = 0, const QString& title = QString());
  ~KNewFileDlg();

  KPushButton* cancelButton();

public:
  QString userNameText;
  QString userStreetText;
  QString userTownText;
  QString userCountyText;
  QString userPostcodeText;
  QString userTelephoneText;
  QString userEmailText;

protected:
  /// helper method for constructors
  void init(const QString& title);

protected slots:
  void okClicked();
  void loadFromAddressBook();
  /** Gets the result of searching for the contact details of the current user */
  void slotContactFetched(const ContactData &identity);

private:
  /// \internal d-pointer class.
  struct Private;
  /// \internal d-pointer instance.
  Private* const d;
};

#endif
