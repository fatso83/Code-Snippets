import requests, re, sys
from bs4 import BeautifulSoup

my_email = 'carlerik@gmail.com'
user_agent = "User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/27.0.1453.110 Safari/537.36" 
extra_headers = { 'User-Agent' : user_agent }

r = requests.get('http://www.amazon.com/gp/digital/fiona/manage', headers = extra_headers)
soup = BeautifulSoup(r.content)
inputs = soup.find_all('input')

attributes = [i.attrs for i in inputs]
form_values = {}

for a in attributes:
    form_values[a.get('name')] = a.get('value')
form_values.pop(None)

form_values['email'] = my_email
form_values['password'] = sys.argv[1]
