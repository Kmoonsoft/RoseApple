//-----------------------------------------------------------------------------
// 파일이름: UiControl.h
// 만든사람: Lucky Kim
// 만든일자: 2008.6.2
// 버    전: 1.0
// 설    명: Qt UI 위젯을 공통으로 제어하기 위한 클래스
//-----------------------------------------------------------------------------
/* 변경이력: 
			 2008.07.10 Lucky Kim	Qt4.2부터 제공되어 트리아이템 확장 하는 부분 삭제
*/
#include <QtGui>
#include <QtWidgets>
#include "UiControl.h"

// QSplitter Widget의 균등분할 한다.
// 입력 : w 대상 스플리터
void
UiControl::InitSplitter( QSplitter* w )
{
	QList<int> l;
	
	for( int i = 0; i < w->count(); i++ )
	{
		l << 1;// 균등 비율
	}

	w->setSizes(l);
}

// QSplitter Widget의 2개의 분할 비율을 지정한다.
// 입력 : w 대상 스플리터, a와 b는 분할 비율
void
UiControl::InitSplitter( QSplitter* w, int a, int b )
{
	QList<int> l;

	// 비율 설정
	l << a*10000 << b*10000;

	w->setSizes(l);
}

// QSplitter Widget의 3개의 분할 비율을 지정한다.
// 입력 : w는 대상 스플리터,  a, b, c값은 분할 비율이다.
void
UiControl::InitSplitter( QSplitter* w, int a, int b, int c )
{
	QList<int> l;

	// 비율 설정
	l << a*10000 << b*10000 << c*10000;

	w->setSizes(l);
}

// QTableView Widget을 초기화한다. 정해진 규격으로 초기화
// 입력 : w는 대상 테이블 뷰
void
UiControl::InitTable( QTableView* w )
{
	// 수직 해더 설정
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);									// 수직 해더 이동 금지
	v->setHidden(true);										// 수직 해더 숨김
	v->setDefaultSectionSize( 18 );							// 수직해더 높이는 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );				// 텍스트 중앙정렬

	// 수평 해더 설정
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);									// 수평 해더 이동 금지
	r->setSortIndicatorShown(true);							// 소팅 인디케이더 표시

	w->setAlternatingRowColors(true);						// 한줄씩 회색 채움색 지정
	w->setEditTriggers( QAbstractItemView::NoEditTriggers );//에디터 금지

	// 섹션 누름시 소팅 기능 설정
	w->disconnect( r, SIGNAL(sectionPressed(int)), w, SLOT(selectColumn(int)) );
	w->connect( r, SIGNAL(sectionClicked(int)), w, SLOT(sortByColumn(int)) );
}

// QTableView Widget을 초기화한다. 정해진 규격으로 초기화
// 입력 : w는 대상 테이블 뷰
void
UiControl::InitTable2( QTableView* w )
{
	// 수직 해더 설정
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// 수직 해더 이동 금지
	v->setHidden(true);							// 수직 해더 숨김
	v->setDefaultSectionSize( 18 );				// 수직해더 높이는 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// 텍스트 중앙정렬

	// 수평 해더 설정
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// 수평 해더 이동 금지
	r->setSortIndicatorShown(true);				// 소팅 인디케이더 표시

	w->setAlternatingRowColors(true);			// 한줄씩 회색 채움색 지정

	// 섹션 누름시 소팅 기능 설정
	w->disconnect( r, SIGNAL(sectionPressed(int)), w, SLOT(selectColumn(int)) );
	w->connect( r, SIGNAL(sectionClicked(int)), w, SLOT(sortByColumn(int)) );
}

// QTableView Widget을 초기화한다. 정해진 규격으로 초기화
// 입력 : w는 대상 테이블 뷰
void
UiControl::InitTable3( QTableView* w )
{
	// 수직 해더 설정
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// 수직 해더 이동 금지
	v->setHidden(true);							// 수직 해더 숨김
	v->setDefaultSectionSize( 18 );				// 수직해더 높이는 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// 텍스트 중앙정렬

	// 수평 해더 설정
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// 수평 해더 이동 금지
	r->setSortIndicatorShown(false);			// 소팅 인디케이더 숨기기

	w->setAlternatingRowColors(true);			// 한줄씩 회색 채움색 지정
}

