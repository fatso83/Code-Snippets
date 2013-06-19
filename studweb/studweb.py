##
# Program to alert you when your grades have arrived in StudWeb
# @author Carl-Erik Kopseng <carlerik@ifi.uio.no>
# @date June 20 2013
# Requires: Python 3
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
    inputs = soup.find_all('input')

    attributes = [i.attrs for i in inputs]
    form_values = {}

    for a in attributes:
        form_values[a.get('name')] = a.get('value')

    # remove superfluous data
    form_values.pop(None)

    form_values['fodselsnr'] = ssn
    form_values['pinkode'] = pin_code

    # This gets us in. The session cookies are crucial!
    #s.post( amazon + '/ap/signin',
    #        data = form_values,
    #        allow_redirects=True )
    print(form_values)
    print(soup)

if __name__ == '__main__':
    if len(sys.argv) < 3:
        usage();

    my_ssn = sys.argv[1]
    my_pin = sys.argv[2]

    init_session(my_ssn, my_pin)
