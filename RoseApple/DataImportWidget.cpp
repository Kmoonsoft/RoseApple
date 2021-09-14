#include <QtGui>
#include <QtWidgets>
#include <QtSql>

#include "SqlQuery.h"
#include "UiControl.h"
#include "TableSelectWidget.h"
#include "DataImportWidget.h"

class Highlighter : public QSyntaxHighlighter
{
public:
	Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent){};
	~Highlighter(){};

protected:

	void highlightBlock(const QString &text)
	{
		QTextCharFormat myClassFormat;
		myClassFormat.setFontWeight(QFont::Bold);
		myClassFormat.setForeground(Qt::red);
		QString pattern = "[Ee][Rr][Rr][Oo][Rr]*";

		QRegExp expression(pattern);
		int index = text.indexOf(expression);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, myClassFormat);
			index = text.indexOf(expression, index + length);
		}
	};
};

class DataImportWidgetData
{
public:
	SqlQuery* query;
	QSqlQueryModel* model;
	QStringList fields;
	QStringList targetFields;

	QString sourceTableName;
	QString targetTableName;
	QSqlDatabase sourceDB;
	int odbcType;
	bool cancel;

	DataImportWidgetData()
	{
		query = 0;
		model = 0;
		odbcType = 0;
	};
};

// constructor
// parent Widget, Target DB Query
DataImportWidget::DataImportWidget(QWidget *parent, SqlQuery* q)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.progressBar->hide();
	_d = new DataImportWidgetData();
	_d->query = q;

	ui.cancel->hide();
	UiControl::InitTable( ui.fields );
	UiControl::InitTable( ui.datas );
	if( q != 0 )
	{
		QSqlDatabase db = q->database();
		QString usr = db.userName().toUpper();
		QStringList tables = db.tables();
		QStringList targets;
		qSort( tables );
		foreach(QString t, tables)
		{
			QStringList list = t.split(".");
			if( list.count() == 0 ) continue;
			if( list.count() == 1 ) targets << list[0];
			else targets << list[1];
		}

		ui.tableCmb->addItems( targets );

		ui.tableDescLbl->clear();

		_d->model = new QSqlQueryModel(ui.datas);
		ui.datas->setModel(_d->model);
		setTable();

		setWindowTitle(tr("Data Inport[") + db.databaseName() + "]" );
	}
	else
	{
		ui.tableDescLbl->setText("You can select table if a DB is connected.");
	}

	connect( ui.tableCmb, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(setTable()) );
	connect( ui.from, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(tableChanged()) );

	new Highlighter(ui.log->document());
}

// Destructor
DataImportWidget::~DataImportWidget()
{
	if( _d->sourceDB.isOpen() )
	{
		_d->sourceDB.close();
		QSqlDatabase::removeDatabase(_d->sourceDB.connectionName());
	}

	delete _d;	// delete private data
}

// source DB table changed and set fields
void DataImportWidget::setTable()
{
	if( _d->query == 0 ) return;	// this case is not occured

	ui.fields->setRowCount(0);	// clear previous field items
	_d->targetFields.clear();
	
	_d->targetTableName = ui.tableCmb->currentText();
	QSqlDatabase db = _d->query->database();
	QSqlRecord r = db.record(_d->targetTableName);

	int cnt = r.count();	// field count
	if( cnt )
	{
		ui.fields->setRowCount(cnt);
		for( int row = 0; row < cnt; row++ )
		{
			QString filedName = r.field(row).name();
			_d->targetFields << filedName;
			QTableWidgetItem* item = new QTableWidgetItem(filedName);
			ui.fields->setItem(row, 0, item);	// first column is source field coulum
		}
	}
	ui.fields->resizeColumnsToContents();

	_d->model->setQuery(QString("select * from %1").arg(_d->targetTableName), _d->query->database());	// select data

	ui.datas->resizeColumnsToContents();
	tableChanged();
}

// source file select
void DataImportWidget::on_file_2_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Database File", "", "Database(*.mdb *.accdb *.xls *.xlsx);;All Files(*.*)");
	if( filePath.isEmpty() ) return;

	QFileInfo fi(filePath);
	QString fileName = fi.fileName();
	QString ext = fi.suffix().toLower();

	QString connName = QString::number((long long)this);
	if( ext == "mdb" || ext == "accdb" || ext == "xls" || ext == "xlsx" )
	{
		_d->sourceDB = QSqlDatabase::addDatabase( "QODBC", connName );
		QString dbName;
		if( ext == "mdb" || ext == "accdb" ) dbName = "DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=";
		else 
		{
			dbName = "DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};FIL={MS Excel};DBQ=";
			_d->odbcType = TableSelectWidget::EXCEL;
		}
		dbName += filePath;

		_d->sourceDB.setConnectOptions("SQL_ATTR_ACCESS_MODE=SQL_MODE_READ_ONLY" );
		_d->sourceDB.setDatabaseName( dbName );

		if( !_d->sourceDB.open() )
		{
			MSG_CRITICAL( tr("Cannot open this database file.\n\n") + _d->sourceDB.lastError().text() );
			return;
		}
	}
	else
	{
		MSG_INFORMATION( tr("Not support yet!") );
		return;
	}

	TableSelectWidget d(_d->sourceDB, this, Qt::Dialog, (TableSelectWidget::ODBCType)_d->odbcType );

	if( d.exec() != QDialog::Accepted ) return;

	ui.from->clear();
	ui.from->addItems(d.tables());
	_d->sourceTableName = d.selectedTable();
	ui.from->setCurrentIndex(ui.from->findText(_d->sourceTableName) );
	if( _d->sourceTableName.isEmpty() ) return;

	//QSqlRecord r = _d->sourceDB.record(_d->sourceTableName);
	//int cnt = r.count();

	//_d->fields.clear();

	//for( int row = 0; row < cnt; row++ )
	//{
	//	_d->fields << r.field(row).name();
	//}

	//for( int i = 0, c = ui.fields->rowCount(); i < c; i++ )
	//{
	//	QComboBox* cmbbox = new QComboBox(ui.fields);
	//	cmbbox->setModel( new QStringListModel(_d->fields) );
	//	ui.fields->setCellWidget(i, 1, cmbbox);
	//	if( i < _d->fields.count() ) cmbbox->setCurrentIndex(i);
	//}
}

