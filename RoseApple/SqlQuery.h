//-----------------------------------------------------------------------------
// 파일이름: SqlQuery.h
// 만든사람: Lucky Kim(neoeeff@gmail.com)
// 만든일자: 2010.08.23
// 버    전: 1.00
// 설    명: 데이터베이스 입출력을 담당하는 클래스
//-----------------------------------------------------------------------------
#pragma once

#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qmutex.h>
#include <qvariant.h>
#include <qstringlist.h>

// 문자열 key, 문자열 value로 Hash를 정의한다.
typedef QHash<QString,QString> QStringHash;

// QStringHash 리스트를 정의한다.
typedef QList<QStringHash> QStringHashList;


class QStringList;

//-----------------------------------------------------------------------------
// SqlQuery 클래스
//-----------------------------------------------------------------------------
class SqlQuery : public QObject
{
	Q_OBJECT

public:

	// 생성자
	SqlQuery(QObject* parent);

	// 소멸자
	virtual ~SqlQuery();

	// 데이터베이스 연결 정보를 설정한다.
	// 파라미터: driver는 드라이버, dbname은 DB명, host는 호스트명(주소), port는 포트번호, user는 사용자명, password는 암호
	void		setConnection(const QString& driver, const QString& dbname,
		const QString& host = QString::null, int port = 0, const QString& user = QString::null, const QString& password = QString::null);

	void		setFileName(const QString fileName );

	// 데이터베이스 연결 여부를 반환한다.
	// 리턴: 연결되어 있으면 true, 그렇지 않으면 false 반환.
	bool		isOpen();

	// 데이터베이스에 연결한다.
	// 리턴: 데이터베이스 연결에 성공하면 true, 그렇지 않으면 false를 반환.
	bool		open();

	// 데이터베이스 연결을 끊고 다시 연결한다.
	bool		reopen();

	// 데이터베이스 연결을 끊는다.
	void		close();
	
	// 데이터베이스를 반환한다.
	QSqlDatabase	database();

	// 데이터베이스를 검색하여 결과를 반환한다.
	// 파라미터: sql은 SQL문
	// 리턴: 쿼리 객체를 반환한다.
	QSqlQuery	select(const QString& sql);

	// 데이터베이스를 검색하여 결과를 반환한다.
	// 파라미터:  sql은 SQL문, bindValue는 바인딩할 값
	// 리턴: 쿼리 객체를 반환한다.
	QSqlQuery	select(const QString& sql, const QString& bindValue);

	// 데이터베이스를 검색하여 결과를 반환한다.
	// 파라미터:  sql은 SQL문, bindValues는 바인딩할 값 리스트
	// 리턴: 쿼리 객체를 반환한다.
	QSqlQuery	select(const QString& sql, const QStringList& bindValues);
	
	// select를 제외한 update, insert, delete SQL을 실행한다.
	// 파라미터: sql(s)는 SQL문
	// 리턴: 성공하면 true, 그렇지 않으면 false
	bool		query(const QString& sql, bool autoTransaction=true);
	bool		query(const QStringList& sqls, bool autoTransaction=true);
	bool		query(const QString& sql, const QString& bindValue, bool autoTransaction=true);
	bool		query(const QString& sql, const QStringList& bindValues, bool autoTransaction=true);
	// select를 제외한 update, insert, delete SQL을 실행한다.
	// 파라미터: sql(s)은 SQL문, other는 SQL문에 바이드할 데이터가 들어있는 다른 쿼리객체
	// 리턴: 성공하면 true, 그렇지 않으면 false
	bool		query(const QString& sql, QSqlQuery& other, bool autoTransaction=true);

	// select를 제외한 update, insert, delete SQL을 실행한다.
	// 파라미터: sql은 SQL문, bindValues은 SQL에 바인드할 데이터, autoTransaction은 자동트랜잭션 설정 여부
	// 리턴: 성공하면 true, 그렇지 않으면 false
	bool		query(const QString& sql, const QVariantList& bindValues, bool autoTransaction);

	// batch query를 실행한다.
	// 파라미터: sql(s)은 SQL문, bindValues은 SQL에 바인드할 데이터 QVariantList 리스트
	// 리턴: 성공하면 true, 그렇지 않으면 false
	bool		batchQuery(const QString& sql, const QList<QVariantList>& bindValues, bool autoTransaction);

	// 데이터베이스 transaction을 수행한다.
	// _autoTransaction이 true이면 제어되지 않는다.
	// 리턴: 성공하면 true, 그렇지 않으면 false
	bool		transaction();

	// 데이터베이스 commit을 수행한다.
	// _autoTransaction이 true이면 제어되지 않는다.
	// 리턴: 성공하면 true, 그렇지 않으면 false
	bool		commit();

	// 데이터베이스 rollback을 수행한다.
	// _autoTransaction이 true이면 제어되지 않는다.
	// 리턴: 성공하면 true, 그렇지 않으면 false
	bool		rollback();

	// 소스DB로 부터 데이터를 복사하여 저장한다.
	// 파라미터: srcDB는 소스DB, tables는 복사할 테이블 목록, conditional은 조건문
	bool		copy(QSqlDatabase& srcDB, const QStringList& tables, const QString& conditional="", const QStringList& bindValues=QStringList(), bool autoTransaction=true);

	bool		replace( const QString& tableName, const QStringHash& keys, const QStringHash& values, bool autoTransaction=true );

private:

	// 데이터베이스 컨넥션 이름
	QString	_connection;

	// 데이터베이스 드라이브명
	QString _driver;

	// db file
	QString _fileName;
};
