/******************************************************************************
** 파일명 : SqlMakerWidget.h
** 버전   : 1.0
** 갱신일 : 2011/08/23
** 만든이 : Lucky Kim(neoeeff@gmail.com)
** 설  명 : Sql 쿼리문 만들기 위젯
******************************************************************************/
#ifndef SQLMAKERWIDGET_H
#define SQLMAKERWIDGET_H

#include <QWidget>
#include "ui_SqlMakerWidget.h"

class SqlQuery;
class SqlMakerWidgetData;

class SqlMakerWidget : public QWidget
{
	Q_OBJECT

public:
	SqlMakerWidget(QWidget *parent = 0, SqlQuery* q = 0);
	~SqlMakerWidget();

public slots:
	void setTable();
	void setField();

private slots:
	void on_arrangeBtn_clicked();
	void make();

private:
	QString whereString(const QString& str, const QString& mark="?");
	QString updateString(const QString& str, const QString& mark="?");

private:
	Ui::SqlMakerWidgetClass ui;
	SqlMakerWidgetData*	_d;
};

#endif // SQLMAKERWIDGET_H
