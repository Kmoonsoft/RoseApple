#include <QtGui>
#include <QtWidgets>
#include <QtSql>

#include "SqlQuery.h"
#include "UiControl.h"
#include "TableSelectWidget.h"

class TableSelectWidgetData
{
public:
	QSqlDatabase db;
	QStringListModel * model;
	QString table;
	QStringList tables;
	int odType;
	
	TableSelectWidgetData()
	{
		model = 0;
		odType = 0;
	};
};


TableSelectWidget::TableSelectWidget(QSqlDatabase& db, QWidget *parent, Qt::WindowFlags f, ODBCType type )
	: QDialog(parent, f)
{
	ui.setupUi(this);

	_d = new TableSelectWidgetData();

	_d->db = db;
	_d->odType = type;
	if( db.isValid() && db.isOpen() )
	{
		QString usr = _d->db.userName().toUpper();
		_d->tables = _d->db.tables(_d->odType == EXCEL ? QSql::AllTables : QSql::Tables);
		QStringList targets;
		foreach(QString t, _d->tables)
		{
			QStringList list = t.split(".");
			if( list.count() == 0 ) continue;
			if( list.count() == 1 ) targets << list[0];
			else targets << list[1];
		}

		_d->model = new QStringListModel(ui.tables);
		_d->model->setStringList(targets);
		ui.tables->setModel(_d->model);
	}
}

TableSelectWidget::~TableSelectWidget()
{
	delete _d;
}

void TableSelectWidget::on_ok_clicked()
{
	QItemSelectionModel* sel = ui.tables->selectionModel();

	if( !sel->hasSelection() ) 
	{
		MSG_WARNING("No selection!");
		return;
	}

	_d->table = sel->selectedIndexes()[0].data().toString();
	QDialog::accept();
}

QString TableSelectWidget::selectedTable()
{
	return _d->table;
}

QStringList TableSelectWidget::tables()
{
	return _d->tables;
}

void TableSelectWidget::on_cancel_clicked()
{
	QDialog::reject();
}

