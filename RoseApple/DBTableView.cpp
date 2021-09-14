#include <QtGui>
#include <QtWidgets>
#include <QtSql>

#include "SqlQuery.h"
#include "SqlHighlighter.h"
#include "UiControl.h"
#include "FilterDialog.h"
#include "DBTableView.h"

DBTableView::DBTableView(SqlQuery* q, QWidget *parent)
	: QWidget(parent), _q(q)
{
	ui.setupUi(this);

	ui.qryFilterEdit->hide();
	ui.filterApplyBtn->hide();

	UiControl::InitSplitter(ui.splitter, 2, 10);
	UiControl::InitSplitter(ui.vSplitter, 1, 9);

	getTables();
	QStringListModel* model = new QStringListModel(_map.values(_usr), ui.listView);
	
	ui.listView->setModel(model);

	_dataModel = new QSqlTableModel(ui.dataTable, q->database());
	ui.dataTable->setModel(_dataModel);
	_dataModel->select();
	_dataModel->setEditStrategy(QSqlTableModel::OnRowChange);

	UiControl::InitTable(ui.columnTable);

	connect( ui.listView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), 
		this, SLOT(selectTable(const QItemSelection&, const QItemSelection&)) );

	connect(ui.usrCmbbox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(on_usrCmbbox_textChanged(const QString &) ) );

	QSqlDatabase db = _q->database();
	if( db.isOpen() )
	{
		setWindowTitle(tr("Table View[") + db.databaseName() + "]" );
	}

	ui.ddlBrowser->setTabStopWidth(40);// tab=40pixels

	new SqlHighlighter(ui.ddlBrowser->document());
	new SqlHighlighter(ui.qryFilterEdit->document());

	connect( ui.tableRdo, SIGNAL(toggled(bool)), this, SLOT(getTables()) );
	connect( ui.viewRdo, SIGNAL(toggled(bool)), this, SLOT(getTables()) );
	connect( ui.sysTableRdo, SIGNAL(toggled(bool)), this, SLOT(getTables()) );
	connect( ui.allRdo, SIGNAL(toggled(bool)), this, SLOT(getTables()) );
}

DBTableView::~DBTableView()
{

}

void DBTableView::getTables()
{
	QSqlDatabase db = _q->database();
	_usr = db.userName().toUpper();

	QSql::TableType type = QSql::Tables;
	if( ui.viewRdo->isChecked() ) type = QSql::Views;
	else if( ui.sysTableRdo->isChecked() ) type = QSql::SystemTables;
	else if( ui.allRdo->isChecked() ) type = QSql::AllTables;
	QStringList tables = db.tables(type);
	_map.clear();
	foreach(QString t, tables)
	{
		QStringList list = t.split(".");
		if( list.count() == 0 ) continue;
		if( list.count() == 1 ) _map.insertMulti(_usr, list[0]);
		else _map.insertMulti(list[0], list[1]);
	}

	ui.usrCmbbox->clear();
	ui.usrCmbbox->addItems(_map.uniqueKeys());
	ui.usrCmbbox->setCurrentIndex(ui.usrCmbbox->findText(_usr));
}

SqlQuery* DBTableView::query()
{
	return _q;
}

void DBTableView::selectTable(const QItemSelection& selected, const QItemSelection&)
{
	QModelIndexList mList = selected.indexes();
	if( mList.isEmpty() ) return;

	_refreshList.clear();

	_filterMap[_oldTableName] = ui.qryFilterEdit->toPlainText();
	QString name = currentTableName();
	ui.qryFilterEdit->setPlainText(_filterMap[name]);

	int type = ui.tabWidget->currentIndex();
	on_tabWidget_currentChanged(type);
}

void DBTableView::on_filterEdit_textChanged(const QString &txt)
{	
	QSqlDatabase db = _q->database();

	QStringListModel* model = (QStringListModel*)ui.listView->model();
	if( model == 0 ) return;
	
	QStringList l = _map.values(ui.usrCmbbox->currentText());
	QString filter = txt.trimmed();
	if(filter.isEmpty() ) model->setStringList(l);
	else 
	{
		l = l.filter(filter, Qt::CaseInsensitive);
		model->setStringList(l);
	}
}

void DBTableView::on_usrCmbbox_textChanged(const QString &txt)
{
	ui.filterEdit->clear();
	on_filterEdit_textChanged("");
	_oldTableName.clear();
}

void DBTableView::on_listView_customContextMenuRequested(QPoint)
{
	if( ui.listView->model()->rowCount() <= 0 ) return;

	QMenu menu("Menu", this);
	menu.addAction("Get &Select Query", this, SLOT(getSelectQuery()));
	menu.addAction("Record &Count", this, SLOT(recordCount()));

	menu.exec(QCursor::pos());
}

void DBTableView::getSelectQuery()
{
	QString name = currentTableName();
	QSqlDatabase db = _q->database();
	QSqlRecord r = db.record(name);
	int cnt = r.count();

	QString qry = "select ";
	for( int row = 0; row < cnt; row++ )
	{
		qry += r.field(row).name() + ", ";
	}
	qry.chop(2);
	QApplication::clipboard()->setText(qry + " from " + name);
}

