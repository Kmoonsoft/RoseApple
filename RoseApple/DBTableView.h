/******************************************************************************
** 파일명 : DBTableView.h
** 버전   : 1.0
** 갱신일 : 2011/08/23
** 만든이 : Lucky Kim(neoeeff@gmail.com)
** 설  명 : 데이터베이스 테이블 목록 위젯
******************************************************************************/
#ifndef DBTABLEVIEW_H
#define DBTABLEVIEW_H

#include <QWidget>
#include "ui_DBTableView.h"

class SqlQuery;
class QItemSelection;
class QSqlTableModel;

class DBTableView : public QWidget
{
	Q_OBJECT

public:
	DBTableView(SqlQuery* q, QWidget *parent = 0);
	~DBTableView();

	SqlQuery* query();

private slots:
	void on_filterApplyBtn_clicked();
	void on_tabWidget_currentChanged(int);
	void queryFiltering();
	void on_listView_customContextMenuRequested(QPoint);
	void on_usrCmbbox_textChanged(const QString &);
	void on_filterEdit_textChanged(const QString &);
	void selectTable(const QItemSelection&, const QItemSelection&);
	void getSelectQuery();
	void recordCount();

	void getTables();

private:
	QString currentTableName();
	void	createDDL();

protected:
	void keyPressEvent ( QKeyEvent * e );

private:
	Ui::DBTableViewClass ui;

	SqlQuery* _q;
	QMultiMap<QString, QString>	_map;
	QString		_usr;
	QSqlTableModel*	_dataModel;

	QString	_oldTableName;
	QString	_filterString;

	QVector<int> _refreshList;
	QMap<QString, QString> _filterMap;
};

#endif // DBTABLEVIEW_H
