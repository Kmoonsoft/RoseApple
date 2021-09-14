//-----------------------------------------------------------------------------
// �����̸�: UiControl.h
// ������: Lucky Kim(neoeeff@gmail.com)
// ��������: 2008.7.10
// ��    ��: 1.0
// ��    ��: UI�� ���� ��Ʈ�ѷ��� ��� �� Ŭ����
//			 staic �Լ��� ��κ� �����ȴ�.
//-----------------------------------------------------------------------------
#ifndef UICONTROL_H
#define UICONTROL_H

#include <QMessageBox>

class QObject;
class QSplitter;
class QTableView;

#define	MSG_INFORMATION(msg)				QMessageBox::information(this, windowTitle(), msg, tr("&Ok"))
#define	MSG_INFORMATION_T(title, msg)		QMessageBox::information(this, title, msg, tr("&Ok"))
#define	MSG_WARNING(msg)					QMessageBox::warning(this, windowTitle(), msg, tr("&Ok"))
#define	MSG_WARNING_T(title, msg)			QMessageBox::warning(this, title, msg, tr("Ȯ��"))
#define	MSG_CRITICAL(msg)					QMessageBox::critical(this, windowTitle(), msg, tr("&Ok"))
#define	MSG_CRITICAL_T(title, msg)			QMessageBox::critical(this, title, msg, tr("&Ok"))

class UiControl
{
public:

	//-----------------------------------------------------------------------------
	// QSplitter ����
	//-----------------------------------------------------------------------------

	// QSplitter Widget�� �յ���� �Ѵ�.
	// �Է� : w ��� ���ø��� a, b, c�� ���� ����
	static void InitSplitter( QSplitter* w );// �յ����
	static void InitSplitter( QSplitter* w, int a, int b );// 2�� ���� ����
	static void InitSplitter( QSplitter* w, int a, int b, int c ); // 3�� ���� ����

	//-----------------------------------------------------------------------------
	// QTableView ����
	//-----------------------------------------------------------------------------

	// QTableView Widget�� �ʱ�ȭ�Ѵ�. ������ �԰����� �ʱ�ȭ
	// �Է� : w�� ��� ���̺� ��
	static void	InitTable( QTableView* w );
	static void	InitTable2( QTableView* w );
	static void	InitTable3( QTableView* w );
	static void	InitTable4( QTableView* w );
	
	// ������Ʈ(parent)�� ���� �ڽ� ������Ʈ�� QLineEdit�� QTextEdit�̸� ������ Ŭ���� ��Ų��.
	//	�Է� : parent �κ� ������Ʈ
	static void ClearEdit(const QObject* parent);

};

//-----------------------------------------------------------------------------
//	Double Validator ����
//	QDoubleValidator�� ���׷� ���� �籸����
//-----------------------------------------------------------------------------
#include <QDoubleValidator>

class DoubleValidator : public QDoubleValidator
{
	Q_OBJECT

public:

    DoubleValidator( QObject * parent);
    DoubleValidator( double bottom, double top, int decimals, QObject * parent );
    ~DoubleValidator();

	QValidator::State validate( QString& input, int& pos ) const;
};

//-----------------------------------------------------------------------------
//	Ư�� Validator�� ������ ������ Delegate ����
//-----------------------------------------------------------------------------
#include <QItemDelegate>
class ItemValidationDelegate : public QItemDelegate
{
	Q_OBJECT

public:

	// ������
	// ������ ����� Validator�� ������ �ش�.
	ItemValidationDelegate(QValidator* v, QObject* parent = 0);
	~ItemValidationDelegate();

	// ���� ������ ������ �����Ͽ� ��ȯ�Ѵ�.
	QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

private:
	QValidator*	_v;
};

//-----------------------------------------------------------------------------
//	Ư�� �Է� Mask�� ������ ������ Delegate ����
//-----------------------------------------------------------------------------
class ItemMaskDelegate : public QItemDelegate
{
	Q_OBJECT

public:

	// ������
	// ������ ����� Validator�� ������ �ش�.
	ItemMaskDelegate(const QString& mask, QObject* parent = 0);
	~ItemMaskDelegate();

	// ���� ������ ������ �����Ͽ� ��ȯ�Ѵ�.
	QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

private:
	QString		_mask;
};

//-----------------------------------------------------------------------------
//	�޺��ڽ�(����Ʈ type)�� ������ ������ Delegate ����
//-----------------------------------------------------------------------------
class ComboBoxItemDelegate : public QItemDelegate
{
	Q_OBJECT

public:

	// ������
	// ������ ����� Validator�� ������ �ش�.
	ComboBoxItemDelegate(const QStringList& list = QStringList(), QObject* parent = 0);
	~ComboBoxItemDelegate();

	// ���� ������ ������ �����Ͽ� ��ȯ�Ѵ�.
	QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	void setModelData ( QWidget * w, QAbstractItemModel * model, const QModelIndex & index ) const;

private:
	QStringList	_list;
};

#include <QStandardItemModel>
#include <QBitArray>

class CheckStringListModel : public QStandardItemModel
{
	Q_OBJECT
public :
	CheckStringListModel(QObject* parent=0);
	CheckStringListModel(const QStringList& strings, QObject* parent=0);

	QVariant data ( const QModelIndex & index, int role ) const;
	bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

	void	resetData(const QStringList& strings);
	void	setIcon(const QPixmap& icon);
	void	setCheck(int row, bool check = true);
	void	setChecks(const QBitArray& checks);
	const QPixmap& icon() const;

private :
	QStringList _l;
	QPixmap		_icon;
	QBitArray	_checks;
};

//-----------------------------------------------------------------------------
//	üũ�� ��Ʈ���� �տ� �������� ǥ���ϴ� �� Delegate
//-----------------------------------------------------------------------------

class CheckStringListDelegate : public QItemDelegate
{
		Q_OBJECT

public:

	enum CheckStringListDelegateRole { ValueRole = Qt::UserRole + 1000, CheckDisplayRole };

	// ������
	CheckStringListDelegate(const QStringList& strings = QStringList(), const QStringList& values = QStringList(), QObject* parent = 0);
	~CheckStringListDelegate();

	// ���� ������ ������ �����Ͽ� ��ȯ�Ѵ�.
	QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	void setModelData ( QWidget * w, QAbstractItemModel * model, const QModelIndex & index ) const;
	
	void	setCheckIcon(const QPixmap& icon);

private slots:
	void	endEdit();

private:
	QStringList _l;		// string list
	QStringList _v;		// value list
	QPixmap		_icon;
};

//-----------------------------------------------------------------------------
//	üũ�ڽ� + ���ο��� ����
//-----------------------------------------------------------------------------
class QCheckBox;
class QLineEdit;
class CheckBoxEdit : public QWidget
{
	Q_OBJECT

public:
	CheckBoxEdit(const QString& checkLabel, const QString& text, QWidget* parent = 0, Qt::WindowFlags f = 0 );
	~CheckBoxEdit();
	bool isChecked();
	void setChecked(bool check);
	QString text();
	void setText(const QString& text);

private :
	QCheckBox*	_c;
	QLineEdit*	_e;
};

//-----------------------------------------------------------------------------
//	üũ�ڽ� Delegate
//-----------------------------------------------------------------------------
class CheckBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:

	// ������
	// ������ ����� Validator�� ������ �ش�.
	CheckBoxDelegate(QObject* parent = 0);
	~CheckBoxDelegate();

	// ���� ������ ������ �����Ͽ� ��ȯ�Ѵ�.
	QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

	void setModelData ( QWidget * w, QAbstractItemModel * model, const QModelIndex & index ) const;

	void	setIcon(const QPixmap& icon);

private :
	QPixmap		_icon;

};

#endif // UICONTROL_H
