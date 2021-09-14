//-----------------------------------------------------------------------------
// 파일이름: UiControl.h
// 만든사람: Lucky Kim(neoeeff@gmail.com)
// 만든일자: 2008.7.10
// 버    전: 1.0
// 설    명: UI의 공통 컨트롤러를 모아 둔 클래스
//			 staic 함수로 대부분 구성된다.
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
#define	MSG_WARNING_T(title, msg)			QMessageBox::warning(this, title, msg, tr("확인"))
#define	MSG_CRITICAL(msg)					QMessageBox::critical(this, windowTitle(), msg, tr("&Ok"))
#define	MSG_CRITICAL_T(title, msg)			QMessageBox::critical(this, title, msg, tr("&Ok"))

class UiControl
{
public:

	//-----------------------------------------------------------------------------
	// QSplitter 제어
	//-----------------------------------------------------------------------------

	// QSplitter Widget의 균등분할 한다.
	// 입력 : w 대상 스플리터 a, b, c는 분할 비율
	static void InitSplitter( QSplitter* w );// 균등분할
	static void InitSplitter( QSplitter* w, int a, int b );// 2개 위젯 분할
	static void InitSplitter( QSplitter* w, int a, int b, int c ); // 3개 위젯 분할

	//-----------------------------------------------------------------------------
	// QTableView 제어
	//-----------------------------------------------------------------------------

	// QTableView Widget을 초기화한다. 정해진 규격으로 초기화
	// 입력 : w는 대상 테이블 뷰
	static void	InitTable( QTableView* w );
	static void	InitTable2( QTableView* w );
	static void	InitTable3( QTableView* w );
	static void	InitTable4( QTableView* w );
	
	// 오브젝트(parent)가 가진 자식 오브젝트가 QLineEdit나 QTextEdit이면 내용을 클리어 시킨다.
	//	입력 : parent 부보 오브젝트
	static void ClearEdit(const QObject* parent);

};

//-----------------------------------------------------------------------------
//	Double Validator 구현
//	QDoubleValidator가 버그로 인한 재구현임
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
//	특정 Validator를 가지는 아이템 Delegate 구현
//-----------------------------------------------------------------------------
#include <QItemDelegate>
class ItemValidationDelegate : public QItemDelegate
{
	Q_OBJECT

public:

	// 생성자
	// 생성시 사용할 Validator를 설정해 준다.
	ItemValidationDelegate(QValidator* v, QObject* parent = 0);
	~ItemValidationDelegate();

	// 모델의 에디터 위젯을 생성하여 반환한다.
	QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

private:
	QValidator*	_v;
};

//-----------------------------------------------------------------------------
//	특정 입력 Mask를 가지는 아이템 Delegate 구현
//-----------------------------------------------------------------------------
class ItemMaskDelegate : public QItemDelegate
{
	Q_OBJECT

public:

	// 생성자
	// 생성시 사용할 Validator를 설정해 준다.
	ItemMaskDelegate(const QString& mask, QObject* parent = 0);
	~ItemMaskDelegate();

	// 모델의 에디터 위젯을 생성하여 반환한다.
	QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

private:
	QString		_mask;
};

//-----------------------------------------------------------------------------
//	콤보박스(리스트 type)를 가지는 아이템 Delegate 구현
//-----------------------------------------------------------------------------
class ComboBoxItemDelegate : public QItemDelegate
{
	Q_OBJECT

public:

	// 생성자
	// 생성시 사용할 Validator를 설정해 준다.
	ComboBoxItemDelegate(const QStringList& list = QStringList(), QObject* parent = 0);
	~ComboBoxItemDelegate();

	// 모델의 에디터 위젯을 생성하여 반환한다.
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
//	체크된 스트링의 앞에 아이콘을 표시하는 모델 Delegate
//-----------------------------------------------------------------------------

class CheckStringListDelegate : public QItemDelegate
{
		Q_OBJECT

public:

	enum CheckStringListDelegateRole { ValueRole = Qt::UserRole + 1000, CheckDisplayRole };

	// 생성자
	CheckStringListDelegate(const QStringList& strings = QStringList(), const QStringList& values = QStringList(), QObject* parent = 0);
	~CheckStringListDelegate();

	// 모델의 에디터 위젯을 생성하여 반환한다.
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
//	체크박스 + 라인에디스 위젯
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
//	체크박스 Delegate
//-----------------------------------------------------------------------------
class CheckBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:

	// 생성자
	// 생성시 사용할 Validator를 설정해 준다.
	CheckBoxDelegate(QObject* parent = 0);
	~CheckBoxDelegate();

	// 모델의 에디터 위젯을 생성하여 반환한다.
	QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

	void setModelData ( QWidget * w, QAbstractItemModel * model, const QModelIndex & index ) const;

	void	setIcon(const QPixmap& icon);

private :
	QPixmap		_icon;

};

#endif // UICONTROL_H
