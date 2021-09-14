/******************************************************************************
** 파일명 : NewConnectionDlg.h
** 버전   : 1.0
** 갱신일 : 2011/08/23
** 만든이 : Lukcy Kim(neoeeff@gmail.com)
** 설  명 : 데이터베이스 연결 다이얼로그
******************************************************************************/
#ifndef NEWCONNECTIONDLG_H
#define NEWCONNECTIONDLG_H

#include <QDialog>
#include "ui_NewConnectionDlg.h"

class SqlQuery;
class ConnInfo;
class QItemSelection;
class QSqlDatabase;

class NewConnectionDlg : public QDialog
{
	Q_OBJECT

public:
	NewConnectionDlg(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~NewConnectionDlg();

	SqlQuery*	query() const;

	static QString	getLastQuery( const QSqlDatabase& db );
	static bool		setLastQuery( const QSqlDatabase& db, const QString& query );

private:
	bool	deleteInfo(const ConnInfo& info);
	void	addConnInfo(const ConnInfo& info);
	void	load();

private:
	Ui::NewConnectionDlgClass ui;

	SqlQuery*	_query;
	SqlQuery*	_connQuery;
	QString		_lastQry;

private slots:
	void on_tableView_doubleClicked(QModelIndex);
	void on_tableView_customContextMenuRequested(QPoint);
	void on_fileBtn_clicked();
	void on_driveCmbbox_textChanged(const QString &);
	void on_connectBtn_clicked();
	void on_cancelBtn_clicked();

	void selected(const QItemSelection&, const QItemSelection&);
	void clearSelected();
	void clearAll();
};

#endif // NEWCONNECTIONDLG_H
