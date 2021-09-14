#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractItemModel>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QHeaderView>

#include <QtDebug>

#include "UiControl.h"
#include "GTableView.h"

GTableView::GTableView(QWidget *parent)
	: QTableView(parent)
{
	UiControl::InitTable2(this);
}

GTableView::~GTableView()
{

}

void GTableView::resizeColumnsToContents()
{
	QTableView::resizeColumnsToContents();
	QHeaderView* v = horizontalHeader();
	for( int c = 0, cnt=v->count(); c < cnt; c++ )
	{
		if( v->sectionSize(c) > 500 ) v->resizeSection(c, 500);
	}
}

void GTableView::contextMenuEvent ( QContextMenuEvent * event )
{
	QMenu menu("Menu", this);
	menu.addAction("Selected to clipboard", this, SLOT(saveToClip()) );
	menu.addAction("Save to excel", this, SLOT(saveToExcel()));

	menu.exec(QCursor::pos());
}

void GTableView::keyReleaseEvent ( QKeyEvent * e )
{
	if( e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_C )	// copy
	{
		saveToClip();
	}

	QTableView::keyReleaseEvent( e );
}

void GTableView::saveToClip()
{
	QModelIndexList idxList = selectedIndexes();
	
	QString text;
	int row = 0;
	foreach( QModelIndex i, idxList )
	{
		if( i.row() != row )
		{
			text.chop(2);
			text += "\n";
			row = i.row();
		}
		text += i.data().toString() + ", ";
	}

	text.chop(2);

	QApplication::clipboard()->setText(text);
}

void GTableView::saveToExcel()
{
	QString filePath = QFileDialog::getSaveFileName(this, "Save to a [MS Excel] file", ".", "Excel File(*.xls *.xlsx)" );
	if( filePath.isEmpty() ) return;

	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", QString("Excel%1").arg((long long)this) );
	QString dbName = QString("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%1\";FIL={MS Excel};DBQ=").arg(filePath) + filePath;
	db.setDatabaseName(dbName);

	if( !db.open() )
	{
		MSG_CRITICAL( tr("Fail to create excel file.\n\n") + db.lastError().text() );
		return;
	}

	QSqlQuery q(db);

	QAbstractItemModel* m = model();
	if( m == 0 )
	{
		MSG_WARNING( tr("There is no data") );
		return;
	}
	
	int rowCnt = m->rowCount();
	int colCnt = m->columnCount();
	QStringList heads;
	QStringList ps;
	for( int c = 0; c < colCnt; c++ )
	{
		heads << "\"" + m->headerData(c, Qt::Horizontal).toString() + "\"";
		ps << "?";
	}

	QString qryStr = "create table Sheet ( " + heads.join(" Text,") + " Text)";
	qDebug() << qryStr;
	qDebug() << "Create table : " << q.exec(qryStr) << q.lastError().text();

	qryStr = "insert into Sheet values(" + ps.join(",") + ")";
	for( int r = 0; r < rowCnt; r++ )
	{
		q.prepare(qryStr);
		for( int c = 0; c < colCnt; c++ )
		{
			q.addBindValue(m->data(m->index(r, c)));
		}
		qDebug() << r << q.exec();

		qApp->processEvents();
	}
	db.close();
}
