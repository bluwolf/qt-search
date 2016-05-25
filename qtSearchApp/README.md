##To make:
+ qmake, make (don't use qmake -project)

##To run search engine with ad-enable feature, the commands are as follows: 
+ bin/qtSearchApp data/webpagedirectory.txt data/companyInfo.txt data/billinginfo.txt
+ 

##To test the web crawler, the terminal commands are as follows:
+ ./html_web_crawler realpages/pageDirectory.txt [website of your choice]

+ In order for the html_web_crawler to compile, you need to have the curl library installed on your system

+ NOTE: The crawler takes some time to finish doing its thing, so we recommend trying www.usc.edu first; it has an average running time of 1 minute. An example of a longer running time (about 5 minutes and 400 crawled pages) is http://www.ign.com/. To see the list of pages crawled, there is a list in the realpages repo. 