void DBTableView::recordCount()
{
	QString name = currentTableName();

	QSqlQuery q = _q->select("select count(1) from " + name);
	if( q.next() ) 
	{
		ui.tableNameLbl->setText( name + " : " + q.value(0).toString() );
	}
}

QString DBTableView::currentTableName()
{
	QModelIndexList mList = ui.listView->selectionModel()->selectedIndexes();
	if( mList.isEmpty() ) return "";

	QString name = mList.at(0).data().toString();
	QSqlDatabase db = _q->database();
	QString usr2 = ui.usrCmbbox->currentText();
	if( _usr != usr2 ) name = usr2 + "." + name;

	return name;
}

void DBTableView::createDDL()
{
	QString name = currentTableName();
	if( name.isEmpty() ) return;

	QString ddl;

	QSqlDatabase db = _q->database();
	QSqlQuery q(db);

	QString fieldQuery;
	if( db.driverName() == "QSQLITE" )
	{
		fieldQuery = QString("select sql from sqlite_master where tbl_name = '%1' ").arg(name);
		q.exec(fieldQuery);
		while( q.next() ) 
		{
			if( q.value(0).isNull() ) continue;
			ddl += q.value(0).toString() + "\n\n";
		}
		if( !ddl.isEmpty() ) ddl.chop(2);
	}
	else if( db.driverName() == "QPSQL" )
	{
		ddl = QString("CREATE TABLE %1 (\n").arg(name);
		fieldQuery = "select t.column_name || ' ' || "
			"upper( udt_name || (case when data_type = 'character varying' then '(' || character_maximum_length || ')' "
			"when data_type = 'numeric' then (case when numeric_scale > 0 then '(' || numeric_precision || ', ' || numeric_scale || ')' else '(' || numeric_precision || ')' end) "
			"else '' end)) || '' || "
			"case when is_nullable='NO' then ' NOT NULL,' else ',' end clm from information_schema.columns as t "
			"where table_name = '%1'";

		q.exec(fieldQuery.arg(name));
		while( q.next() )
		{
			ddl += "\t" + q.value(0).toString() + " \n";
		}

		QString pkeyQuery = "select c.constraint_name, u.column_name "
			"from information_schema.key_column_usage u "
			"left join information_schema.table_constraints c on u.table_name=c.table_name and u.constraint_name = c.constraint_name "
			"where u.table_name = '%1' and c.constraint_type='PRIMARY KEY'";
		q.exec(pkeyQuery.arg(name));
		QString pkeyName;
		QStringList pkeyList;
		while( q.next() )
		{
			if( pkeyName.isEmpty() ) pkeyName = q.value(0).toString();
			pkeyList << q.value(1).toString();
		}
		if( !pkeyName.isEmpty() )
		{
			ddl += QString("\tCONSTRAINT %1 PRIMARY KEY (%2), \n").arg(pkeyName, pkeyList.join(", "));
		}

		QString fkeyQuery = "select c.constraint_name, u.column_name, u.table_name, 'ON UPDATE ' || r.update_rule, 'ON DELETE ' || r.delete_rule "
			"from INFORMATION_SCHEMA.KEY_COLUMN_USAGE u "
			"left join information_schema.referential_constraints r on u.constraint_name = r.unique_constraint_name "
			"left join information_schema.table_constraints c on c.constraint_name=r.constraint_name "
			"where c.table_name='%1' and c.constraint_type='FOREIGN KEY' "
			"order by u.ordinal_position";
		q.exec(fkeyQuery.arg(name));

		QString fkeyName;
		QString fkeyRefTableName;
		QStringList fkeyList;
		QString onUpdateAction;
		QString onDeleteAction;
		while( q.next() )
		{
			if( fkeyName.isEmpty() ) 
			{
				fkeyName = q.value(0).toString();
				fkeyRefTableName = q.value(2).toString();
				onUpdateAction = q.value(3).toString();
				onDeleteAction = q.value(4).toString();
			}
			fkeyList << q.value(1).toString();
		}
		if( !fkeyName.isEmpty() )
		{
			ddl += QString("\tCONSTRAINT %1 FOREIGN KEY (%2) \n"
				"\t\tREFERENCES %3(%2) \n\t\t%4 \n\t\t%5\n").arg(fkeyName, fkeyList.join(", "), fkeyRefTableName, onUpdateAction, onDeleteAction);
		}
		ddl += ");";
	}

	//ui.ddlBrowser->setHtml(ddl);
	ui.ddlBrowser->setPlainText(ddl);
}

