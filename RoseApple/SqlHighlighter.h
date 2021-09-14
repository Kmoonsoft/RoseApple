/******************************************************************************
** 파일명 : SqlHighlighter.h
** 버전   : 1.0
** 갱신일 : 2011/08/23
** 만든이 : Lucky Kim(neoeeff@gmail.com)
** 설  명 : 텍스트 하이라이트 표시
******************************************************************************/
#ifndef SqlHighlighter_H
#define SqlHighlighter_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

class QTextDocument;

/**
** \file SqlHighlighter.h
**
** \brief SQL Query 하이라이트 표시 클래스
**
** QSyntaxHighlighter를 상속
*/
class SqlHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
    SqlHighlighter(QTextDocument *parent);
    ~SqlHighlighter();

protected:

	void highlightBlock(const QString &text);

private:

	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};

	QVector<HighlightingRule> highlightingRules;
};

#endif // SqlHighlighter_H
