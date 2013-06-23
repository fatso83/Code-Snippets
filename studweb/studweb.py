##
# Program to alert you when your grades have arrived in StudWeb
# @author Carl-Erik Kopseng <carlerik@ifi.uio.no>
# @date June 20 2013
# Requires: Python 3
#
# Inspired by studweb.py by Sindre FrÃ¸yn <sindrf@ifi.uio.no>, 2009
#
# It works by
# - logging into studweb, 
# - parsing the results 
# - comparing those with the previously fetched results
# - printing all results or just a formatted diff (if run with -d) to screen 
# - writingthe result to file for later comparison
##

import requests, re, sys, json
from bs4 import BeautifulSoup
from html.parser import HTMLParser

#The session object that persists cookies and default values across requests
s = requests.Session()
h = HTMLParser();
studweb = 'https://studweb.uio.no'

def usage():
    print("USAGE:", sys.argv[0], "<social security number> <studweb pin code>")
    sys.exit(1);

def init_session(ssn, pin_code):
    #we need to fake the user agent in order to get amazon to return anything other than 404
    #user_agent = "User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/27.0.1453.110 Safari/537.36" 
    #extra_headers = { 'User-Agent' : user_agent }
    #s.headers.update(extra_headers) 

    r = s.get(studweb)
    soup = BeautifulSoup(r.content)
    form = soup.select("form[name=fnrForm]")[0]
    inputs = soup.select("form[name=fnrForm] input") 

    attributes = [i.attrs for i in inputs]
    form_values = {}

    for a in attributes:
        form_values[a.get('name')] = a.get('value')

    form_values['fodselsnr'] = ssn
    form_values['pinkode'] = pin_code

    # remove the field that says we need to get the pin by sms
    form_values.pop('pinmail')

    # set the submit action to be Logg inn
    form_values['WOSubmitAction'] = "Logg inn"

    # This gets us in. The session cookies are crucial!
    #debug(form_values)
    #return
    action = form['action']
    #print(studweb + action)
    r = s.post( studweb + action,
            data = form_values,
            allow_redirects=True )

    # Når innlogget, husk å logge ut
    return r

def logout(html_page):
    if not html_page: 
        raise Exception("Missing legal string argument")
    soup = BeautifulSoup(html_page)
    link = soup.find_all('a',text=re.compile('Logg ut'))
    check(link, "Could not find <a> tag with text \"Logg ut\"")
    logoutUrl =  studweb + link[0]['href']
    #return logoutUrl 
    return s.get( logoutUrl )
    
def get_url_to_result_page(start_page):
        soup = BeautifulSoup(start_page)
        link = soup.find_all('a',text=re.compile('Se opplysninger om deg'))
        check(link, "Could not find <a> tag with text \"Se opplysninger om deg\"")
        r = s.get(studweb + link[0]['href'])
        soup = BeautifulSoup(r.content)
        link = soup.find_all("a", title="Se dine resultater")
        check(link, "Could not find <a> tag with title \"Se dine resultater\"")
        return studweb + link[0]['href']
        

def check(find_result, error_msg):
        if not find_result:
                raise Exception(error_msg)


def debug(o):
    print(str(o).encode('utf-8'))

if __name__ == '__main__':
    if len(sys.argv) < 3:
        usage();

    my_ssn = sys.argv[1]
    my_pin = sys.argv[2]

    # r = init_session(my_ssn, my_pin)
    # r=s.get(get_url_to_result_page(r.content))
    # soup = BeautifulSoup(r.content)
    # results_html = soup.prettify()
    #r= logout(r.content)
    results_html = ""
    f = open("results.html")
    for line in f:
	    results_html += line
    soup = BeautifulSoup(results_html)

    # parse the results table
    result_table = soup.table.table
    headers = result_table.find_all("th")
    
    index={}
    for s in ['Semester', 'Emnekode', 'Resultat']:
        hits = [ i for i, th in enumerate(headers) if th.text.find(s) >= 0 ]
        assert len(hits) > 0, "Did not find a header with the name %s" % s
        index[s]=hits[0]

    # only find rows with non-blank subject code
    relevant_trs = [tr 
		    for tr in result_table.find_all('tr')[1:] #Skip the first row with headers 
                    for i,c in enumerate(tr.children) 
                    if i == index['Emnekode'] and c.text.strip()]


    results=[]
    value_key = [v,k for k,v in index]
    for tr in relevant_trs:
	    result = {}
	    for i,c in enumerate(tr.find_all('td')):
		    if i in value_key:
			    result[key]=c.text
	    results.append(result)
