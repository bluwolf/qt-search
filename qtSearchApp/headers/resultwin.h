#ifndef RESULTWIN_H
#define RESULTWIN_H

#include <QWidget>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QString>
#include <QLabel>
#include <string>
#include <set>
#include <list>
#include <map>
#include "WebPage.h"
#include "Set.h"

class ResultWin : public QMainWindow
{
	Q_OBJECT

	public:
		ResultWin(std::string webName, std::map<std::string, WebPage*> *webPtrs, QWidget *parent = 0);
		~ResultWin();
		std::map<std::string, WebPage*> *pagePtrs;

	private slots:
		void updateWin(QListWidgetItem* item);
		
	private:
		QWidget * centerWidget;
		QHBoxLayout * mainLayout;
		QPlainTextEdit * pageText;
		QVBoxLayout * linkLayout;
		QLabel * incomingLabel;
		QLabel * outgoingLabel;
		QListWidget * incomingList;
		QListWidget * outgoingList;
		std::string name;
};

#endif