// start import data
void DataImportWidget::on_get_clicked()
{
	_d->cancel = false;
	if( !_d->sourceDB.isValid() || !_d->sourceDB.isOpen() ) 
	{
		MSG_WARNING( "Source DB is not open!" );
		return;
	}

	if( _d->query == 0 ) 
	{
		MSG_WARNING( "Target DB is not open!" );
		return;
	}

	QString sourceTableName = _d->sourceTableName;
	if( _d->odbcType == (int)TableSelectWidget::EXCEL )
	{
		sourceTableName = "[" + sourceTableName + "]";	// excle sheet table needs braket '[]'
	}

	QSqlQuery q(_d->sourceDB);
	q.setForwardOnly(true);

	int cnt = 0;
	if( q.exec( QString("select count(1) from %1").arg(sourceTableName) ) )
	{
		if( q.next() )
		{
			cnt = q.value(0).toInt();
		}
		else
		{
			MSG_WARNING( "Source data is empty" );
			ui.log->append( q.lastError().text() );
			return;
		}
	}
	else
	{
		MSG_WARNING( "Fail to query source data!" );
		ui.log->append( q.lastError().text() );
		return;
	}

	ui.progressBar->show();
	ui.log->append(QString("Import is started : %1").arg(_d->targetTableName));
	ui.progressBar->setMaximum(cnt);
	ui.progressBar->setValue(0);

	QStringList sourceFields;

	QString qry = QString("insert into %1(%2) values(" ).arg(_d->targetTableName, _d->targetFields.join(",") );

	QString fieldQry;

	foreach( QString f, _d->targetFields )
	{
		fieldQry += QString("?,");
	}
	fieldQry.chop(1);
	qry += fieldQry;
	qry += ")";

	qDebug() << qry;

	for( int i = 0; i < ui.fields->rowCount(); i++ )
	{
		sourceFields << "\"" + ((QComboBox*)ui.fields->cellWidget(i, 1))->currentText() + "\"";
	}

	QString selectQry = QString("select %1 from %2").arg(sourceFields.join(","), sourceTableName);
	
	ui.cancel->show();
	ui.get->hide();

	int total = 0;
	int imported = 0;
	int fCount = sourceFields.count();
	if( q.exec(selectQry) )
	{
		int p = 1;
		while(q.next() )
		{
			if( _d->cancel ) break;

			total++;
			ui.progressBar->setValue(p++);
			QSqlQuery t(_d->query->database());
			t.prepare(qry);
			for( int i = 0; i < fCount; i++ )
			{
				t.addBindValue(q.value(i));
			}

			if( !t.exec() ) 
			{
				ui.log->append(t.lastError().text() );
				if( ui.tabWidget->currentIndex() != 0 ) ui.tabWidget->setCurrentIndex(0);
			}
			else imported++;

			qApp->processEvents();
		}
	}
	else
	{
		ui.log->append(QString("Fail : %1").arg(q.lastError().text()));
	}

	_d->model->setQuery(QString("select * from %1").arg(_d->targetTableName), _d->query->database());
	ui.datas->resizeColumnsToContents();

	ui.log->append( QString("\n%1/%2 EA is imported.").arg(imported).arg(total) );
	ui.log->append(QString("Import is finished : %1\n").arg(_d->targetTableName) );
	ui.progressBar->hide();
	ui.cancel->hide();
	ui.get->show();
}

void DataImportWidget::tableChanged()
{
	_d->sourceTableName = ui.from->currentText();

	if( _d->sourceTableName.isEmpty() ) return;

	QSqlRecord r = _d->sourceDB.record(_d->sourceTableName);
	int cnt = r.count();

	_d->fields.clear();

	for( int row = 0; row < cnt; row++ )
	{
		_d->fields << r.field(row).name();
	}

	for( int i = 0, c = ui.fields->rowCount(); i < c; i++ )
	{
		QComboBox* cmbbox = new QComboBox(ui.fields);
		cmbbox->setModel( new QStringListModel(_d->fields) );
		ui.fields->setCellWidget(i, 1, cmbbox);
		if( i < _d->fields.count() ) cmbbox->setCurrentIndex(i);
	}
}

void DataImportWidget::on_cancel_clicked()
{
	_d->cancel = true;
}

void DataImportWidget::on_deleteAll_clicked()
{
	if( 1 == QMessageBox::question(this, windowTitle(), tr("Are you sure you want to delete all data from the database?"), tr("Yes"), tr("No")) )
	{
		return;
	}

	QSqlQuery q(_d->query->database());
	if( !q.exec("delete from " + _d->targetTableName) )
	{
		ui.log->append(QString("Fail to delete all data.\n") + q.lastError().text() );
	}

	_d->model->setQuery(QString("select * from %1").arg(_d->targetTableName), _d->query->database());
	ui.datas->resizeColumnsToContents();
}