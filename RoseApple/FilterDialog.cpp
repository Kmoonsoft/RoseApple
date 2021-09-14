#include "FilterDialog.h"

FilterDialog::FilterDialog(const QString& filter, QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	ui.setupUi(this);

	ui.filterEdit->setPlainText(filter);
}

FilterDialog::~FilterDialog()
{

}

QString FilterDialog::getFilter()
{
	return ui.filterEdit->toPlainText();
}



void FilterDialog::on_okBtn_clicked()
{
	QDialog::accept();
}

void FilterDialog::on_cancelBtn_clicked()
{
	QDialog::reject();
}