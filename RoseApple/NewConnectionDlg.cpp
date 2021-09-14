#include <QtWidgets>
#include <QtGui>
#include <QtSql>

#include "SqlQuery.h"
#include "UiControl.h"

#include "NewConnectionDlg.h"

const QString connInfoFileName = "connections";

// Connection information structure
class ConnInfo : QObject
{
public :
	bool savePwd;
	QString drive;
	QString host;
	QString	dbName;
	QString	port;
	QString	user;
	QString	pwd;
	QString file;

	ConnInfo::ConnInfo() : savePwd(false), drive(""), host(""), dbName(""), port(""), user(""), pwd(""), file("")
	{};
};

QDataStream & operator>> ( QDataStream & stream, ConnInfo & info ) 
{
	stream >> info.savePwd;
	stream >> info.drive;
	stream >> info.host;
	stream >> info.dbName;
	stream >> info.port;
	stream >> info.user;
	stream >> info.pwd;
	stream >> info.file;

	return stream;
}

QDataStream & operator<< ( QDataStream & stream, const ConnInfo & info ) 
{
	stream << info.savePwd;
	stream << info.drive;
	stream << info.host;
	stream << info.dbName;
	stream << info.port;
	stream << info.user;
	stream << info.pwd;
	stream << info.file;

	return stream;
}

// Class NewConnectionDlg
NewConnectionDlg::NewConnectionDlg(QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f), _query(0)
{
	ui.setupUi(this);

	ui.extWidget->setVisible(false);
	connect(ui.connectionsBtn, SIGNAL(toggled(bool)), ui.extWidget, SLOT(setVisible(bool)));
	connect(ui.driveCmbbox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(on_driveCmbbox_textChanged(const QString&) ) );

	ui.driveCmbbox->addItems( QSqlDatabase::drivers() );

	UiControl::InitTable(ui.tableView);
	ui.tableView->setModel( new QSqlQueryModel(ui.tableView) );
	ui.tableView->verticalHeader()->setVisible(true);

	connect( ui.tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), 
		this, SLOT(selected(const QItemSelection&, const QItemSelection&)) );

	QString connFile = QDir::homePath() + "\\" + connInfoFileName;
	_connQuery = new SqlQuery(this);
	_connQuery->setConnection("QSQLITE", connFile, "", 0, "", "");
	int loop = 3;
	while(loop--)	// test loop count
	{
		if( _connQuery->open() )
		{
			// sqlite contains table defined by connInfoFileName
			QStringList tl = _connQuery->database().tables();
			if( tl.count() == 0 || !tl.contains(connInfoFileName) )	// if there is no table
			{	// create table
				QString createSql = QString("create table %1 ("
					"SavePwd varchar, Driver varchar, Host varchar, DBName varchar, Port varchar, User varchar, Password varchar, File varchar, LastQuery varchar)" ).arg(connInfoFileName);
				_connQuery->query(createSql);
			}
			break;
		}
		else
		{	// delete before connFile
			QFile::remove(connFile);
		}
	}

	load();
}

NewConnectionDlg::~NewConnectionDlg()
{
}

SqlQuery* NewConnectionDlg::query() const
{
	return _query;
}

void NewConnectionDlg::on_cancelBtn_clicked()
{
	reject();
}

void NewConnectionDlg::on_connectBtn_clicked()
{
	_query = new SqlQuery(parent());
	QString drive = ui.driveCmbbox->currentText();
	
	_query->setConnection(drive, ui.nameEdit->text(), ui.hostEdit->text(), ui.portEdit->text().toInt(), ui.userEdit->text(), ui.passwordEdit->text());
	if( ui.fileEdit->isEnabled() && !ui.fileEdit->text().isEmpty() ) _query->setFileName(QFileInfo(ui.fileEdit->text()).fileName() );
	if( _query->open() )
	{
		ConnInfo info;
		info.savePwd = ui.savePwdChkbox->checkState() == Qt::Checked;
		info.drive = drive;
		info.host = ui.hostEdit->text();
		info.dbName = ui.nameEdit->text();
		info.port = ui.portEdit->text();
		info.user = ui.userEdit->text();
		info.pwd = ui.passwordEdit->text();
		info.file = ui.fileEdit->text();

		deleteInfo(info);	// delete previous info
		addConnInfo(info);
	
		accept();
	}
	else
	{
		QSqlError err = _query->database().lastError();
		QMessageBox::critical(this, "Alley", "Fail to connect to the database!\n"
			+ err.text());
	}
}

void NewConnectionDlg::on_driveCmbbox_textChanged(const QString& name)
{
	bool fileVisible = ( name == "QSQLITE" || name == "QODBC" || name == "QODBC3" );
	ui.fileWidget->setEnabled(fileVisible);
	ui.editWidget->setEnabled(!fileVisible);

	if( name.contains("QOCI") ) ui.portEdit->setText( "1521" );
	if( name == "QMYSQL" ) ui.portEdit->setText( "60891" );

	if( !fileVisible ) {
		ui.fileEdit->clear();
	}
	else {
		ui.nameEdit->clear();
		ui.hostEdit->clear();
		ui.portEdit->clear();
		ui.userEdit->clear();
		ui.passwordEdit->clear();
	}
}

