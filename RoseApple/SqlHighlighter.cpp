#include "SqlHighlighter.h"

SqlHighlighter::~SqlHighlighter()
{

}

SqlHighlighter::SqlHighlighter(QTextDocument *parent)
: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	QTextCharFormat keywordFormat;// 키워드를 이용한 하일라이트 설정
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat quotationFormat;// 쌍따옴표를 이용한 하일라이트 설정

	keywordFormat.setForeground(Qt::blue);
	keywordFormat.setFontWeight(QFont::Bold);

	QString keywords = "\\b("
		"ABSOLUTE|"
		"ACTION|"
		"ADA|"
		"ADD|"
		"ALL|"
		"ALLOCATE|"
		"ALTER|"
		"AND|"
		"ANY|"
		"ARE|"
		"AS|"
		"ASC|"
		"ASSERTION|"
		"AT|"
		"AUTHORIZATION|"
		"AVG|"
		"BEGIN|"
		"BETWEEN|"
		"BIT_LENGTH|"
		"BOTH|"
		"BY|"
		"CASCADE|"
		"CASCADED|"
		"CASE|"
		"CAST|"
		"CATALOG|"
		"CHAR_LENGTH|"
		"CHARACTER|"
		"CHARACTER_LENGTH|"
		"CHECK|"
		"CLOSE|"
		"COALESCE|"
		"COLLATE|"
		"COLLATION|"
		"COLUMN|"
		"COMMIT|"
		"CONNECT|"
		"CONNECTION|"
		"CONSTRAINT|"
		"CONSTRAINTS|"
		"CONTINUE|"
		"CONVERT|"
		"CORRESPONDING|"
		"COUNT|"
		"CREATE|"
		"CROSS|"
		"CURRENT|"
		"CURRENT_DATE|"
		"CURRENT_TIME|"
		"CURRENT_TIMESTAMP|"
		"CURRENT_USER|"
		"CURSOR|"
		"DEALLOCATE|"
		"DECLARE|"
		"DEFAULT|"
		"DEFERRABLE|"
		"DEFERRED|"
		"DELETE|"
		"DESC|"
		"DESCRIBE|"
		"DESCRIPTOR|"
		"DIAGNOSTICS|"
		"DISCONNECT|"
		"DISTINCT|"
		"DOMAIN|"
		"DROP|"
		"ELSE|"
		"END|"
		"END-EXEC|"
		"ESCAPE|"
		"EXCEPT|"
		"EXCEPTION|"
		"EXEC|"
		"EXECUTE|"
		"EXISTS|"
		"EXTERNAL|"
		"EXTRACT|"
		"FETCH|"
		"FIELD|"
		"FIRST|"
		"FOR|"
		"FOREIGN|"
		"FORTRAN|"
		"FOUND|"
		"FROM|"
		"FULL|"
		"GET|"
		"GLOBAL|"
		"GO|"
		"GOTO|"
		"GRANT|"
		"GROUP|"
		"HAVING|"
		"IDENTITY|"
		"IMMEDIATE|"
		"IN|"
		"INCLUDE|"
		"INDEX|"
		"INDICATOR|"
		"INITIALLY|"
		"INNER|"
		"INPUT|"
		"INSENSITIVE|"
		"INSERT|"
		"INTERSECT|"
		"INTO|"
		"IS|"
		"ISOLATION|"
		"JOIN|"
		"KEY|"
		"LANGUAGE|"
		"LAST|"
		"LEADING|"
		"LEFT|"
		"LEVEL|"
		"LIKE|"
		"LOCAL|"
		"LOWER|"
		"MATCH|"
		"MAX|"
		"MIN|"
		"MINUTE|"
		"MODULE|"
		"NAMES|"
		"NATIONAL|"
		"NATURAL|"
		"NEXT|"
		"NO|"
		"NONE|"
		"NOT|"
		"NULL|"
		"NULLIF|"
		"OCTET_LENGTH|"
		"ON|"
		"ONLY|"
		"OPEN|"
		"OPTION|"
		"OR|"
		"ORDER|"
		"OUTER|"
		"OUTPUT|"
		"OVERLAPS|"
		"PAD|"
		"PARTIAL|"
		"PASCAL|"
		"POSITION|"
		"PRECISION|"
		"PREPARE|"
		"PRESERVE|"
		"PRIMARY|"
		"PRIOR|"
		"PRIVILEGES|"
		"PROCEDURE|"
		"PUBLIC|"
		"READ|"
		"REFERENCES|"
		"RELATIVE|"
		"RESTRICT|"
		"REVOKE|"
		"RIGHT|"
		"ROLLBACK|"
		"ROWS|"
		"SCHEMA|"
		"SCROLL|"
		"SECOND|"
		"SECTION|"
		"SELECT|"
		"SESSION|"
		"SESSION_USER|"
		"SET|"
		"SIZE|"
		"SOME|"
		"SPACE|"
		"SQL|"
		"SQLCA|"
		"SQLCODE|"
		"SQLERROR|"
		"SQLSTATE|"
		"SQLWARNING|"
		"SUBSTRING|"
		"SUM|"
		"SYSTEM_USER|"
		"TABLE|"
		"TEMPORARY|"
		"THEN|"
		"TIMEZONE_HOUR|"
		"TIMEZONE_MINUTE|"
		"TO|"
		"TRAILING|"
		"TRANSACTION|"
		"TRANSLATE|"
		"TRANSLATION|"
		"TRIM|"
		"UNION|"
		"UNIQUE|"
		"UNKNOWN|"
		"UPDATE|"
		"UPPER|"
		"USAGE|"
		"USER|"
		"USING|"
		"VALUE|"
		"VALUES|"
		"VARYING|"
		"VIEW|"
		"WHEN|"
		"WHENEVER|"
		"WHERE|"
		"WITH|"
		"WORK|"
		"WRITE|"
		"ZONE|"
		"FALSE|"
		"TRUE)\\b";

	QString dataType = "\\b("
		"BIT|BOOL(EAN)?|"
		"TEXT|N?CHAR|CHARACTER|N?VARCHAR[2]?|"
		"INTE(GER|RVAL)|INT[0-9]*|(BIG|TINY|SMALL|LARGE)INT|WORD|LONG|SINGLE|"
		"NUMERIC|NUMBER|"
		"TIME(STAMP)?|DATE(TIME)?|"
		"HOUR|MINUTE|SECOND|"
		"YEAR|MONTH|DAY|"
		"DEC(IMAL)?|FLOAT|DOUBLE|REAL|"
		"VARYING|"
		"N?[BC]LOB|"	// nclob, clob, blob
		"SERIAL|"
		"BYTE[A]?|BINARY|BFILE|XML|"
		"RAW|ROWID|UROWID|"
		"MONEY|CURRENCY|MEMO|WIDEMEMO|GRAPHIC|POINT|POLYGON|IMAGE)\\b";

	rule.pattern = QRegExp(keywords, Qt::CaseInsensitive);
	rule.format = keywordFormat;
	highlightingRules.append(rule);

	keywordFormat.setForeground(Qt::black);
	rule.pattern = QRegExp(dataType, Qt::CaseInsensitive);
	rule.format = keywordFormat;
	highlightingRules.append(rule);

	singleLineCommentFormat.setForeground(Qt::gray);
	singleLineCommentFormat.setFontItalic(true);
	rule.pattern = QRegExp("--[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	quotationFormat.setForeground(Qt::magenta);//"abc", 'abc' 형태
	rule.pattern = QRegExp("\"[^\"]*\"|\'[^\']*\'");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	quotationFormat.setForeground( QColor(0x00CC00) );
	rule.pattern = QRegExp("\\b[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?\\b");	// 숫자, 123, -123.34, 1E6, -1E6, +123.45, .34, -.34
	rule.format = quotationFormat;
	highlightingRules.append(rule);
}

void 
SqlHighlighter::highlightBlock( const QString &text )
{
	foreach(HighlightingRule rule, highlightingRules) 
	{
		QRegExp expression(rule.pattern);
		int index = text.indexOf(expression);
		while( index >= 0 ) 
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = text.indexOf(expression, index + length);
		}
	}
	setCurrentBlockState(0);
}
