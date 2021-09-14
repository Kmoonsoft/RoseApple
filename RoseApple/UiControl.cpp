//-----------------------------------------------------------------------------
// �����̸�: UiControl.h
// ������: Lucky Kim
// ��������: 2008.6.2
// ��    ��: 1.0
// ��    ��: Qt UI ������ �������� �����ϱ� ���� Ŭ����
//-----------------------------------------------------------------------------
/* �����̷�: 
			 2008.07.10 Lucky Kim	Qt4.2���� �����Ǿ� Ʈ�������� Ȯ�� �ϴ� �κ� ����
*/
#include <QtGui>
#include <QtWidgets>
#include "UiControl.h"

// QSplitter Widget�� �յ���� �Ѵ�.
// �Է� : w ��� ���ø���
void
UiControl::InitSplitter( QSplitter* w )
{
	QList<int> l;
	
	for( int i = 0; i < w->count(); i++ )
	{
		l << 1;// �յ� ����
	}

	w->setSizes(l);
}

// QSplitter Widget�� 2���� ���� ������ �����Ѵ�.
// �Է� : w ��� ���ø���, a�� b�� ���� ����
void
UiControl::InitSplitter( QSplitter* w, int a, int b )
{
	QList<int> l;

	// ���� ����
	l << a*10000 << b*10000;

	w->setSizes(l);
}

// QSplitter Widget�� 3���� ���� ������ �����Ѵ�.
// �Է� : w�� ��� ���ø���,  a, b, c���� ���� �����̴�.
void
UiControl::InitSplitter( QSplitter* w, int a, int b, int c )
{
	QList<int> l;

	// ���� ����
	l << a*10000 << b*10000 << c*10000;

	w->setSizes(l);
}

// QTableView Widget�� �ʱ�ȭ�Ѵ�. ������ �԰����� �ʱ�ȭ
// �Է� : w�� ��� ���̺� ��
void
UiControl::InitTable( QTableView* w )
{
	// ���� �ش� ����
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);									// ���� �ش� �̵� ����
	v->setHidden(true);										// ���� �ش� ����
	v->setDefaultSectionSize( 18 );							// �����ش� ���̴� 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );				// �ؽ�Ʈ �߾�����

	// ���� �ش� ����
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);									// ���� �ش� �̵� ����
	r->setSortIndicatorShown(true);							// ���� �ε����̴� ǥ��

	w->setAlternatingRowColors(true);						// ���پ� ȸ�� ä��� ����
	w->setEditTriggers( QAbstractItemView::NoEditTriggers );//������ ����

	// ���� ������ ���� ��� ����
	w->disconnect( r, SIGNAL(sectionPressed(int)), w, SLOT(selectColumn(int)) );
	w->connect( r, SIGNAL(sectionClicked(int)), w, SLOT(sortByColumn(int)) );
}

// QTableView Widget�� �ʱ�ȭ�Ѵ�. ������ �԰����� �ʱ�ȭ
// �Է� : w�� ��� ���̺� ��
void
UiControl::InitTable2( QTableView* w )
{
	// ���� �ش� ����
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// ���� �ش� �̵� ����
	v->setHidden(true);							// ���� �ش� ����
	v->setDefaultSectionSize( 18 );				// �����ش� ���̴� 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// �ؽ�Ʈ �߾�����

	// ���� �ش� ����
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// ���� �ش� �̵� ����
	r->setSortIndicatorShown(true);				// ���� �ε����̴� ǥ��

	w->setAlternatingRowColors(true);			// ���پ� ȸ�� ä��� ����

	// ���� ������ ���� ��� ����
	w->disconnect( r, SIGNAL(sectionPressed(int)), w, SLOT(selectColumn(int)) );
	w->connect( r, SIGNAL(sectionClicked(int)), w, SLOT(sortByColumn(int)) );
}

// QTableView Widget�� �ʱ�ȭ�Ѵ�. ������ �԰����� �ʱ�ȭ
// �Է� : w�� ��� ���̺� ��
void
UiControl::InitTable3( QTableView* w )
{
	// ���� �ش� ����
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// ���� �ش� �̵� ����
	v->setHidden(true);							// ���� �ش� ����
	v->setDefaultSectionSize( 18 );				// �����ش� ���̴� 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// �ؽ�Ʈ �߾�����

	// ���� �ش� ����
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// ���� �ش� �̵� ����
	r->setSortIndicatorShown(false);			// ���� �ε����̴� �����

	w->setAlternatingRowColors(true);			// ���پ� ȸ�� ä��� ����
}

