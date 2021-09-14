#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QKeyEvent>
#include <QTextCursor>
#include <QTextBlock>
#include <QTime>

#include "NewConnectionDlg.h"
#include "UiControl.h"
#include "SqlHighlighter.h"
#include "QueryWidget.h"

class QueryWidgetData
{
public:
	QSqlQueryModel*	m;
	QSqlDatabase	db;
	QTime			t;
};

QueryWidget::QueryWidget(const QSqlDatabase& db, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	_d = new QueryWidgetData();
	_d->m = new QSqlQueryModel(ui.tableView);
	_d->db = db;
	ui.tableView->setModel(_d->m);

	SqlHighlighter* h = new SqlHighlighter(ui.textEdit->document());

	UiControl::InitSplitter(ui.splitter, 2, 6, 2);

	if( _d->db.isOpen() )
	{
		setWindowTitle(tr("Sql Query[") + _d->db.databaseName() + "]" );
		QString qry = NewConnectionDlg::getLastQuery(_d->db);
		ui.textEdit->setPlainText(qry);
	}
}

QueryWidget::~QueryWidget()
{
	if( _d->db.isOpen() )
	{
		NewConnectionDlg::setLastQuery(_d->db, ui.textEdit->toPlainText());
	}
	delete _d;
}

void QueryWidget::on_exeBtn_clicked()
{
	if( !_d->db.isOpen() ) return;

	_d->t.start();
	QString qryStr = ui.textEdit->toPlainText().simplified();
	_d->m->setQuery(qryStr, _d->db);
	int msec = _d->t.elapsed();

	setResultLog(msec);
}

void QueryWidget::on_lineExeBtn_clicked()
{
	if( !_d->db.isOpen() ) return;

	_d->t.start();
	QString qryStr = ui.textEdit->textCursor().block().text();
	_d->m->setQuery(qryStr, _d->db);

	int msec = _d->t.elapsed();
	setResultLog(msec);
}

void QueryWidget::on_selExeBtn_clicked()
{
	if( !_d->db.isOpen() ) return;

	_d->t.start();
	QString qryStr = ui.textEdit->textCursor().selectedText().simplified();
	_d->m->setQuery(qryStr, _d->db);
	int msec = _d->t.elapsed();
	setResultLog(msec);
}

void QueryWidget::keyReleaseEvent ( QKeyEvent * e )
{
	if( e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_F9 )	// query all
	{
		on_exeBtn_clicked();
	}
	else if( e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_Return )	// query current line
	{
		on_lineExeBtn_clicked();
	}
	else if( e->key() == Qt::Key_F9 )	// query selected
	{
		on_selExeBtn_clicked();
	}

	QWidget::keyReleaseEvent( e );
}

void QueryWidget::setResultLog(int msec)
{
	ui.tableView->resizeColumnsToContents();
	ui.logEdit->appendPlainText(_d->m->query().lastQuery());

	QSqlError e = _d->m->lastError();
	QString log;
	if(e.isValid() )
	{
		log = QString("%1, %2").arg(e.number()).arg(e.text());
	}
	else
	{
		int cnt = _d->m->rowCount();
		if( cnt == 0 ) log = tr("Empty set(%1 ms)").arg(msec);
		else log = tr("%1 Row%2 returned(%3 ms)").arg(cnt).arg(cnt == 0 ? "" : "s").arg(msec);
	}
	ui.logEdit->appendPlainText(log + "\n");
}
