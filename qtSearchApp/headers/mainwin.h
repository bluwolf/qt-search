#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QRadioButton>
#include <QString>
#include <string>
#include <set>
#include <deque>
#include <map>
#include "WebPage.h"
#include "Set.h"
#include "resultwin.h"
#include "AdInfo.h"


class MainWin : public QMainWindow
{
	Q_OBJECT

	public:
		MainWin(QWidget *parent = 0);
		~MainWin();

		std::map<std::string, WebPage*> webPtrs; // name of webpage and pointer to page

		int initializeMaps();

		int initializeAds();
		void listAds(Set<std::string> & strSet);
		void updateBill(string s);

		void correctCase(std::string & s);
		void checkAlphaNum (std::string s, Set<std::string> & strSet);

		std::map< WebPage*, Set<WebPage*> > setGraph (Set<WebPage> s);
		void doPageRank (std::map< WebPage*, Set<WebPage*> > adjacencyList);



	private slots:
		void openResult(QListWidgetItem *item);
		void openAd(QListWidgetItem *item);
		
		void showAbout();

		void singleSearch();
		void orSearch();
		void andSearch();

	private:
		QVBoxLayout * mainWindowLayout;
		QWidget * centerWidget;
		QHBoxLayout * resultTypeLayout;
		QPushButton * aboutButton;
		QHBoxLayout * searchButtonLayout;
		QPlainTextEdit * txtWordsInput;
		QLabel * searchListLabel; 
		QLabel * adListLabel;
		QListWidget * mainResults;
		QListWidget * adResults; // new for hw10
		QPushButton * singleButton;
		QPushButton * orButton;
		QPushButton * andButton;
		QRadioButton * alphaButton;
		QRadioButton * rankButton;

		std::map< std::string, Set<WebPage> > webMap;
		std::map<std::string, Ad> adMap;

		std::deque<ResultWin*> listResultWindows;
};

#endif
