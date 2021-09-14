/******************************************************************************
** ���ϸ� : FilterDialog.h
** ����   : 1.0
** ������ : 2011/08/23
** ������ : Lucky Kim(neoeeff@gmail.com)
** ��  �� : �����ͺ��̽� ���̺� ���� ����
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
