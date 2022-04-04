// Copyright (C) 2001 - 2013 Rob Caelers & Raymond Penners
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef PREFERENCESDIALOG_HH
#define PREFERENCESDIALOG_HH

#include <memory>

#include <QtGui>
#include <QtWidgets>

#include "ui/IApplicationContext.hh"

#include "DataConnector.hh"
#include "SizeGroup.hh"

class IconListNotebook;

class PreferencesDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PreferencesDialog(std::shared_ptr<IApplicationContext> app);

private:
  void add_page(const QString &label, const char *image, QWidget *page);

  auto create_timer_page() -> QWidget *;
  auto create_ui_page() -> QWidget *;

private:
  std::shared_ptr<IApplicationContext> app;
  IconListNotebook *notebook{nullptr};

  std::shared_ptr<SizeGroup> hsize_group;
  std::shared_ptr<SizeGroup> vsize_group;

  DataConnector::Ptr connector;
};

#endif // PREFERENCESDIALOG_HH
