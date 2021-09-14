//-----------------------------------------------------------------------------
// 파일이름: SqlQuery.cpp
// 만든사람: Lucky Kim
// 만든일자: 2008.10.30
// 버    전: 1.00
// 설    명: 데이터베이스 입출력을 담당하는 클래스
//-----------------------------------------------------------------------------
#include <qsqlerror.h>
#include <qstringlist.h>
#include <qsqlresult.h>
#include <qvariant.h>
#include <qsqlrecord.h>
#include <QtDebug>

#include "SqlQuery.h"

// 생성자
SqlQuery::SqlQuery(QObject* parent) : QObject(parent)
{
	//내용 없음
}

// 소멸자
SqlQuery::~SqlQuery()
{
	// 혹시 모를 트랜잭션에 대해 롤백처리한다.
	database().rollback();

	// 데이터베이스를 제거한다.
	QSqlDatabase::removeDatabase(_connection);
}

// 데이터베이스 연결 정보를 설정한다.
// 파라미터: driver는 드라이버, dbname은 DB명, host는 호스트명(주소), port는 포트번호, user는 사용자명, password는 암호
void SqlQuery::setConnection(const QString& driver, const QString& dbname, const QString& host, int port, const QString& user, const QString& password)
{
	static int seqNo = 0;
	_connection = driver + "_" + dbname + ":" + QString::number(seqNo++);
	_driver = driver;

	if (!QSqlDatabase::contains(_connection)) {
		QSqlDatabase db = QSqlDatabase::addDatabase(driver, _connection); // 데이터베이스 컨넥션 등록
		db.setDatabaseName(dbname);	// 데이터베이스 이름
		if(!host.isEmpty()) db.setHostName(host);		// 데이터베이스 호스트이름
		if(port > 0 ) db.setPort(port);			// 데이터베이스 포트
		if(!user.isEmpty()) db.setUserName(user);		// 데이터베이스 접속계정
		if(!password.isEmpty()) db.setPassword(password);	// 데이터베이스 접속암호
	}
}

void SqlQuery::setFileName(const QString fileName )
{
	_fileName = fileName;
}

// 데이터베이스 연결 여부를 반환한다.
// 리턴: 연결되어 있으면 true, 그렇지 않으면 false 반환.
bool SqlQuery::isOpen()
{
	QSqlQuery q(QSqlDatabase::database(_connection));
	return _driver == "QOCI" ? q.exec("select 0 from dual") : q.exec("select 0");
}

// 데이터베이스에 연결한다.
// 리턴: 데이터베이스 연결에 성공하면 true, 그렇지 않으면 false를 반환.
bool SqlQuery::open()
{
	// 데이터베이스가 유효한지 먼지 확인한다.
	if (!QSqlDatabase::isDriverAvailable(_driver)) {
		return false;
	}

	// 데이터베이스 연결에 실패하면 false를 반환하고 종료한다.
	QSqlDatabase db = QSqlDatabase::database(_connection);
	if (!db.open()) {
		qDebug() << db.lastError();
		return false;
	}

	return true;
}

// 데이터베이스 연결을 끊는다.
void SqlQuery::close()
{
	QSqlDatabase::database(_connection, false).close();
}

// 데이터베이스를 반환한다.
QSqlDatabase SqlQuery::database()
{
	QSqlDatabase db = QSqlDatabase::database(_connection);
	if (!isOpen()) {
		bool opened = db.open();
	}
	return db;
}

// 데이터베이스를 검색하여 결과를 반환한다.
// 파라미터:  sql은 SQL문
// 리턴: 쿼리 객체를 반환한다.
QSqlQuery SqlQuery::select(const QString& sql)
{
	QSqlQuery q(database());
	q.setForwardOnly(true);

	if (!q.exec(sql)) {
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
	}

	return q;
}

// 데이터베이스를 검색하여 결과를 반환한다.
// 파라미터:  sql은 SQL문, bindValue는 바인딩할 값
// 리턴: 쿼리 객체를 반환한다.
QSqlQuery SqlQuery::select(const QString& sql, const QString& bindValue)
{
	QSqlQuery q(database());
	q.setForwardOnly(true);

	q.prepare(sql);
	// 데이터를 SQL에 바인드한다.
	q.addBindValue(bindValue);

	if (!q.exec()) {
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		qDebug() << bindValue;
	}

	return q;
}

