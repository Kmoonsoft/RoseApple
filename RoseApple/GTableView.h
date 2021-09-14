/******************************************************************************
** 파일명 : GTableView.h
** 버전   : 1.0
** 갱신일 : 2011/08/23
** 만든이 : Lukcy Kim(neoeeff@gmail.com)
** 설  명 : 데이터베이스 테이블 보기 위젯
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