// QTableView Widget을 초기화한다. 정해진 규격으로 초기화
// 입력 : w는 대상 테이블 뷰
void
UiControl::InitTable4( QTableView* w )
{
	// 수직 해더 설정
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// 수직 해더 이동 금지
	v->setDefaultSectionSize( 20 );				// 수직해더 높이는 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// 텍스트 중앙정렬
	v->setSectionResizeMode(QHeaderView::Fixed);		// 수직 해더 사이즈 고정

	// 수평 해더 설정
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// 수평 해더 이동 금지
	r->setSortIndicatorShown(false);			// 소팅 인디케이더 숨기기

	w->setAlternatingRowColors(true);			// 한줄씩 회색 채움색 지정
	w->setEditTriggers( QAbstractItemView::NoEditTriggers );//에디터 금지
}

// 오브젝트(parent)가 가진 자식 오브젝트가 QLineEdit나 QTextEdit이면 내용을 클리어 시킨다.
//	입력 : parent 부보 오브젝트
void UiControl::ClearEdit(const QObject* parent)
{
	if( !parent ) return;
	
	const QObjectList& list = parent->children();
	foreach(QObject* o, list) {
		//라인 에디트 클리어
		QLineEdit* e = dynamic_cast<QLineEdit*>(o);
		if( e ) {
			e->clear();
			continue;
		}

		// TextEdit 클리어
		QTextEdit* t  = dynamic_cast<QTextEdit*>(o);
		if( t ) {
			t->clear();
			continue;
		}
		
		if( dynamic_cast<QLabel*>(o)) continue; // 레이블 건너띄기
		if( dynamic_cast<QComboBox*>(o) ) continue;// 콤보박스 건너띄기

		ClearEdit(o);
	}
}

//-----------------------------------------------------------------------------
//	Double Validator 구현
//	QDoubleValidator가 버그로 인한 재구현임
//-----------------------------------------------------------------------------
#include <math.h>

DoubleValidator::DoubleValidator( QObject * parent )
    : QDoubleValidator( parent)
{
}

DoubleValidator::DoubleValidator( double bottom, double top, int decimals, QObject * parent )
    : QDoubleValidator( bottom, top, decimals, parent )
{
}

DoubleValidator::~DoubleValidator()
{
}

QValidator::State DoubleValidator::validate( QString & input, int & ) const
{
    QRegExp empty( QString::fromLatin1(" *-?\\.? *") );
    if ( bottom() >= 0 && input.simplified().startsWith(QString::fromLatin1("-")) )	{
		return Invalid;
	}

    if ( empty.exactMatch(input) )	{
		return Intermediate;
	}

    bool ok = TRUE;
    double entered = input.toDouble( &ok );
	int nume = input.count( 'e', Qt::CaseInsensitive );
    if ( !ok ) {
		// explicit exponent regexp
		QRegExp expexpexp( QString::fromLatin1("[Ee][+-]?\\d*$") );
		int eeePos = expexpexp.indexIn( input );
		if ( eeePos > 0 && nume == 1 ) {
			QString mantissa = input.left( eeePos );
			entered = mantissa.toDouble( &ok );
			if ( !ok )
			return Invalid;
		} 
		else if ( eeePos == 0 ) {
			return Intermediate;
		} 
			
		return Invalid;
    }

    int i = input.indexOf( '.' );
    if ( i >= 0 && nume == 0 ) {
		// has decimal point (but no E), now count digits after that
		i++;
		int j = i;
		while( input[j].isDigit() ) j++;
		if ( j - i > decimals() ) {
			return QValidator::Invalid;
		}
    }

	if ( entered < bottom() || entered > top() ) {
		return QValidator::Invalid;
	}
		
	return Acceptable;
}

//-----------------------------------------------------------------------------
//	특정 Validator를 가지는 아이템 Delegate 구현
//-----------------------------------------------------------------------------
ItemValidationDelegate::ItemValidationDelegate(QValidator* v, QObject* parent ) 
: QItemDelegate( parent ), _v(v)
{
	// do nothing
}

// 파괴자
ItemValidationDelegate::~ItemValidationDelegate()
{
	// do nothing
}

// 모델의 에디터 위젯을 생성하여 반환한다.
QWidget*
ItemValidationDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	QVariant v = index.model()->data( index );

	QLineEdit* e = new QLineEdit( parent );
	e->setFrame(false);
	e->installEventFilter(const_cast<ItemValidationDelegate*>(this));
	
	e->setValidator( _v );
	return e;
}