void NewConnectionDlg::on_fileBtn_clicked()
{
	QString drive = ui.driveCmbbox->currentText();
	QString filter;
	if( drive == "QODBC" || drive == "QODBC3" ) filter = "Ms Access Files(*.mdb *.accdb)";
	else if( drive == "QSQLITE" ) filter = "DB Files(*.*)";
	else filter = "All Files(*.*)";

	QString fileName = QFileDialog::getOpenFileName(this, "Select a database file", QApplication::applicationDirPath(), filter );
	ui.fileEdit->setText(fileName);
	if( fileName.isEmpty() ) return;

	QString name;
	//if( drive == "QODBC" ) name = "DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=" + fileName;
	if( drive == "QODBC" || drive == "QODBC3") name = "DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=" + fileName;
	else if( drive == "QSQLITE" ) name = fileName;
	ui.nameEdit->setText(name);
}

bool NewConnectionDlg::deleteInfo(const ConnInfo& info) 
{
	QSqlQuery q = _connQuery->select( QString("select LastQuery from %1 "
		"where Driver = '%2' and Host = '%3' and DBName = '%4' and Port = '%5' and User = '%6' and File = '%7'")
		.arg(connInfoFileName, info.drive, info.host, info.dbName, info.port, info.user, info.file) );
	if( q.next() ) _lastQry = q.value(0).toString();

	return _connQuery->query( QString("delete from %1 "
		"where Driver = '%2' and Host = '%3' and DBName = '%4' and Port = '%5' and User = '%6' and File = '%7'")
		.arg(connInfoFileName, info.drive, info.host, info.dbName, info.port, info.user, info.file) );
}

void NewConnectionDlg::addConnInfo(const ConnInfo& info)
{
	_connQuery->query(QString("insert into %1(SavePwd, Driver, Host, DBName, Port, User, Password, File, LastQuery) "
		"values ('%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10')")
		.arg(connInfoFileName, info.savePwd ? "Yes" : "No", info.drive, info.host, info.dbName, info.port, info.user, info.savePwd ? info.pwd : "", info.file).arg(_lastQry) );

	load();
}

void NewConnectionDlg::load()
{
	((QSqlQueryModel*)ui.tableView->model())->setQuery( QString("select SavePwd, Driver, Host, DBName, Port, User, Password, File from %1").arg(connInfoFileName), _connQuery->database() );
	
	ui.tableView->hideColumn(6);
	ui.tableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void NewConnectionDlg::selected(const QItemSelection& selected, const QItemSelection&)
{
	QModelIndexList mList = selected.indexes();
	if( mList.isEmpty() ) return;

	ui.savePwdChkbox->setChecked( mList.at(0).data().toString() == "Yes");
	ui.driveCmbbox->setCurrentIndex(ui.driveCmbbox->findText(mList.at(1).data().toString()));
	ui.hostEdit->setText(mList.at(2).data().toString());
	ui.nameEdit->setText(mList.at(3).data().toString());
	ui.portEdit->setText(mList.at(4).data().toString());
	ui.userEdit->setText(mList.at(5).data().toString());
	ui.passwordEdit->setText(mList.at(6).data().toString());
	ui.fileEdit->setText(mList.at(7).data().toString());
}


void NewConnectionDlg::on_tableView_customContextMenuRequested(QPoint)
{
	QMenu menu("Menu", this);
	menu.addAction("Clear &Selected", this, SLOT(clearSelected()));
	menu.addAction("Clear &All", this, SLOT(clearAll()));
	menu.exec(QCursor::pos());
}

void NewConnectionDlg::clearSelected()
{
	QModelIndexList mList = ui.tableView->selectionModel()->selectedIndexes();
	if( mList.isEmpty() ) return;

	ConnInfo info;
	info.savePwd = mList.at(0).data().toString() == "Yes";
	info.drive = mList.at(1).data().toString();
	info.host = mList.at(2).data().toString();
	info.dbName = mList.at(3).data().toString();
	info.port = mList.at(4).data().toString();
	info.user = mList.at(5).data().toString();
	info.pwd = mList.at(6).data().toString();
	info.file = mList.at(7).data().toString();

	if( deleteInfo(info) ) load();
	else QMessageBox::warning( this, "Alley", "Fail to delete the selected information" );
}

void NewConnectionDlg::clearAll()
{
	if( _connQuery->query(QString("delete from %1").arg(connInfoFileName) ) ) load();
	else QMessageBox::warning( this, "Alley", "Fail to delete all information" );
}


void NewConnectionDlg::on_tableView_doubleClicked(QModelIndex)
{
	this->on_connectBtn_clicked();
}

QString	NewConnectionDlg::getLastQuery( const QSqlDatabase& db )
{
	QString connFile = QDir::homePath() + "\\" + connInfoFileName;
	SqlQuery connQuery(0);
	connQuery.setConnection("QSQLITE", connFile, "", 0, "", "");

	QSqlQuery q = connQuery.select( QString("select LastQuery from %1 where Driver='%2' and Host='%3' and DBName='%4' and User='%5'")
		.arg(connInfoFileName).arg(db.driverName()).arg(db.hostName()).arg(db.databaseName()).arg(db.userName()) );
	QString text;
	if( q.next() ) text = q.value(0).toString();

	q.clear();
	connQuery.close();
	return text;
}

bool NewConnectionDlg::setLastQuery( const QSqlDatabase& db, const QString& query )
{
	QString connFile = QDir::homePath() + "\\" + connInfoFileName;
	SqlQuery connQuery(0);
	connQuery.setConnection("QSQLITE", connFile, "", 0, "", "");
	bool rst = connQuery.query( QString("update %1 set LastQuery=? where Driver='%2' and Host='%3' and DBName='%4' and User='%5'")
		.arg(connInfoFileName).arg(db.driverName()).arg(db.hostName()).arg(db.databaseName()).arg(db.userName()), query );

	connQuery.close();
	return rst;
}
