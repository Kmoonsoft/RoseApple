/******************************************************************************
** ���ϸ� : GTableView.h
** ����   : 1.0
** ������ : 2011/08/23
** ������ : Lukcy Kim(neoeeff@gmail.com)
** ��  �� : �����ͺ��̽� ���̺� ���� ����
******************************************************************************/
#ifndef GTABLEVIEW_H
#define GTABLEVIEW_H

#include <QTableView>

class GTableView : public QTableView
{
	Q_OBJECT

public:
	GTableView(QWidget *parent);
	~GTableView();

	void	resizeColumnsToContents();

protected:
	void	contextMenuEvent ( QContextMenuEvent * event );
	void	keyReleaseEvent ( QKeyEvent * event );

private slots:
	void	saveToClip();
	void	saveToExcel();
};

#endif // GTABLEVIEW_H