//-----------------------------------------------------------------------------
//	특정 입력 마스크를 가지는 아이템 Delegate 구현
//-----------------------------------------------------------------------------
ItemMaskDelegate::ItemMaskDelegate(const QString& mask, QObject* parent ) 
: QItemDelegate( parent ), _mask(mask)
{
	// do nothing
}

// 파괴자
ItemMaskDelegate::~ItemMaskDelegate()
{
	// do nothing
}

// 모델의 에디터 위젯을 생성하여 반환한다.
QWidget*
ItemMaskDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	QVariant v = index.model()->data( index );

	QLineEdit* e = new QLineEdit( parent );
	e->setFrame(false);
	e->installEventFilter(const_cast<ItemMaskDelegate*>(this));
	
	e->setInputMask(_mask);
	return e;
}

//-----------------------------------------------------------------------------
//	콤보박스 아이템
//-----------------------------------------------------------------------------
ComboBoxItemDelegate::ComboBoxItemDelegate(const QStringList& list, QObject* parent ) 
: QItemDelegate( parent ), _list(list)
{
	// do nothing
}

// 파괴자
ComboBoxItemDelegate::~ComboBoxItemDelegate()
{
	// do nothing
}

// 모델의 에디터 위젯을 생성하여 반환한다.
QWidget*
ComboBoxItemDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	QVariant v = index.model()->data( index, Qt::UserRole );

	QComboBox* e = new QComboBox( parent );
	e->addItems(_list);
	
	e->installEventFilter(const_cast<ComboBoxItemDelegate*>(this));
	
	e->setCurrentIndex(v.toInt());
	return e;
}

void ComboBoxItemDelegate::setModelData ( QWidget * w, QAbstractItemModel * model, const QModelIndex & index ) const
{
	QComboBox* cbox = dynamic_cast<QComboBox*>(w);
	if( !w ) return;

	model->setData(index, cbox->currentText());
	model->setData(index, cbox->currentIndex(), Qt::UserRole);
}

//-----------------------------------------------------------------------------
//	체크된 스트링의 앞에 아이콘을 표시하는 모델
//-----------------------------------------------------------------------------
CheckStringListModel::CheckStringListModel(QObject* parent)
: QStandardItemModel(parent)
{
}

CheckStringListModel::CheckStringListModel(const QStringList& strings, QObject* parent)
: QStandardItemModel(parent)
{
	resetData(strings);
}

QVariant CheckStringListModel::data( const QModelIndex & index, int role ) const
{
	if( _l.count() <= index.row() ) return QVariant();

	if( role == Qt::DecorationRole && _checks.count() > index.row() && _checks[index.row()]) return _icon;
	else return QStandardItemModel::data(index, role);
}

bool CheckStringListModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
	if( _l.count() <= index.row() ) return false;
	if( role == Qt::EditRole || role == Qt::DisplayRole ) _l[index.row()] = value.toString();
	
	return QStandardItemModel::setData(index, value, role);
}

void CheckStringListModel::resetData(const QStringList& strings)
{
	_l.clear();
	_l = strings;
	_checks.clear();
	clear();

	setColumnCount(1);
	int cnt = strings.count();
	_checks = QBitArray(cnt);
	setRowCount(cnt);

	for( int r = 0; r < cnt; r++ ) {
		QModelIndex i = index(r, 0);
		setData(i, strings[r]);
	}
}

// 아이콘을 설정한다.
void CheckStringListModel::setIcon(const QPixmap& icon)
{
	_icon = icon;
}

const QPixmap& CheckStringListModel::icon() const
{
	return _icon;
}

void CheckStringListModel::setCheck(int row, bool check)
{
	if( row >= _checks.size() ) return;
	_checks[row] = check;
}

void CheckStringListModel::setChecks(const QBitArray& checks)
{
	_checks = checks;
}

//-----------------------------------------------------------------------------
//	체크된 스트링의 앞에 아이콘을 표시하는 모델 Delegate
//-----------------------------------------------------------------------------
CheckStringListDelegate::CheckStringListDelegate(const QStringList& list, const QStringList& values, QObject* parent ) 
: QItemDelegate( parent ), _l(list), _v(values)
{
	
}

// 파괴자
CheckStringListDelegate::~CheckStringListDelegate()
{
	
}

