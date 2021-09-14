#include <QtGui>
#include <QtWidgets>

#include "SqlQuery.h"
#include "UiControl.h"
#include "NewConnectionDlg.h"
#include "SqlMakerWidget.h"
#include "DataImportWidget.h"
#include "CSTextManageWidget.h"
#include "QueryWidget.h"
#include "DBTableView.h"

#include "RoseApple.h"

class RoseAppleData
{
public:
	SqlQuery*	query;

	RoseAppleData()
	{
		query = 0;
	};
};

RoseApple::RoseApple(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	_d = new RoseAppleData();
	ui.gridLayout->setContentsMargins(0, 0, 0, 0);
}

RoseApple::~RoseApple()
{

}


void RoseApple::on_actionQuit_triggered()
{
	close();
}

void RoseApple::on_actionSQL_Maker_triggered()
{
	SqlMakerWidget* w = new SqlMakerWidget(this, _d->query);
	QMdiSubWindow* subWindow = ui.mdiArea->addSubWindow(w);
	subWindow->setAttribute(Qt::WA_DeleteOnClose, true);
	subWindow->resize(800, 600);
	subWindow->show();
}

void RoseApple::on_actionTappedView_triggered()
{
	ui.mdiArea->setViewMode(QMdiArea::TabbedView);
	ui.actionTappedView->setChecked(true);
	ui.actionSubwindow_View->setChecked(false);
}

void RoseApple::on_actionSubwindow_View_triggered()
{
	ui.mdiArea->setViewMode(QMdiArea::SubWindowView);
	ui.actionTappedView->setChecked(false);
	ui.actionSubwindow_View->setChecked(true);
}

void RoseApple::on_actionCascadeView_triggered()
{
	ui.mdiArea->cascadeSubWindows();
}

void RoseApple::on_actionTileView_triggered()
{
	ui.mdiArea->tileSubWindows();
}

void RoseApple::on_actionClose_All_triggered()
{
	ui.mdiArea->closeAllSubWindows();
}


void RoseApple::on_actionConnectDB_triggered()
{
	NewConnectionDlg d(this);
	if( d.exec() == QDialog::Rejected || d.query() == 0 ) return;

	_d->query = d.query();
}

void RoseApple::on_actionDBTableView_triggered()
{
	if( _d->query == 0 )
	{
		QMessageBox::warning( this, windowTitle(), "Database is not connected." );
		return;
	}

	DBTableView* w = new DBTableView(_d->query, this);
	QMdiSubWindow* subWindow = ui.mdiArea->addSubWindow(w);
	subWindow->setAttribute(Qt::WA_DeleteOnClose, true);
	subWindow->resize(800, 600);
	subWindow->show();
}

void RoseApple::on_actionDataImpot_triggered()
{
	DataImportWidget* w = new DataImportWidget(this, _d->query);
	QMdiSubWindow* subWindow = ui.mdiArea->addSubWindow(w);
	subWindow->setAttribute(Qt::WA_DeleteOnClose, true);
	subWindow->resize(800, 600);
	subWindow->show();
}

void RoseApple::on_actionCSTextManager_triggered()
{
	CSTextManageWidget* w = new CSTextManageWidget(this);
	QMdiSubWindow* subWindow = ui.mdiArea->addSubWindow(w);
	subWindow->setAttribute(Qt::WA_DeleteOnClose, true);
	subWindow->resize(800, 600);
	subWindow->show();
}

void RoseApple::on_actionQuery_triggered()
{
	QSqlDatabase db;
	if( _d->query ) db = _d->query->database();
	QueryWidget* w = new QueryWidget(db, this);
	QMdiSubWindow* subWindow = ui.mdiArea->addSubWindow(w);
	subWindow->setAttribute(Qt::WA_DeleteOnClose, true);
	subWindow->resize(800, 600);
	subWindow->show();
}
