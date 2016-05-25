#include <QApplication>
#include "mainwin.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	if(argc !=4){
		cout<<"Please enter the right number of command line arguments"<<endl;
		return 0;
	}

	MainWin w;
	w.show();
	return app.exec();
}
