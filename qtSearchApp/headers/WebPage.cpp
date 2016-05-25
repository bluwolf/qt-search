#include "WebPage.h"

using namespace std;

WebPage::WebPage()
{
}

WebPage::WebPage (string fname)
{
  // open file
  ifstream ifile (fname);

  // check if file opened
  if (ifile.fail())
    throw std::invalid_argument ("Bad file name.");

  // set filename_
  filename_ = fname;

  string word;
  string read_line;
  // read in words and store in wordList
  while (!ifile.eof()){
    // read in line
    getline(ifile,read_line);

    // check for anchor and links
    size_t anchor_front;
    do {
      anchor_front = read_line.find("[");
      size_t anchor_end = read_line.find("]");
      size_t link_front = read_line.find("(");
      size_t link_end = read_line.find(")");
      if ((anchor_front != string::npos) && (anchor_end != string::npos) && (link_front != string::npos) && (link_end != string::npos) && 
      (anchor_front < anchor_end) && (link_front < link_end) && (anchor_end < link_front)) {
        // make substring of link next to anchor text
        string link_name = read_line.substr(link_front+1,(link_end-link_front-1));
        // add link to outlinksList; checks if page tries to link to itself
        if (link_name != filename_)
          outlinksList.insert(link_name);
        // remove brackets and links from read_line
        read_line.erase(anchor_front,1);
        anchor_end = read_line.find("]");
        read_line.erase(anchor_end,1);
        link_front = read_line.find("(");
        link_end = read_line.find(")");
        read_line.erase(link_front,(link_end-link_front+1));
      }
    } while (anchor_front != string::npos);

    stringstream ss;
    ss << read_line;
    while (!ss.fail()) {
      ss >> word;
      if (ss.fail())
        break;

      // loop to break up initial word into its correct parts
      while (!word.empty()){
        unsigned int numChar = 0;
        // check if word is alphanumeric
        while (isalnum(word[numChar])){
	  // makes characters lowercase
  	  word[numChar] = towlower(word[numChar]);
	  numChar++;
        }

        string subWord;
        // if word is interuppted by symbol,
        // creates substring of just alphanumeric and adds it to wordList
        // word becomes whatever comes after the substring
        if (numChar < word.size()){
	  subWord = word.substr(0,numChar);
  	  word = word.substr(numChar+1);
          if (!subWord.empty())
            wordList.insert(subWord);
        }
        // if word is all alphanumeric, add it to wordList
        else {
          wordList.insert(word);
	  break;
        }
      }
    }
  }

  // close file
  ifile.close();

  // initialize currentRank
  currentRank = 0.0;
}

WebPage::~WebPage ()
{
}

Set<string> WebPage::allWords () const
{
  return wordList;
}

Set<string> WebPage::stringOutgoingSet () const
{
  return outlinksList;
}

Set<WebPage*> WebPage::allOutgoingLinks (map<string, WebPage*> *pagePtrs) const
{
  Set<WebPage*> outgoing;
  typename std::set<string>::iterator string_it = outlinksList.begin();
  while (string_it != outlinksList.end()){
    try {
      outgoing.insert(pagePtrs->at(*string_it));
      ++string_it;
    }
    catch (const out_of_range & oor) {
      cout << "Invalid link given: " << *string_it << endl;
    }
  }

  return outgoing;
}

Set<WebPage*> WebPage::allIncomingLinks (map<string, WebPage*> *pagePtrs) const
{
  Set<WebPage*> incoming;

  typename std::map<string, WebPage*>::iterator pagePtrs_it = pagePtrs->begin();
  while (pagePtrs_it != pagePtrs->end()) {
    WebPage* webPtr = pagePtrs_it->second;
    if (webPtr->filename() == (this->filename_)) {
      ++pagePtrs_it;
      continue;
    }
    Set<WebPage*> webPageSet = webPtr->allOutgoingLinks(pagePtrs);
    typename std::set<WebPage*>::iterator webPage_it = webPageSet.begin();
    while (webPage_it != webPageSet.end()) {
      if ((*webPage_it)->filename() == filename_)
        incoming.insert(webPtr);
      ++webPage_it;
    }

    ++pagePtrs_it;
  }

  return incoming;
}

string WebPage::filename () const
{
  return filename_;
}

bool WebPage::operator== (const WebPage & page)
{
  return (filename_ == page.filename_);
}

bool WebPage::operator< (const WebPage & page) const
{
  return (filename_ < page.filename_);
}

ostream & operator<< (ostream & os, const WebPage & page)
{
  // open file
  ifstream ifile (page.filename_);

  // check if file opened
  if (ifile.fail())
    throw std::invalid_argument ("Bad file name.");

  // get line from file then print out line
  while (!ifile.eof()){
    string line;
    getline(ifile,line);

    // check for anchor and links
    size_t anchor_front;
    do {
      anchor_front = line.find("[");
      size_t anchor_end = line.find("]");
      size_t link_front = line.find("(");
      size_t link_end = line.find(")");
      if ((anchor_front != string::npos) && (anchor_end != string::npos) && (link_front != string::npos) && (link_end != string::npos) && 
      (anchor_front < anchor_end) && (link_front < link_end) && (anchor_end < link_front)) {
        // remove brackets and links from line
        line.erase(anchor_front,1);
        anchor_end = line.find("]");
        line.erase(anchor_end,1);
        link_front = line.find("(");
        link_end = line.find(")");
        line.erase(link_front,(link_end-link_front+1));
      }
    } while (anchor_front != string::npos);

    os << line << endl;
  }

  ifile.close();

  return os;
}

void WebPage::setRank (double cr)
{
  currentRank = cr;
}

double WebPage::getRank ()
{
  return currentRank;
}
