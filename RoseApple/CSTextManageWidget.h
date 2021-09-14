/******************************************************************************
** ���ϸ� : CSTextManageWidget.h
** ����   : 1.0
** ������ : 2011/08/23
** ������ : Lucky Kim(neoeeff@gmail.com)
******************************************************************************/
#ifndef CSTEXTMANAGEWIDGET_H
#define CSTEXTMANAGEWIDGET_H

#include <QWidget>
#include "ui_CSTextManageWidget.h"

struct CSTextManageWidgetData;

class CSTextManageWidget : public QWidget
{
	Q_OBJECT

public:
	CSTextManageWidget(QWidget *parent = 0);
	~CSTextManageWidget();

private:
	Ui::CSTextManageWidgetClass ui;
	CSTextManageWidgetData*	_d;

private slots:
	void on_fileOpen_clicked();

	void selectTable(const QItemSelection&, const QItemSelection&);
	void distinctChanged(bool checked);
};

#endif // CSTEXTMANAGEWIDGET_H
