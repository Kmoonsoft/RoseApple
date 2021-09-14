/******************************************************************************
** 파일명 : DataImportWidget.h
** 버전   : 1.0
** 갱신일 : 2011/08/23
** 만든이 : Lucky Kim(neoeeff@gmail.com)
** 설  명 : 엑셀로 부터 데이터베이스 Import 위젯
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
