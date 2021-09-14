#include <QtGui>
#include <QtWidgets>
#include <QtSql>
#include <QApplication>

#include "UiControl.h"

#include "CSTextManageWidget.h"

struct CSTextManageWidgetData
{
	QSqlDatabase db;
	QSqlDatabase tdb;
	QString tableName;

	CSTextManageWidgetData()
	{
		tdb = QSqlDatabase::addDatabase("QSQLITE");
		tdb.setDatabaseName("sdata");
		if( tdb.open() )
		{
			qDebug() << "Target Database is opened(SQLITE).";
			QSqlQuery q(tdb);
			q.exec("create table sdata(data text)" );
		}
		else
		{
			qDebug() << "Fail to open target Database(SQLITE).";
		}
	};
};

CSTextManageWidget::CSTextManageWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	_d = new CSTextManageWidgetData();

	UiControl::InitSplitter(ui.splitter, 6, 4);
	ui.progressBar->hide();

	connect( ui.distinct, SIGNAL(toggled(bool)), this, SLOT(distinctChanged(bool)) );
}

CSTextManageWidget::~CSTextManageWidget()
{
	delete _d;
}

void CSTextManageWidget::on_fileOpen_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Data file open", ".", "Excel File(*.xls *.xlsx);;CSV File(*.csv *.txt)");
	if(filePath.isEmpty() ) return;

	QString conn = QString("CSV%1").arg((long long)this);
	if( _d->db.isOpen() ) _d->db.close();
	_d->db = QSqlDatabase::addDatabase("QODBC", conn );
	
	QFileInfo fi(filePath);
	QString fileName = fi.fileName();
	QString ext = fi.suffix().toLower();

	ui.fileName->setText(filePath);

	QString dbName;
	if( ext == "txt" || ext == "csv" )
	{
		dbName = "DRIVER={Microsoft Text Driver (*.txt; *.csv)};FIL={MS Text};DBQ=" + filePath;
	}
	else
	{
		dbName = "DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};FIL={MS Excel};DBQ=" + filePath;
	}
	_d->db.setConnectOptions("SQL_ATTR_ACCESS_MODE=SQL_MODE_READ_ONLY" );
	_d->db.setDatabaseName( dbName );

	if( !_d->db.open() )
	{
		MSG_CRITICAL( tr("Cannot open this database file.\n\n") + _d->db.lastError().text() );
		return;
	}

	QStringList tables;
	tables << _d->db.tables(QSql::AllTables);
	if( tables.isEmpty() ) 
	{
		MSG_WARNING( tr("There is no table in this file.") );
		return;
	}

	_d->tableName = tables[0];
	QSqlQueryModel* m = new QSqlQueryModel(ui.sourceView);
	m->setQuery( QString("Select * from [%1]").arg(_d->tableName), _d->db );
	ui.sourceView->setModel(m);

	connect( ui.sourceView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), 
		this, SLOT(selectTable(const QItemSelection&, const QItemSelection&)) );
}

void CSTextManageWidget::selectTable(const QItemSelection& selected, const QItemSelection& old)
{
	QModelIndexList mList = selected.indexes();
	if( mList.isEmpty() ) return;

	int col = mList[0].column();

	QSqlQuery q(_d->db);
	q.setForwardOnly(true);
	QSqlRecord r = _d->db.record(_d->tableName);

	if( r.count() <= col ) 
	{
		qDebug( "Error! Field count is less than column" );
		return;
	}

	int count = 0;
	if( !q.exec( QString("select count(1) from [%1]").arg(_d->tableName) ) )
	{
		MSG_CRITICAL(q.lastError().text() );
		return;
	}

	if( q.next() ) count = q.value(0).toInt();
	ui.progressBar->setMaximum(count);
	ui.progressBar->setValue(0);

	ui.count->setText(tr("Count:") + QString::number(count) );

	if( !q.exec( QString("select %1 from [%2]").arg(r.field(col).name(), _d->tableName) ) )
	{
		MSG_CRITICAL(q.lastError().text() );
		return;
	}

	ui.progressBar->show();
	QSqlQuery tq(_d->tdb);
	qDebug() << "Delete previous target data : " << tq.exec("delete from sdata");
	QStringList strings;
	tq.prepare("insert into sdata values(?)");
	int c = 0;
	while( q.next() )
	{
		QStringList data = q.value(0).toString().simplified().split(",", QString::SkipEmptyParts);
		foreach( QString d, data)
		{
			tq.bindValue(0, d.simplified());
			tq.exec();
		}
		ui.progressBar->setValue(++c);
		qApp->processEvents();
	}
	
	//tq.addBindValue(strings);

	//qDebug() << "Insert : " << tq.execBatch();

	//QStringListModel* m = new QStringListModel(strings, ui.targetView );
	QSqlQueryModel* m = new QSqlQueryModel(ui.targetView);
	m->setQuery(QString("select %1 data from sdata").arg(ui.distinct->isChecked() ? "distinct" : ""), _d->tdb);

	ui.targetView->setModel(m);

	ui.progressBar->hide();
}

void CSTextManageWidget::distinctChanged(bool checked)
{
	QSqlQueryModel* m =(QSqlQueryModel*)ui.targetView->model();
	if( m == 0 ) return;

	m->setQuery(QString("select %1 data from sdata").arg(ui.distinct->isChecked() ? "distinct" : ""), _d->tdb);
}
