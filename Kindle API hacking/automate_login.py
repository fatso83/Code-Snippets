import requests, re, sys
from bs4 import BeautifulSoup

def usage():
    print(sys.argv[0], " <email> <username> ")
    sys.exit(1);

if len(sys.argv) < 3:
    usage();

my_email = sys.argv[1]
my_password = sys.argv[2]

# persist cookies across requests
s = requests.Session()

user_agent = "User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/27.0.1453.110 Safari/537.36" 
extra_headers = { 'User-Agent' : user_agent }
s.headers.update(extra_headers) #needed in order to get amazon to return anything other than 404
amazon = 'https://www.amazon.com'

r = s.get(amazon + '/gp/digital/fiona/manage')
soup = BeautifulSoup(r.content)
inputs = soup.find_all('input')

attributes = [i.attrs for i in inputs]
form_values = {}

for a in attributes:
    form_values[a.get('name')] = a.get('value')
form_values.pop(None)

form_values['email'] = my_email
form_values['password'] = my_password

r2 = s.post(
        amazon + '/ap/signin',
        data = form_values,
        allow_redirects=True )

# Now we should be inside :-)
