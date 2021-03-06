/***************************************************************************
                         interesteditwizardpage  -  description
                            -------------------
   begin                : Sun Jul 4 2010
   copyright            : (C) 2010 by Fernando Vilas
   email                : kmymoney-devel@kde.org
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef INTERESTEDITWIZARDPAGE_H
#define INTERESTEDITWIZARDPAGE_H

// ----------------------------------------------------------------------------
// QT Includes

#include <QWizardPage>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include "ui_interesteditwizardpagedecl.h"

/**
 * This class implements the Interest Edit page of the
 * @ref KNewLoanWizard.
 */
class InterestEditWizardPageDecl : public QWizardPage, public Ui::InterestEditWizardPageDecl
{
public:
  InterestEditWizardPageDecl(QWidget *parent) : QWizardPage(parent) {
    setupUi(this);
  }
};

class InterestEditWizardPage : public InterestEditWizardPageDecl
{
  Q_OBJECT
public:
  explicit InterestEditWizardPage(QWidget *parent = 0);

  /**
   * Overload the isComplete function to control the Next button
   */
  bool isComplete() const;

};

#endif