// 데이터베이스를 검색하여 결과를 반환한다.
// 파라미터:  sql은 SQL문, bindValues는 바인딩할 값 리스트
// 리턴: 쿼리 객체를 반환한다.
QSqlQuery SqlQuery::select(const QString& sql, const QStringList& bindValues)
{
	QSqlQuery q(database());
	q.setForwardOnly(true);

	q.prepare(sql);
	// 데이터를 SQL에 바인드한다.
	for (int i = 0, count = bindValues.count(); i < count; i++) {
		q.addBindValue(bindValues[i]);
	}

	if (!q.exec()) {
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		qDebug() << bindValues.join(",");
	}

	return q;
}

// select를 제외한 update, insert, delete SQL을 실행한다.
// 파라미터: sqls은 SQL문
// 리턴: 성공하면 true, 그렇지 않으면 false
bool SqlQuery::query(const QString& sql, bool autoTransaction)
{
	return query(QStringList(sql), autoTransaction);
}

// select를 제외한 update, insert, delete SQL을 실행한다.
// 파라미터: sqls은 SQL문
// 리턴: 성공하면 true, 그렇지 않으면 false
bool SqlQuery::query(const QStringList& sqls, bool autoTransaction)
{
	QSqlDatabase db = database();
	// 트랜잭션을 건다.
	if( autoTransaction) db.transaction();

	QSqlQuery q(db);
	q.setForwardOnly(true);

	// SQL문 갯수만큼 반복하여 쿼리를 실행한다.
	// 만약 쿼리에서 오류가 나면 롤백하고 결과를 반환한다.
	for (int i = 0, count = sqls.count(); i < count; i++) {
		if (!q.exec(sqls[i])) {
			if( autoTransaction) db.rollback();
			qDebug() << q.lastQuery();
			qDebug() << q.lastError();
			return false;
		}
	}

	// 커밋한다.
	if( autoTransaction) db.commit();
	return true;
}

// select를 제외한 update, insert, delete SQL을 실행한다.
// 파라미터: sql(s)은 SQL문, bindValues은 SQL에 바인드할 데이터
// 리턴: 성공하면 true, 그렇지 않으면 false
bool SqlQuery::query(const QString& sql, const QString& bindValue, bool autoTransaction)
{
	QSqlDatabase db = database();
	// 트랜잭션을 건다.
	if( autoTransaction) db.transaction();
	
	QSqlQuery q(db);
	q.setForwardOnly(true);

	// SQL문을 지정한다.
	q.prepare(sql);
	q.addBindValue(bindValue);

	// 쿼리에서 오류가 나면 롤백하고 결과를 반환한다.
	if (!q.exec()) {
		if( autoTransaction) db.rollback();
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		qDebug() << bindValue;
		return false;
	}

	// 커밋한다.
	if( autoTransaction) db.commit();
	return true;
}

// select를 제외한 update, insert, delete SQL을 실행한다.
// 파라미터: sql(s)은 SQL문, bindValues은 SQL에 바인드할 데이터
// 리턴: 성공하면 true, 그렇지 않으면 false
bool SqlQuery::query(const QString& sql, const QStringList& bindValues, bool autoTransaction)
{
	QSqlDatabase db = database();
	// 트랜잭션을 건다.
	if( autoTransaction) db.transaction();

	QSqlQuery q(db);
	q.setForwardOnly(true);

	// SQL문을 지정한다.
	q.prepare(sql);

	// 데이터를 SQL에 바인드한다.
	for (int i = 0, count = bindValues.count(); i < count; i++) {
		q.addBindValue(bindValues[i]);
	}
		// 쿼리에서 오류가 나면 롤백하고 결과를 반환한다.
	if (!q.exec()) {
		if( autoTransaction) db.rollback();
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		qDebug() << bindValues.join(",");
		return false;
	}

	// 커밋한다.
	if( autoTransaction) db.commit();
	return true;
}

