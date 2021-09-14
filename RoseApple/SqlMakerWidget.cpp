#include <QtGui>
#include <QtWidgets>
#include <QtSql>

#include "SqlQuery.h"
#include "SqlHighlighter.h"
#include "SqlMakerWidget.h"

class SqlMakerWidgetData
{
public:
	SqlQuery* query;
	
	SqlMakerWidgetData()
	{
		query = 0;
	};
};

SqlMakerWidget::SqlMakerWidget(QWidget *parent, SqlQuery* q)
	: QWidget(parent)
{
	ui.setupUi(this);
	_d = new SqlMakerWidgetData();
	_d->query = q;

	if( q != 0 )
	{
		QSqlDatabase db = q->database();
		QString usr = db.userName().toUpper();
		QStringList tables = db.tables();
		qSort( tables );
		QStringList targets;
		foreach(QString t, tables)
		{
			QStringList list = t.split(".");
			if( list.count() == 0 ) continue;
			if( list.count() == 1 ) targets << list[0];
			else targets << list[1];
		}

		ui.tableCmb->addItems( targets );

		ui.tableDescLbl->clear();

		setWindowTitle(tr("Sql Maker[") + db.databaseName() + "]" );
	}
	else
	{
		ui.tableDescLbl->setText("You can select table if a DB is connected.");
	}

	new SqlHighlighter(ui.selectEdit->document());
	new SqlHighlighter(ui.insertEdit->document());
	new SqlHighlighter(ui.updateEdit->document());
	new SqlHighlighter(ui.deleteEdit->document());

	connect( ui.tableCmb, SIGNAL(editTextChanged(const QString&)), this, SLOT(setTable()) );
	connect( ui.fieldEdit, SIGNAL(textChanged()), this, SLOT(make()) );
	connect( ui.normalRdo, SIGNAL(toggled(bool)), this, SLOT(make()) );

	setTable();
}

SqlMakerWidget::~SqlMakerWidget()
{
	delete _d;
}


void SqlMakerWidget::setTable()
{
	if( _d->query == 0 ) 
	{
		make();
		return;
	}

	QString name = ui.tableCmb->currentText();
	QSqlDatabase db = _d->query->database();
	QSqlRecord r = db.record(name);

	if( r.count() )
	{
		QStringList list;
		for( int row = 0, cnt=r.count(); row < cnt; row++ )
		{
			list << r.field(row).name();
		}
		ui.fieldEdit->setText( list.join(", ") );
	}
}

void SqlMakerWidget::setField()
{
}

QString insertString(const QString& str, const QString& mark="?")
{
	QStringList l = str.split(",");
	QStringList out;
	for( int i = 0, cnt = l.count(); i < cnt; i++ ) out << mark;
	return out.join(", ");
}

QString SqlMakerWidget::updateString(const QString& str, const QString& mark)
{
	QStringList l = str.split(",");
	QStringList out;
	if( _d->query == 0 ) 
	{
		foreach(QString field, l)
		{
			out << QString("%1=%2").arg(field, mark);
		}
		return out.join(", ");
	}

	QStringList wheres;
	QSqlIndex key = _d->query->database().primaryIndex(ui.tableCmb->currentText());
	foreach(QString field, l)
	{
		if( key.contains(field) ) wheres << QString("%1=%2").arg(field, mark);
		else out << QString("%1=%2").arg(field, mark);
	}

	QString rst = out.join(", ");
	if( !wheres.isEmpty() ) rst += " \nwhere " + wheres.join(" and ");
	return rst;
}

QString SqlMakerWidget::whereString(const QString& str, const QString& mark)
{
	if( _d->query == 0 ) 
	{
		return "";
	}

	QStringList l = str.split(",");
	QStringList out;
	QStringList wheres;
	QSqlIndex key = _d->query->database().primaryIndex(ui.tableCmb->currentText());
	qDebug() << key;
	foreach(QString field, l)
	{
		if( key.contains(field) ) wheres << QString("%1=%2").arg(field, mark);
	}

	QString rst;
	if( !wheres.isEmpty() ) rst = wheres.join(" and ");
	return rst;
}

void SqlMakerWidget::make()
{
	QString mark = (ui.normalRdo->isChecked() ? " ": "?");
	QString table = ui.tableCmb->currentText();
	QString field = ui.fieldEdit->toPlainText().simplified();
	QString str = field;
	str.replace(" ", "");

	QString select = QString( "select %1 \nfrom %2 \nwhere %3" ).arg(field, table, whereString(str, mark));
	QString insert = QString("insert into %1(%2) values(%3)").arg( table, field, insertString(field, mark ) );
	QString update = QString("update %1 set %2").arg(table, updateString(str, mark));
	QString del = QString("delete from %1 \nwhere %2" ).arg(table, whereString(str, mark));

	ui.selectEdit->setText(select);
	ui.insertEdit->setText(insert);
	ui.updateEdit->setText(update);
	ui.deleteEdit->setText(del);

	// field count
	if( field.trimmed().isEmpty() ) ui.fieldCntLbl->clear();
	else ui.fieldCntLbl->setText(QString::number(field.split(",").count()) + " EA");
}

void SqlMakerWidget::on_arrangeBtn_clicked()
{
	QString field = ui.fieldEdit->toPlainText().trimmed().simplified();
	field.remove('\'').remove("\"");
	//field.replace(",", ", ");
	ui.fieldEdit->setText(field);
}
