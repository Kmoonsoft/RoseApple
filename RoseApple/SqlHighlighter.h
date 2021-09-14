/******************************************************************************
** ���ϸ� : SqlHighlighter.h
** ����   : 1.0
** ������ : 2011/08/23
** ������ : Lucky Kim(neoeeff@gmail.com)
** ��  �� : �ؽ�Ʈ ���̶���Ʈ ǥ��
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
** \brief SQL Query ���̶���Ʈ ǥ�� Ŭ����
**
** QSyntaxHighlighter�� ���
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
