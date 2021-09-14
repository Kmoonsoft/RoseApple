/******************************************************************************
** ���ϸ� : DataImportWidget.h
** ����   : 1.0
** ������ : 2011/08/23
** ������ : Lucky Kim(neoeeff@gmail.com)
** ��  �� : ������ ���� �����ͺ��̽� Import ����
******************************************************************************/
#ifndef DATAIMPORTWIDGET_H
#define DATAIMPORTWIDGET_H

#include <QWidget>
#include "ui_DataImportWidget.h"

class DataImportWidgetData;
class SqlQuery;

class DataImportWidget : public QWidget
{
	Q_OBJECT

public:
	DataImportWidget(QWidget *parent = 0, SqlQuery* q = 0);
	~DataImportWidget();

public slots:
	void setTable();

private:
	Ui::DataImportWidgetClass ui;
	DataImportWidgetData*	_d;

private slots:
	void on_deleteAll_clicked();
	void on_cancel_clicked();
	void on_get_clicked();
	void on_file_2_clicked();
	void tableChanged();
};

#endif // DATAIMPORTWIDGET_H
