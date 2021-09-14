#include <QApplication>
#include <QTextCodec>
#include "RoseApple.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	RoseApple w;
	w.show();
	return a.exec();
}
