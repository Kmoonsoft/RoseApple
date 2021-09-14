//-----------------------------------------------------------------------------
// �����̸�: SqlQuery.cpp
// ������: Lucky Kim
// ��������: 2008.10.30
// ��    ��: 1.00
// ��    ��: �����ͺ��̽� ������� ����ϴ� Ŭ����
//-----------------------------------------------------------------------------
#include <qsqlerror.h>
#include <qstringlist.h>
#include <qsqlresult.h>
#include <qvariant.h>
#include <qsqlrecord.h>
#include <QtDebug>

#include "SqlQuery.h"

// ������
SqlQuery::SqlQuery(QObject* parent) : QObject(parent)
{
	//���� ����
}

// �Ҹ���
SqlQuery::~SqlQuery()
{
	// Ȥ�� �� Ʈ����ǿ� ���� �ѹ�ó���Ѵ�.
	database().rollback();

	// �����ͺ��̽��� �����Ѵ�.
	QSqlDatabase::removeDatabase(_connection);
}

// �����ͺ��̽� ���� ������ �����Ѵ�.
// �Ķ����: driver�� ����̹�, dbname�� DB��, host�� ȣ��Ʈ��(�ּ�), port�� ��Ʈ��ȣ, user�� ����ڸ�, password�� ��ȣ
void SqlQuery::setConnection(const QString& driver, const QString& dbname, const QString& host, int port, const QString& user, const QString& password)
{
	static int seqNo = 0;
	_connection = driver + "_" + dbname + ":" + QString::number(seqNo++);
	_driver = driver;

	if (!QSqlDatabase::contains(_connection)) {
		QSqlDatabase db = QSqlDatabase::addDatabase(driver, _connection); // �����ͺ��̽� ���ؼ� ���
		db.setDatabaseName(dbname);	// �����ͺ��̽� �̸�
		if(!host.isEmpty()) db.setHostName(host);		// �����ͺ��̽� ȣ��Ʈ�̸�
		if(port > 0 ) db.setPort(port);			// �����ͺ��̽� ��Ʈ
		if(!user.isEmpty()) db.setUserName(user);		// �����ͺ��̽� ���Ӱ���
		if(!password.isEmpty()) db.setPassword(password);	// �����ͺ��̽� ���Ӿ�ȣ
	}
}

void SqlQuery::setFileName(const QString fileName )
{
	_fileName = fileName;
}

// �����ͺ��̽� ���� ���θ� ��ȯ�Ѵ�.
// ����: ����Ǿ� ������ true, �׷��� ������ false ��ȯ.
bool SqlQuery::isOpen()
{
	QSqlQuery q(QSqlDatabase::database(_connection));
	return _driver == "QOCI" ? q.exec("select 0 from dual") : q.exec("select 0");
}

// �����ͺ��̽��� �����Ѵ�.
// ����: �����ͺ��̽� ���ῡ �����ϸ� true, �׷��� ������ false�� ��ȯ.
bool SqlQuery::open()
{
	// �����ͺ��̽��� ��ȿ���� ���� Ȯ���Ѵ�.
	if (!QSqlDatabase::isDriverAvailable(_driver)) {
		return false;
	}

	// �����ͺ��̽� ���ῡ �����ϸ� false�� ��ȯ�ϰ� �����Ѵ�.
	QSqlDatabase db = QSqlDatabase::database(_connection);
	if (!db.open()) {
		qDebug() << db.lastError();
		return false;
	}

	return true;
}

// �����ͺ��̽� ������ ���´�.
void SqlQuery::close()
{
	QSqlDatabase::database(_connection, false).close();
}

// �����ͺ��̽��� ��ȯ�Ѵ�.
QSqlDatabase SqlQuery::database()
{
	QSqlDatabase db = QSqlDatabase::database(_connection);
	if (!isOpen()) {
		bool opened = db.open();
	}
	return db;
}

// �����ͺ��̽��� �˻��Ͽ� ����� ��ȯ�Ѵ�.
// �Ķ����:  sql�� SQL��
// ����: ���� ��ü�� ��ȯ�Ѵ�.
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

// �����ͺ��̽��� �˻��Ͽ� ����� ��ȯ�Ѵ�.
// �Ķ����:  sql�� SQL��, bindValue�� ���ε��� ��
// ����: ���� ��ü�� ��ȯ�Ѵ�.
QSqlQuery SqlQuery::select(const QString& sql, const QString& bindValue)
{
	QSqlQuery q(database());
	q.setForwardOnly(true);

	q.prepare(sql);
	// �����͸� SQL�� ���ε��Ѵ�.
	q.addBindValue(bindValue);

	if (!q.exec()) {
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		qDebug() << bindValue;
	}

	return q;
}

