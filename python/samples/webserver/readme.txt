Simple web server - just serving files relative to current directory
-----
- remove (or hide) index.html
- python http_server.py
- browse to localhost:8000

You should see a listing of the current directory and be able to browse from there


Simple CGI-based server
----
- have index.html available in this directory
- python cgihttp_server.py
- browse to localhost:8000

You should see the login page as given in index.html and on pressing the login button should receive a new page from the CGI script login.py
