/******************************************************************************
** 파일명 : RoseApple.h
** 버전   : 1.0
** 갱신일 : 2011/08/23
** 만든이 : Lukcy Kim(neoeeff@gmail.com)
** 설  명 : 메인 윈도우
******************************************************************************/
#ifndef ROSEAPPLE_H
#define ROSEAPPLE_H

#include <QMainWindow>
#include "ui_RoseApple.h"

class RoseAppleData;

class RoseApple : public QMainWindow
{
	Q_OBJECT

public:
	RoseApple(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~RoseApple();

private:
	Ui::RoseAppleClass ui;
	RoseAppleData*	_d;

private slots:
	void on_actionQuery_triggered();
	void on_actionCSTextManager_triggered();
	void on_actionDataImpot_triggered();
	void on_actionDBTableView_triggered();
	void on_actionConnectDB_triggered();
	void on_actionSubwindow_View_triggered();
	void on_actionClose_All_triggered();
	void on_actionTileView_triggered();
	void on_actionCascadeView_triggered();
	void on_actionTappedView_triggered();
	void on_actionSQL_Maker_triggered();
	void on_actionQuit_triggered();
};

#endif // ROSEAPPLE_H
