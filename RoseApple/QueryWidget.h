/******************************************************************************
** ���ϸ� : QueryWidget.h
** ����   : 1.0
** ������ : 2011/08/23
** ������ : Lukcy Kim(neoeeff@gmail.com)
** ��  �� : ���� �� ��� ���� ����
******************************************************************************/
#ifndef QUERYWIDGET_H
#define QUERYWIDGET_H

#include <QWidget>
#include "ui_QueryWidget.h"

class QueryWidgetData;
class QSqlDatabase;

class QueryWidget : public QWidget
{
	Q_OBJECT

public:
	QueryWidget(const QSqlDatabase& db, QWidget *parent = 0);
	~QueryWidget();

private:
	Ui::QueryWidgetClass ui;
	QueryWidgetData*	_d;

protected:
	void	keyReleaseEvent ( QKeyEvent * event );

private slots:
	void on_lineExeBtn_clicked();
	void on_selExeBtn_clicked();
	void on_exeBtn_clicked();
	void setResultLog(int msec);
};

#endif // QUERYWIDGET_H
