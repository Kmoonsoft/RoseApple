/******************************************************************************
** 파일명 : FilterDialog.h
** 버전   : 1.0
** 갱신일 : 2011/08/23
** 만든이 : Lucky Kim(neoeeff@gmail.com)
** 설  명 : 데이터베이스 테이블 필터 위젯
******************************************************************************/
#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include "ui_FilterDialog.h"

class FilterDialog : public QDialog
{
	Q_OBJECT

public:
	FilterDialog(const QString& filter, QWidget *parent = 0, Qt::WindowFlags f = 0);
	~FilterDialog();

	QString getFilter();

private:
	Ui::FilterDialogClass ui;

private slots:
	void on_cancelBtn_clicked();
	void on_okBtn_clicked();
};

#endif // FILTERDIALOG_H