// select를 제외한 update, insert, delete SQL을 실행한다.
// 파라미터: sql(s)은 SQL문, other는 SQL문에 바이드할 데이터가 들어있는 다른 쿼리객체
// 리턴: 성공하면 true, 그렇지 않으면 false
bool SqlQuery::query(const QString& sql, QSqlQuery& other, bool autoTransaction)
{
	QSqlDatabase db = database();
	// 트랜잭션을 건다.
	if( autoTransaction) db.transaction();

	QSqlQuery q(db);
	q.setForwardOnly(true);

	// other에 데이터가 없으면 그냥 나간다.
	if (!other.next()) return true;

	// SQL문을 지정한다.
	q.prepare(sql);

	// 데이터를 SQL에 바인드한다.
	do {
		for (int i = 0, count = other.record().count(); i < count; i++) {
			q.addBindValue(other.value(i));
		}

		// 쿼리에서 오류가 나면 롤백하고 결과를 반환한다.
		if (!q.exec()) {
			if( autoTransaction) db.rollback();
			qDebug() << q.lastQuery();
			qDebug() << q.lastError();
			return false;
		}
	} while (other.next());

	// 커밋한다.
	if( autoTransaction) db.commit();
	return true;
}

// select를 제외한 update, insert, delete SQL을 실행한다.
// 파라미터: sql은 SQL문, bindValues은 SQL에 바인드할 데이터, autoTransaction은 자동트랜잭션 설정 여부
// 리턴: 성공하면 true, 그렇지 않으면 false
bool SqlQuery::query(const QString& sql, const QVariantList& bindValues, bool autoTransaction)
{
	QSqlDatabase db = database();

	// 트랜잭션을 건다.
	if (autoTransaction) db.transaction();
	QSqlQuery q(db);

	// SQL문을 지정한다.
	q.prepare(sql);

	// 데이터를 SQL에 바인드한다.
	for (int i = 0, count = bindValues.count(); i < count; i++) {
		q.addBindValue(bindValues[i]);
	}

	// 쿼리에서 오류가 나면 롤백하고 결과를 반환한다.
	if (!q.exec()) {
		if (autoTransaction) db.rollback();
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		qDebug() << bindValues;
		return false;
	}

	// 커밋한다.
	if (autoTransaction) db.commit();
	return true;
}

// batch query를 실행한다.
// 파라미터: sql(s)은 SQL문, bindValues은 SQL에 바인드할 데이터 QVariantList 리스트
// 리턴: 성공하면 true, 그렇지 않으면 false
bool SqlQuery::batchQuery(const QString& sql, const QList<QVariantList>& bindValues, bool autoTransaction)
{
	QSqlDatabase db = database();

	// 트랜잭션을 건다.
	if( autoTransaction) db.transaction();

	QSqlQuery q(db);
	q.setForwardOnly(true);

	// SQL문을 지정한다.
	q.prepare(sql);

	// 데이터를 SQL에 바인드한다.
	for (int i = 0, count = bindValues.count(); i < count; i++) {
		q.addBindValue(bindValues[i]);
	}
		// 쿼리에서 오류가 나면 롤백하고 결과를 반환한다.
	if (!q.execBatch()) {
		if( autoTransaction) db.rollback();
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		return false;
	}

	// 커밋한다.
	if( autoTransaction) db.commit();
	return true;
}

// 데이터베이스 transaction을 수행한다.
// 리턴: 성공하면 true, 그렇지 않으면 false
bool SqlQuery::transaction()
{
	// 트랜잭션을 건다.
	return database().transaction();
}

// 데이터베이스 commit을 수행한다.
// 리턴: 성공하면 true, 그렇지 않으면 false
bool SqlQuery::commit()
{
	// 트랜잭션을 건다.
	return database().commit();
}

// 데이터베이스 rollback을 수행한다.
// 리턴: 성공하면 true, 그렇지 않으면 false
bool SqlQuery::rollback()
{
	// 트랜잭션을 건다.
	return database().rollback();
}

