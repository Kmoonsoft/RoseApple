//-----------------------------------------------------------------------------
// 파일이름: TableSelectWidget.h
// 만든사람: Lucky Kim(neoeeff@gmail.com)
// 만든일자: 2010.08.23
// 버    전: 1.00
// 설    명: 데이터베이스 테이블 선택 위젯
//-----------------------------------------------------------------------------
#ifndef TABLESELECTWIDGET_H
#define TABLESELECTWIDGET_H

#include <QDialog>
#include "ui_TableSelectWidget.h"

class TableSelectWidgetData;
class QSqlDatabase;

class TableSelectWidget : public QDialog
{
	Q_OBJECT

public:
	enum ODBCType { ACCESS=0, EXCEL, KNOWN };

	TableSelectWidget(QSqlDatabase& db, QWidget *parent = 0, Qt::WindowFlags f = 0, ODBCType type=ACCESS );
	~TableSelectWidget();

	void setODBCTyp0e( ODBCType type );
	QString selectedTable();
	QStringList tables();

private:
	Ui::TableSelectWidgetClass ui;

	TableSelectWidgetData*	_d;

private slots:
	void on_cancel_clicked();
	void on_ok_clicked();
};

#endif // TABLESELECTWIDGET_H