// QTableView Widget�� �ʱ�ȭ�Ѵ�. ������ �԰����� �ʱ�ȭ
// �Է� : w�� ��� ���̺� ��
void
UiControl::InitTable4( QTableView* w )
{
	// ���� �ش� ����
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// ���� �ش� �̵� ����
	v->setDefaultSectionSize( 20 );				// �����ش� ���̴� 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// �ؽ�Ʈ �߾�����
	v->setSectionResizeMode(QHeaderView::Fixed);		// ���� �ش� ������ ����

	// ���� �ش� ����
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// ���� �ش� �̵� ����
	r->setSortIndicatorShown(false);			// ���� �ε����̴� �����

	w->setAlternatingRowColors(true);			// ���پ� ȸ�� ä��� ����
	w->setEditTriggers( QAbstractItemView::NoEditTriggers );//������ ����
}

// ������Ʈ(parent)�� ���� �ڽ� ������Ʈ�� QLineEdit�� QTextEdit�̸� ������ Ŭ���� ��Ų��.
//	�Է� : parent �κ� ������Ʈ
void UiControl::ClearEdit(const QObject* parent)
{
	if( !parent ) return;
	
	const QObjectList& list = parent->children();
	foreach(QObject* o, list) {
		//���� ����Ʈ Ŭ����
		QLineEdit* e = dynamic_cast<QLineEdit*>(o);
		if( e ) {
			e->clear();
			continue;
		}

		// TextEdit Ŭ����
		QTextEdit* t  = dynamic_cast<QTextEdit*>(o);
		if( t ) {
			t->clear();
			continue;
		}
		
		if( dynamic_cast<QLabel*>(o)) continue; // ���̺� �ǳʶ��
		if( dynamic_cast<QComboBox*>(o) ) continue;// �޺��ڽ� �ǳʶ��

		ClearEdit(o);
	}
}

//-----------------------------------------------------------------------------
//	Double Validator ����
//	QDoubleValidator�� ���׷� ���� �籸����
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
//	Ư�� Validator�� ������ ������ Delegate ����
//-----------------------------------------------------------------------------
ItemValidationDelegate::ItemValidationDelegate(QValidator* v, QObject* parent ) 
: QItemDelegate( parent ), _v(v)
{
	// do nothing
}

// �ı���
ItemValidationDelegate::~ItemValidationDelegate()
{
	// do nothing
}

// ���� ������ ������ �����Ͽ� ��ȯ�Ѵ�.
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
//	Ư�� �Է� ����ũ�� ������ ������ Delegate ����
//-----------------------------------------------------------------------------
ItemMaskDelegate::ItemMaskDelegate(const QString& mask, QObject* parent ) 
: QItemDelegate( parent ), _mask(mask)
{
	// do nothing
}

// �ı���
ItemMaskDelegate::~ItemMaskDelegate()
{
	// do nothing
}

// ���� ������ ������ �����Ͽ� ��ȯ�Ѵ�.
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
//	�޺��ڽ� ������
//-----------------------------------------------------------------------------
ComboBoxItemDelegate::ComboBoxItemDelegate(const QStringList& list, QObject* parent ) 
: QItemDelegate( parent ), _list(list)
{
	// do nothing
}

// �ı���
ComboBoxItemDelegate::~ComboBoxItemDelegate()
{
	// do nothing
}

// ���� ������ ������ �����Ͽ� ��ȯ�Ѵ�.
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
//	üũ�� ��Ʈ���� �տ� �������� ǥ���ϴ� ��
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

// �������� �����Ѵ�.
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
//	üũ�� ��Ʈ���� �տ� �������� ǥ���ϴ� �� Delegate
//-----------------------------------------------------------------------------
CheckStringListDelegate::CheckStringListDelegate(const QStringList& list, const QStringList& values, QObject* parent ) 
: QItemDelegate( parent ), _l(list), _v(values)
{
	
}

// �ı���
CheckStringListDelegate::~CheckStringListDelegate()
{
	
}

// ���� ������ ������ �����Ͽ� ��ȯ�Ѵ�.
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
//	üũ�ڽ� + ���ο��� ����
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
//	üũ�ڽ� Delegate
//-----------------------------------------------------------------------------
// ������
// ������ ����� Validator�� ������ �ش�.
CheckBoxDelegate::CheckBoxDelegate(QObject* parent)
	: QItemDelegate( parent )
{
	_icon = QPixmap(":/MDMg/Resources/convert.png");
}

CheckBoxDelegate::~CheckBoxDelegate(){}

// ���� ������ ������ �����Ͽ� ��ȯ�Ѵ�.
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
