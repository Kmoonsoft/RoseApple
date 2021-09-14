//-----------------------------------------------------------------------------
// �����̸�: SqlQuery.h
// ������: Lucky Kim(neoeeff@gmail.com)
// ��������: 2010.08.23
// ��    ��: 1.00
// ��    ��: �����ͺ��̽� ������� ����ϴ� Ŭ����
//-----------------------------------------------------------------------------
#pragma once

#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qmutex.h>
#include <qvariant.h>
#include <qstringlist.h>

// ���ڿ� key, ���ڿ� value�� Hash�� �����Ѵ�.
typedef QHash<QString,QString> QStringHash;

// QStringHash ����Ʈ�� �����Ѵ�.
typedef QList<QStringHash> QStringHashList;


class QStringList;

//-----------------------------------------------------------------------------
// SqlQuery Ŭ����
//-----------------------------------------------------------------------------
class SqlQuery : public QObject
{
	Q_OBJECT

public:

	// ������
	SqlQuery(QObject* parent);

	// �Ҹ���
	virtual ~SqlQuery();

	// �����ͺ��̽� ���� ������ �����Ѵ�.
	// �Ķ����: driver�� ����̹�, dbname�� DB��, host�� ȣ��Ʈ��(�ּ�), port�� ��Ʈ��ȣ, user�� ����ڸ�, password�� ��ȣ
	void		setConnection(const QString& driver, const QString& dbname,
		const QString& host = QString::null, int port = 0, const QString& user = QString::null, const QString& password = QString::null);

	void		setFileName(const QString fileName );

	// �����ͺ��̽� ���� ���θ� ��ȯ�Ѵ�.
	// ����: ����Ǿ� ������ true, �׷��� ������ false ��ȯ.
	bool		isOpen();

	// �����ͺ��̽��� �����Ѵ�.
	// ����: �����ͺ��̽� ���ῡ �����ϸ� true, �׷��� ������ false�� ��ȯ.
	bool		open();

	// �����ͺ��̽� ������ ���� �ٽ� �����Ѵ�.
	bool		reopen();

	// �����ͺ��̽� ������ ���´�.
	void		close();
	
	// �����ͺ��̽��� ��ȯ�Ѵ�.
	QSqlDatabase	database();

	// �����ͺ��̽��� �˻��Ͽ� ����� ��ȯ�Ѵ�.
	// �Ķ����: sql�� SQL��
	// ����: ���� ��ü�� ��ȯ�Ѵ�.
	QSqlQuery	select(const QString& sql);

	// �����ͺ��̽��� �˻��Ͽ� ����� ��ȯ�Ѵ�.
	// �Ķ����:  sql�� SQL��, bindValue�� ���ε��� ��
	// ����: ���� ��ü�� ��ȯ�Ѵ�.
	QSqlQuery	select(const QString& sql, const QString& bindValue);

	// �����ͺ��̽��� �˻��Ͽ� ����� ��ȯ�Ѵ�.
	// �Ķ����:  sql�� SQL��, bindValues�� ���ε��� �� ����Ʈ
	// ����: ���� ��ü�� ��ȯ�Ѵ�.
	QSqlQuery	select(const QString& sql, const QStringList& bindValues);
	
	// select�� ������ update, insert, delete SQL�� �����Ѵ�.
	// �Ķ����: sql(s)�� SQL��
	// ����: �����ϸ� true, �׷��� ������ false
	bool		query(const QString& sql, bool autoTransaction=true);
	bool		query(const QStringList& sqls, bool autoTransaction=true);
	bool		query(const QString& sql, const QString& bindValue, bool autoTransaction=true);
	bool		query(const QString& sql, const QStringList& bindValues, bool autoTransaction=true);
	// select�� ������ update, insert, delete SQL�� �����Ѵ�.
	// �Ķ����: sql(s)�� SQL��, other�� SQL���� ���̵��� �����Ͱ� ����ִ� �ٸ� ������ü
	// ����: �����ϸ� true, �׷��� ������ false
	bool		query(const QString& sql, QSqlQuery& other, bool autoTransaction=true);

	// select�� ������ update, insert, delete SQL�� �����Ѵ�.
	// �Ķ����: sql�� SQL��, bindValues�� SQL�� ���ε��� ������, autoTransaction�� �ڵ�Ʈ����� ���� ����
	// ����: �����ϸ� true, �׷��� ������ false
	bool		query(const QString& sql, const QVariantList& bindValues, bool autoTransaction);

	// batch query�� �����Ѵ�.
	// �Ķ����: sql(s)�� SQL��, bindValues�� SQL�� ���ε��� ������ QVariantList ����Ʈ
	// ����: �����ϸ� true, �׷��� ������ false
	bool		batchQuery(const QString& sql, const QList<QVariantList>& bindValues, bool autoTransaction);

	// �����ͺ��̽� transaction�� �����Ѵ�.
	// _autoTransaction�� true�̸� ������� �ʴ´�.
	// ����: �����ϸ� true, �׷��� ������ false
	bool		transaction();

	// �����ͺ��̽� commit�� �����Ѵ�.
	// _autoTransaction�� true�̸� ������� �ʴ´�.
	// ����: �����ϸ� true, �׷��� ������ false
	bool		commit();

	// �����ͺ��̽� rollback�� �����Ѵ�.
	// _autoTransaction�� true�̸� ������� �ʴ´�.
	// ����: �����ϸ� true, �׷��� ������ false
	bool		rollback();

	// �ҽ�DB�� ���� �����͸� �����Ͽ� �����Ѵ�.
	// �Ķ����: srcDB�� �ҽ�DB, tables�� ������ ���̺� ���, conditional�� ���ǹ�
	bool		copy(QSqlDatabase& srcDB, const QStringList& tables, const QString& conditional="", const QStringList& bindValues=QStringList(), bool autoTransaction=true);

	bool		replace( const QString& tableName, const QStringHash& keys, const QStringHash& values, bool autoTransaction=true );

private:

	// �����ͺ��̽� ���ؼ� �̸�
	QString	_connection;

	// �����ͺ��̽� ����̺��
	QString _driver;

	// db file
	QString _fileName;
};