void DBTableView::queryFiltering()
{
	QString name = currentTableName();
	if( name.isEmpty() ) return;

	_filterString = ui.qryFilterEdit->toPlainText();

	int type = ui.tabWidget->currentIndex();

	if( type == 1 )
	{
		QSqlDatabase db = _q->database();

		QString qryStr = "select * from " + _oldTableName + (_filterString.isEmpty() ? "" : " where " + _filterString);

		//_dataModel->setQuery( qryStr, db);
		_dataModel->setTable(name);
		if( !_filterString.isEmpty() ) _dataModel->setFilter(_filterString);
		_dataModel->select();
		ui.dataTable->resizeColumnsToContents();
		//ui.dataTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
		QSqlQuery q = _q->select(qryStr.replace("*", "count(1)"));
		if( q.next() ) ui.dataLabel->setText(QString("Count:%1").arg(q.value(0).toInt()));
		else ui.dataLabel->setText("Count:0");
	}
}

void DBTableView::keyPressEvent ( QKeyEvent* e )
{

	if( ui.qryFilterEdit->hasFocus() && e->modifiers() == Qt::CTRL && e->key() == Qt::Key_Return ) {
		queryFiltering();
	}

	QWidget::keyPressEvent(e);
}

void DBTableView::on_tabWidget_currentChanged(int type)
{
	// 테이블이름 선택이 달라지지 않았다면 그냥 리턴
	QString name = currentTableName();
	if( _refreshList.contains(type) && name == _oldTableName ) return;

	_refreshList << type;

	_oldTableName = name;
	QSqlDatabase db = _q->database();

	//int type = ui.tabWidget->currentIndex();

	// make ddl script
	createDDL();

	if( type == 0 ) {
		ui.tableNameLbl->setText(name);

		QString tableInfoQueryStr;

		if( db.driverName() == "QSQLITE" )
		{
			tableInfoQueryStr = QString("PRAGMA table_info(%1)").arg(name);
		}
		else if( db.driverName() == "QPSQL" )
		{
			tableInfoQueryStr = 
				QString("select f1 as \"No\", f2 as \"Field Name\", f3 as \"Field Type\", min(f4) as \"Key\", f5 as \"Null\", f6 as \"Default\" from ("
				"select t.ordinal_position as f1, t.column_name as f2, "
					"udt_name || (case when data_type = 'character varying' then '(' || character_maximum_length || ')' "
					"when data_type = 'numeric' then (case when numeric_scale > 0 then '(' || numeric_precision || ', ' || numeric_scale || ')' else '(' || numeric_precision || ')' end) "
					"else '' end) as f3, "
					"p.constraint_type f4, "
					"is_nullable as f5, column_default as f6 "
				"from information_schema.columns as t "
					"left join information_schema.key_column_usage c on t.table_name=c.table_name and t.column_name=c.column_name "
					"left join information_schema.table_constraints p on c.table_name = p.table_name and c.constraint_name=p.constraint_name " 
				"where t.table_name = '%1' "
				") as tbl group by f1, f2, f3, f5, f6 "
				"order by 1").arg(name);
		}

		if( !tableInfoQueryStr.isEmpty() )
		{
			QSqlQueryModel* m = new QSqlQueryModel(ui.columnTable);
			m->setQuery( tableInfoQueryStr, db);
			ui.columnTable->setModel(m);

			if( m->query().lastError().isValid() )
			{
				qDebug() << tableInfoQueryStr;
				qDebug() << m->query().lastError();
			}

			ui.columnTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
			return;
		}

		QSqlRecord r = db.record(name);
		QSqlIndex key = db.primaryIndex(name);
		int cnt = r.count();

		int t = 0;
		QStandardItemModel* model = new QStandardItemModel(cnt, 6);
		model->setHeaderData(t++, Qt::Horizontal, "Field Name");
		model->setHeaderData(t++, Qt::Horizontal, "Key");
		model->setHeaderData(t++, Qt::Horizontal, "Field Type");
		model->setHeaderData(t++, Qt::Horizontal, "Length");
		model->setHeaderData(t++, Qt::Horizontal, "Precisioin");
		model->setHeaderData(t++, Qt::Horizontal, "Null");
		
		QStringList l;
		for( int row = 0; row < cnt; row++ )
		{
			QSqlField f = r.field(row);
			t = 0;
			model->setItem( row, t++, new QStandardItem( f.name() ) );
			model->setItem( row, t++, new QStandardItem( (key.contains(f.name())? "Y":"")));
			model->setItem( row, t++, new QStandardItem( QVariant::typeToName(f.type())));
			//model->setItem( row, t++, new QStandardItem( (f.length()<0 ? "" : QString::number(f.length())) ));
			model->setItem( row, t++, new QStandardItem( QString::number((uint)f.length()) ));
			model->setItem( row, t++, new QStandardItem( (f.precision()<0 ? "" :QString::number(f.precision())) ) );
			model->setItem( row, t++, new QStandardItem( (f.requiredStatus() == QSqlField::Required	? "Not null" : "")) );
		}

		ui.columnTable->setModel(model);
		ui.columnTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
	}
	else if( type == 1 )
	{
		queryFiltering();
	}
}

void DBTableView::on_filterApplyBtn_clicked()
{
	queryFiltering();
}