// �����ͺ��̽��� �˻��Ͽ� ����� ��ȯ�Ѵ�.
// �Ķ����:  sql�� SQL��, bindValues�� ���ε��� �� ����Ʈ
// ����: ���� ��ü�� ��ȯ�Ѵ�.
QSqlQuery SqlQuery::select(const QString& sql, const QStringList& bindValues)
{
	QSqlQuery q(database());
	q.setForwardOnly(true);

	q.prepare(sql);
	// �����͸� SQL�� ���ε��Ѵ�.
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

// select�� ������ update, insert, delete SQL�� �����Ѵ�.
// �Ķ����: sqls�� SQL��
// ����: �����ϸ� true, �׷��� ������ false
bool SqlQuery::query(const QString& sql, bool autoTransaction)
{
	return query(QStringList(sql), autoTransaction);
}

// select�� ������ update, insert, delete SQL�� �����Ѵ�.
// �Ķ����: sqls�� SQL��
// ����: �����ϸ� true, �׷��� ������ false
bool SqlQuery::query(const QStringList& sqls, bool autoTransaction)
{
	QSqlDatabase db = database();
	// Ʈ������� �Ǵ�.
	if( autoTransaction) db.transaction();

	QSqlQuery q(db);
	q.setForwardOnly(true);

	// SQL�� ������ŭ �ݺ��Ͽ� ������ �����Ѵ�.
	// ���� �������� ������ ���� �ѹ��ϰ� ����� ��ȯ�Ѵ�.
	for (int i = 0, count = sqls.count(); i < count; i++) {
		if (!q.exec(sqls[i])) {
			if( autoTransaction) db.rollback();
			qDebug() << q.lastQuery();
			qDebug() << q.lastError();
			return false;
		}
	}

	// Ŀ���Ѵ�.
	if( autoTransaction) db.commit();
	return true;
}

// select�� ������ update, insert, delete SQL�� �����Ѵ�.
// �Ķ����: sql(s)�� SQL��, bindValues�� SQL�� ���ε��� ������
// ����: �����ϸ� true, �׷��� ������ false
bool SqlQuery::query(const QString& sql, const QString& bindValue, bool autoTransaction)
{
	QSqlDatabase db = database();
	// Ʈ������� �Ǵ�.
	if( autoTransaction) db.transaction();
	
	QSqlQuery q(db);
	q.setForwardOnly(true);

	// SQL���� �����Ѵ�.
	q.prepare(sql);
	q.addBindValue(bindValue);

	// �������� ������ ���� �ѹ��ϰ� ����� ��ȯ�Ѵ�.
	if (!q.exec()) {
		if( autoTransaction) db.rollback();
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		qDebug() << bindValue;
		return false;
	}

	// Ŀ���Ѵ�.
	if( autoTransaction) db.commit();
	return true;
}

// select�� ������ update, insert, delete SQL�� �����Ѵ�.
// �Ķ����: sql(s)�� SQL��, bindValues�� SQL�� ���ε��� ������
// ����: �����ϸ� true, �׷��� ������ false
bool SqlQuery::query(const QString& sql, const QStringList& bindValues, bool autoTransaction)
{
	QSqlDatabase db = database();
	// Ʈ������� �Ǵ�.
	if( autoTransaction) db.transaction();

	QSqlQuery q(db);
	q.setForwardOnly(true);

	// SQL���� �����Ѵ�.
	q.prepare(sql);

	// �����͸� SQL�� ���ε��Ѵ�.
	for (int i = 0, count = bindValues.count(); i < count; i++) {
		q.addBindValue(bindValues[i]);
	}
		// �������� ������ ���� �ѹ��ϰ� ����� ��ȯ�Ѵ�.
	if (!q.exec()) {
		if( autoTransaction) db.rollback();
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		qDebug() << bindValues.join(",");
		return false;
	}

	// Ŀ���Ѵ�.
	if( autoTransaction) db.commit();
	return true;
}

// select�� ������ update, insert, delete SQL�� �����Ѵ�.
// �Ķ����: sql(s)�� SQL��, other�� SQL���� ���̵��� �����Ͱ� ����ִ� �ٸ� ������ü
// ����: �����ϸ� true, �׷��� ������ false
bool SqlQuery::query(const QString& sql, QSqlQuery& other, bool autoTransaction)
{
	QSqlDatabase db = database();
	// Ʈ������� �Ǵ�.
	if( autoTransaction) db.transaction();

	QSqlQuery q(db);
	q.setForwardOnly(true);

	// other�� �����Ͱ� ������ �׳� ������.
	if (!other.next()) return true;

	// SQL���� �����Ѵ�.
	q.prepare(sql);

	// �����͸� SQL�� ���ε��Ѵ�.
	do {
		for (int i = 0, count = other.record().count(); i < count; i++) {
			q.addBindValue(other.value(i));
		}

		// �������� ������ ���� �ѹ��ϰ� ����� ��ȯ�Ѵ�.
		if (!q.exec()) {
			if( autoTransaction) db.rollback();
			qDebug() << q.lastQuery();
			qDebug() << q.lastError();
			return false;
		}
	} while (other.next());

	// Ŀ���Ѵ�.
	if( autoTransaction) db.commit();
	return true;
}

// select�� ������ update, insert, delete SQL�� �����Ѵ�.
// �Ķ����: sql�� SQL��, bindValues�� SQL�� ���ε��� ������, autoTransaction�� �ڵ�Ʈ����� ���� ����
// ����: �����ϸ� true, �׷��� ������ false
bool SqlQuery::query(const QString& sql, const QVariantList& bindValues, bool autoTransaction)
{
	QSqlDatabase db = database();

	// Ʈ������� �Ǵ�.
	if (autoTransaction) db.transaction();
	QSqlQuery q(db);

	// SQL���� �����Ѵ�.
	q.prepare(sql);

	// �����͸� SQL�� ���ε��Ѵ�.
	for (int i = 0, count = bindValues.count(); i < count; i++) {
		q.addBindValue(bindValues[i]);
	}

	// �������� ������ ���� �ѹ��ϰ� ����� ��ȯ�Ѵ�.
	if (!q.exec()) {
		if (autoTransaction) db.rollback();
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		qDebug() << bindValues;
		return false;
	}

	// Ŀ���Ѵ�.
	if (autoTransaction) db.commit();
	return true;
}

// batch query�� �����Ѵ�.
// �Ķ����: sql(s)�� SQL��, bindValues�� SQL�� ���ε��� ������ QVariantList ����Ʈ
// ����: �����ϸ� true, �׷��� ������ false
bool SqlQuery::batchQuery(const QString& sql, const QList<QVariantList>& bindValues, bool autoTransaction)
{
	QSqlDatabase db = database();

	// Ʈ������� �Ǵ�.
	if( autoTransaction) db.transaction();

	QSqlQuery q(db);
	q.setForwardOnly(true);

	// SQL���� �����Ѵ�.
	q.prepare(sql);

	// �����͸� SQL�� ���ε��Ѵ�.
	for (int i = 0, count = bindValues.count(); i < count; i++) {
		q.addBindValue(bindValues[i]);
	}
		// �������� ������ ���� �ѹ��ϰ� ����� ��ȯ�Ѵ�.
	if (!q.execBatch()) {
		if( autoTransaction) db.rollback();
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		return false;
	}

	// Ŀ���Ѵ�.
	if( autoTransaction) db.commit();
	return true;
}

// �����ͺ��̽� transaction�� �����Ѵ�.
// ����: �����ϸ� true, �׷��� ������ false
bool SqlQuery::transaction()
{
	// Ʈ������� �Ǵ�.
	return database().transaction();
}

// �����ͺ��̽� commit�� �����Ѵ�.
// ����: �����ϸ� true, �׷��� ������ false
bool SqlQuery::commit()
{
	// Ʈ������� �Ǵ�.
	return database().commit();
}

// �����ͺ��̽� rollback�� �����Ѵ�.
// ����: �����ϸ� true, �׷��� ������ false
bool SqlQuery::rollback()
{
	// Ʈ������� �Ǵ�.
	return database().rollback();
}

// �ҽ�DB�� ���� �����͸� �����Ѵ�.
// �Ķ����: srcDB�� �ҽ�DB, tables�� ������ ���̺� ���, conditional�� ���ǹ�
//			bindValues�� ���ǿ� �߰������� bind�� ���� ���, autoTransaction�� �ڵ� Ʈ������-�ѹ� ó�� ����
bool SqlQuery::copy(QSqlDatabase& srcDB, const QStringList& tables, const QString& conditional, const QStringList& bindValues, bool autoTransaction)
{
	QSqlDatabase db = database();
	// Ʈ������� �Ǵ�.
	if( autoTransaction) db.transaction();
	
	QSqlQuery q(db);

	QString whereStr = conditional.trimmed().isEmpty() ?  "" : "where " + conditional;

	for (int i = 0, icount = tables.count(); i < icount; i++) {
		q.prepare(QString("delete from %1 %2").arg(tables[i]).arg(whereStr));
		foreach( QString bindValue, bindValues ) q.addBindValue(bindValue);

		// ���� �����͸� �����Ѵ�. ���� ������ ���� �ѹ��ϰ� �����Ѵ�.
		if (!q.exec()) {
			if( autoTransaction) db.rollback();
			qDebug() << q.lastQuery();
			qDebug() << q.lastError();
			return false;
		}

		// �ҽ�DB���� �����Ϸ��� �����͸� ��ȸ�Ѵ�.
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
		
		// SQL���� ���� �ʵ��� placeholder�� ���ڿ��� �����.
		QSqlRecord srcRecord = srcQuery.record();
		QStringList fnames, placeholders;
		for (int j = 0, jcount = srcRecord.count(); j < jcount; j++) {
			fnames << srcRecord.fieldName(j);
			placeholders << "?";
		}
		
		// SQL���� �����Ѵ�.
		q.prepare(QString("insert into %1 (%2) values (%3)").arg(tables[i]).arg(fnames.join(",")).arg(placeholders.join(",")));

		while (srcQuery.next()) {	
			QStringList boundValues;
			// SQL���� �����͸� ���ε��Ѵ�.
			for (int j = 0, jcount = srcRecord.count(); j < jcount; j++) {
				q.addBindValue(srcQuery.value(j));
				boundValues << srcQuery.value(j).toString();
			}

			// �ҽ�DB�� �����͸� Ÿ��DB�� insert�Ѵ�.
			if (!q.exec()) {
				if( autoTransaction) db.rollback();
				qDebug() << q.lastQuery();
				qDebug() << q.lastError();
				qDebug() << boundValues.join(", ");
				return false;
			}
		}
	}

	// Ŀ���Ѵ�.
	if( autoTransaction) db.commit();
	return true;
}

// update or insert ������
bool SqlQuery::replace( const QString& tableName, const QStringHash& keys, const QStringHash& values, bool autoTransaction )
{
	QStringList keyFields = keys.keys();	// Ű �ʵ��
	QStringList keyValues = keys.values();	// Ű ��

	QStringList datas = values.values();	// Ű�� ������ ������ �ʵ��
	QStringList fields = values.keys();		// Ű�� ������ ������ ��

	QStringList placeholders;		// Ű�� ������ ������ �ʵ� ���� ��ŭ ? ����
	QStringList updates;			// ���� ��� ������Ʈ�� �ʵ��=? �� ����
	for( int i = 0, count = datas.count(); i < count; i++ )
	{
		placeholders << "?";
		updates << fields[i] + "=?";
	}

	QStringList keyPlaceholders;	// Ű ������ŭ ?�� ����
	QStringList keyAndValues;		// Ű�ʵ�=�� ����
	for( int i = 0, count = keyValues.count(); i < count; i++ )
	{
		keyPlaceholders << "?";
		keyAndValues << keyFields[i] + "=?";
	}

	QString allFieldNames = keyFields.join(",") + "," + fields.join(",");	// Ű�� ������ ��ü �ʵ���
	QString allPlaceholderNames = keyPlaceholders.join(",") + "," + placeholders.join(",");	// Ű�� ������ ?��

	// merge ����
	QString sql = QString("merge into %1 using dual on (%2) when matched then update set %3 when not matched then insert (%4) values (%5)")		
		.arg(tableName).arg(keyAndValues.join(",")).arg(updates.join(",")).arg(allFieldNames).arg(allPlaceholderNames);
	
	QStringList bindValues;
	bindValues << keyValues << datas << keyValues << datas;

	QSqlDatabase db = database();
	// Ʈ������� �Ǵ�.
	if( autoTransaction) db.transaction();

	QSqlQuery q(db);
	q.setForwardOnly(true);

	// SQL���� �����Ѵ�.
	q.prepare(sql);

	// �����͸� SQL�� ���ε��Ѵ�.
	for (int i = 0, count = bindValues.count(); i < count; i++) {
		q.addBindValue(bindValues[i]);
	}

	// �������� ������ ���� �ѹ��ϰ� ����� ��ȯ�Ѵ�.
	if (!q.exec()) {
		if( autoTransaction) db.rollback();
		qDebug() << q.lastQuery();
		qDebug() << q.lastError();
		return false;
	}

	// Ŀ���Ѵ�.
	if( autoTransaction) db.commit();
	return true;
}
