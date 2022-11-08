#!/usr/bin/env python3

import cgi
from http.cookies import SimpleCookie
import hashlib
import subprocess

PATH_TO_PASSWD = "./machine_passwd"
redirectURL = "./admin.cgi"
redirectURL1 = "./admin1.cgi"
redirectURL2 = "./admin2.cgi"
redirectURL3 = "./admin3.cgi"
redirectURL4 = "./admin4.cgi"
redirectURL5 = "./admin5.cgi"
PATH_TO_MACHINE="./etovucca"

def checkOutput(data):
				# CGI Redirect: https://stackoverflow.com/a/6123179
                print('Content-Type: text/html')
                print('Location: %s' % redirectURL)
                C = SimpleCookie()
                #C['user'] = h.hexdigest() # U+1F914
                #print(C)
                print('')
                print('<html>')
                print('<head>')
                print('<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">')
                print('<meta http-equiv="refresh" content="0;url=%s" />' % data)
                print('<title>You are going to be redirected</title>')
                print('</head>')
                print('<body>')
                print('Redirecting... <a href="%s">Click here if you are not redirected</a>' % redirectURL)
                print('</body>')
                print('</html>')


def login(data):
	if(int(data) == 1):
                # CGI Redirect: https://stackoverflow.com/a/6123179
                print('Content-Type: text/html')
                print('Location: %s' % redirectURL)
                C = SimpleCookie()
                C['data'] = data
                C['user'] = h.hexdigest() # U+1F914
                print(C)
                print('')
                print('<html>')
                print('<head>')
                print('<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">')
                print('<meta http-equiv="refresh" content="0;url=%s" />' % redirectURL)
                print('<title>You are going to be redirected</title>')
                print('</head>')
                print('<body>')
                print('Redirecting... <a href="%s">Click here if you are not redirected</a>' % redirectURL)
                print('</body>')
                print('</html>')
	elif(int(data) == 2):
                # CGI Redirect: https://stackoverflow.com/a/6123179
                print('Content-Type: text/html')
                print('Location: %s' % redirectURL1)
                C = SimpleCookie()
                C['user'] = h.hexdigest() # U+1F914
                C['data'] = data
                print(C)
                print('')
                print('<html>')
                print('<head>')
                print('<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">')
                print('<meta http-equiv="refresh" content="0;url=%s" />' % redirectURL1)
                print('<title>You are going to be redirected</title>')
                print('</head>')
                print('<body>')
                print('Redirecting... <a href="%s">Click here if you are not redirected</a>' % redirectURL1)
                print('</body>')
                print('</html>')
	elif(int(data) == 3):
                # CGI Redirect: https://stackoverflow.com/a/6123179
                print('Content-Type: text/html')
                print('Location: %s' % redirectURL2)
                C = SimpleCookie()
                C['user'] = h.hexdigest() # U+1F914
                C['data'] = data
                print(C)
                print('')
                print('<html>')
                print('<head>')
                print('<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">')
                print('<meta http-equiv="refresh" content="0;url=%s" />' % redirectURL2)
                print('<title>You are going to be redirected</title>')
                print('</head>')
                print('<body>')
                print('Redirecting... <a href="%s">Click here if you are not redirected</a>' % redirectURL2)
                print('</body>')
                print('</html>')
	elif(int(data) == 4):
                # CGI Redirect: https://stackoverflow.com/a/6123179
                print('Content-Type: text/html')
                print('Location: %s' % redirectURL3)
                C = SimpleCookie()
                C['user'] = h.hexdigest() # U+1F914
                C['data'] = data
                print(C)
                print('')
                print('<html>')
                print('<head>')
                print('<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">')
                print('<meta http-equiv="refresh" content="0;url=%s" />' % redirectURL3)
                print('<title>You are going to be redirected</title>')
                print('</head>')
                print('<body>')
                print('Redirecting... <a href="%s">Click here if you are not redirected</a>' % redirectURL3)
                print('</body>')
                print('</html>')
	elif(int(data) == 5):
                # CGI Redirect: https://stackoverflow.com/a/6123179
                print('Content-Type: text/html')
                print('Location: %s' % redirectURL4)
                C = SimpleCookie()
                C['user'] = h.hexdigest() # U+1F914
                C['data'] = data
                print(C)
                print('')
                print('<html>')
                print('<head>')
                print('<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">')
                print('<meta http-equiv="refresh" content="0;url=%s" />' % redirectURL4)
                print('<title>You are going to be redirected</title>')
                print('</head>')
                print('<body>')
                print('Redirecting... <a href="%s">Click here if you are not redirected</a>' % redirectURL4)
                print('</body>')
                print('</html>')  
	else:
                raise ValueError('incorrect hash')

def render_login(failure=False, logout=False):
    print("Content-Type: text/html")
    if logout:
        print("Set-Cookie: user=LOGGEDOUT; expires=Thu, 01 Jan 1970 00:00:00 GMT")
    print()
    print('<link rel="stylesheet" href="https://spar.isi.jhu.edu/teaching/443/main.css">')
    print('<h2 id="dlobeid-etovucca-voting-machine">DLOBEID EtovUcca Voting Machine</h2>')
    print('<h1 id="admin-interface-login">Admin Interface Login</h1><br>')
    if failure:
        print('<b>Login Failed.</b>')
    print('<form method="post">')
    print('<label for="passwd">Admin Password:</label>')
    print('<input type="password" id="passwd" name="passwd">')
    print('<input type="submit" value="Login">')
    print('</form>') 
    print("<a href='./home.cgi'>Return to Homepage</a>")

form = cgi.FieldStorage()

try:
    if 'passwd' in form:           
        #Please don't ever actually do this.
        h = hashlib.new('md5')  #U+1F914
        h.update(form.getvalue('passwd').encode('utf-8'))
        data = subprocess.check_output([PATH_TO_MACHINE, "validate-input", h.hexdigest()])
        if(data):
        	login(data)

           
    elif 'logout' in form:
        render_login(logout=True)
    else:
        render_login()
except Exception:
    render_login(failure=True)