// 모델의 에디터 위젯을 생성하여 반환한다.
QWidget*
CheckStringListDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	QComboBox* e = new QComboBox( parent );
	CheckStringListModel* m = new CheckStringListModel(_l, e);
	m->setIcon(_icon);
	QBitArray checks = index.data(CheckDisplayRole).toBitArray();
	if( checks.count() ) m->setChecks( checks );
	e->setModel(m);

	QString text = index.data().toString();
	int current = e->findText(text);
	if( current >= 0 ) e->setCurrentIndex(current);

	e->installEventFilter(const_cast<CheckStringListDelegate*>(this));
	
	connect(e, SIGNAL(currentIndexChanged(int)), SLOT(endEdit()) );
	return e;
}

void CheckStringListDelegate::setModelData ( QWidget * w, QAbstractItemModel * model, const QModelIndex & index ) const
{
	QComboBox* cbox = dynamic_cast<QComboBox*>(w);
	CheckStringListModel* m = (CheckStringListModel*)(cbox->model());

	if( !w ) return;

	int idx = cbox->currentIndex();
	model->setData(index, cbox->currentText());
	if( idx < _v.count() ) model->setData(index, _v[idx], Qt::UserRole);
	else model->setData(index, QVariant(), Qt::UserRole);

	QBitArray checks = index.data(CheckDisplayRole).toBitArray();
	if(checks.count() > cbox->currentIndex() && checks[cbox->currentIndex()] ) model->setData(index, _icon, Qt::DecorationRole);
	else model->setData(index, QVariant(), Qt::DecorationRole);
}

void CheckStringListDelegate::setCheckIcon(const QPixmap& icon)
{
	_icon = icon;
}

void	CheckStringListDelegate::endEdit()
{
	QWidget* w = dynamic_cast<QWidget*>( sender());
	if( w ) w->clearFocus();
}

//-----------------------------------------------------------------------------
//	체크박스 + 라인에디스 위젯
//-----------------------------------------------------------------------------
CheckBoxEdit::CheckBoxEdit(const QString& checkLabel, const QString& text, QWidget* parent, Qt::WindowFlags f )
:QWidget(parent, f)
{
	_c = new QCheckBox(checkLabel, this);
	_e = new QLineEdit(text, this);
	QHBoxLayout* l = new QHBoxLayout(this);
	l->setSpacing(0);
	l->setMargin(0);
	l->addWidget(_c);
	l->addWidget(_e);
	setLayout(l);
	_c->show();
	_e->show();
	setAutoFillBackground(true);
}

CheckBoxEdit::~CheckBoxEdit()
{
}

bool CheckBoxEdit::isChecked()
{
	return _c->isChecked();
}

QString CheckBoxEdit::text()
{
	return _e->text();
}

void CheckBoxEdit::setChecked(bool check)
{
	_c->setChecked(check);
}

void CheckBoxEdit::setText(const QString& text)
{
	_e->setText(text);
}

//-----------------------------------------------------------------------------
//	체크박스 Delegate
//-----------------------------------------------------------------------------
// 생성자
// 생성시 사용할 Validator를 설정해 준다.
CheckBoxDelegate::CheckBoxDelegate(QObject* parent)
	: QItemDelegate( parent )
{
	_icon = QPixmap(":/MDMg/Resources/convert.png");
}

CheckBoxDelegate::~CheckBoxDelegate(){}

// 모델의 에디터 위젯을 생성하여 반환한다.
QWidget* CheckBoxDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	QVariant v = index.model()->data( index );

	CheckBoxEdit* e = new CheckBoxEdit( "Rx", index.data().toString(), parent );
	e->setChecked(!index.data(Qt::DecorationRole).isNull() );
	e->installEventFilter(const_cast<CheckBoxDelegate*>(this));
	
	return e;
}

void CheckBoxDelegate::setModelData ( QWidget * w, QAbstractItemModel * model, const QModelIndex & index ) const
{
	CheckBoxEdit* cbox = dynamic_cast<CheckBoxEdit*>(w);
	if( !w ) return;

	//model->setData(index, cbox->currentText());
	if( cbox->isChecked() ) model->setData(index, _icon, Qt::DecorationRole);
	else model->setData(index, QVariant(), Qt::DecorationRole);

	model->setData(index, cbox->text());
}

void CheckBoxDelegate::setIcon(const QPixmap& icon)
{
	_icon = icon;
}