// 소스DB로 부터 데이터를 복사한다.
// 파라미터: srcDB는 소스DB, tables는 복사할 테이블 목록, conditional은 조건문
//			bindValues는 조건에 추가적으로 bind할 값의 목록, autoTransaction은 자동 트랜젝션-롤백 처리 여부
bool SqlQuery::copy(QSqlDatabase& srcDB, const QStringList& tables, const QString& conditional, const QStringList& bindValues, bool autoTransaction)
{
	QSqlDatabase db = database();
	// 트랜잭션을 건다.
	if( autoTransaction) db.transaction();
	
	QSqlQuery q(db);

	QString whereStr = conditional.trimmed().isEmpty() ?  "" : "where " + conditional;

	for (int i = 0, icount = tables.count(); i < icount; i++) {
		q.prepare(QString("delete from %1 %2").arg(tables[i]).arg(whereStr));
		foreach( QString bindValue, bindValues ) q.addBindValue(bindValue);

		// 먼저 데이터를 삭제한다. 만약 오류가 나면 롤백하고 종료한다.
		if (!q.exec()) {
			if( autoTransaction) db.rollback();
			qDebug() << q.lastQuery();
			qDebug() << q.lastError();
			return false;
		}

		// 소스DB에서 복사하려는 데이터를 조회한다.
		QSqlQuery srcQuery(srcDB);
		srcQuery.setForwardOnly(true);
		srcQuery.prepare(QString("select * from %1 %2").arg(tables[i]).arg(whereStr));
		foreach( QString bindValue, bindValues ) srcQuery.addBindValue(bindValue);

		if (!srcQuery.exec()) {
			if( autoTransaction) db.rollback();
			qDebug() << q.lastQuery();
			qDebug() << q.lastError();
			return false;
		}
		
		// SQL문에 넣을 필드명과 placeholder를 문자열로 만든다.
		QSqlRecord srcRecord = srcQuery.record();
		QStringList fnames, placeholders;
		for (int j = 0, jcount = srcRecord.count(); j < jcount; j++) {
			fnames << srcRecord.fieldName(j);
			placeholders << "?";
		}
		
		// SQL문을 지정한다.
		q.prepare(QString("insert into %1 (%2) values (%3)").arg(tables[i]).arg(fnames.join(",")).arg(placeholders.join(",")));

		while (srcQuery.next()) {	
			QStringList boundValues;
			// SQL문에 데이터를 바인드한다.
			for (int j = 0, jcount = srcRecord.count(); j < jcount; j++) {
				q.addBindValue(srcQuery.value(j));
				boundValues << srcQuery.value(j).toString();
			}

			// 소스DB의 데이터를 타켓DB에 insert한다.
			if (!q.exec()) {
				if( autoTransaction) db.rollback();
				qDebug() << q.lastQuery();
				qDebug() << q.lastError();
				qDebug() << boundValues.join(", ");
				return false;
			}
		}
	}

	// 커밋한다.
	if( autoTransaction) db.commit();
	return true;
}

// update or insert 쿼리문
bool SqlQuery::replace( const QString& tableName, const QStringHash& keys, const QStringHash& values, bool autoTransaction )
{
	QStringList keyFields = keys.keys();	// 키 필드명
	QStringList keyValues = keys.values();	// 키 값

	QStringList datas = values.values();	// 키를 제외한 설정할 필드명
	QStringList fields = values.keys();		// 키를 제외한 설정할 값

	QStringList placeholders;		// 키를 제외한 설정할 필드 개수 만큼 ? 생성
	QStringList updates;			// 있을 경우 업데이트용 필드명=? 를 생성
	for( int i = 0, count = datas.count(); i < count; i++ )
	{
		placeholders << "?";
		updates << fields[i] + "=?";
	}

	QStringList keyPlaceholders;	// 키 개수만큼 ?를 생성
	QStringList keyAndValues;		// 키필드=값 형태
	for( int i = 0, count = keyValues.count(); i < count; i++ )
	{
		keyPlaceholders << "?";
		keyAndValues << keyFields[i] + "=?";
	}

	QString allFieldNames = keyFields.join(",") + "," + fields.join(",");	// 키를 포함한 전체 필드명들
	QString allPlaceholderNames = keyPlaceholders.join(",") + "," + placeholders.join(",");	// 키를 포함한 ?들

	// merge 문장
	QString sql = QString("merge into %1 using dual on (%2) when matched then update set %3 when not matched then insert (%4) values (%5)")		
		.arg(tableName).arg(keyAndValues.join(",")).arg(updates.join(",")).arg(allFieldNames).arg(allPlaceholderNames);
	
	QStringList bindValues;
	bindValues << keyValues << datas << keyValues << datas;

	QSqlDatabase db = database();
	// 트랜잭션을 건다.
	if( autoTransaction) db.transaction();

	QSqlQuery q(db);
	q.setForwardOnly(true);

	// SQL문을 지정한다.
	q.prepare(sql);

	// 데이터를 SQL에 바인드한다.
	for (int i = 0, count = bindValues.count(); i < count; i++) {
		q.addBindValue(bindValues[i]);
	}

	// 쿼리에서 오류가 나면 롤백하고 결과를 반환한다.
	if (!q.exec()) {
		if( autoTransaction) db.rollback();
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		return false;
	}

	// 커밋한다.
	if( autoTransaction) db.commit();
	return true;
}
