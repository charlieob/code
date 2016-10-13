import cgi
import cgitb; cgitb.enable()

print "Content-type: text/html"
print # a blank line

import time

print "<title>CGI 101</title>"
print "<h1>A CGI Example</h1>"
print "<p>Hello, CGI World!</p>"
print "<p>the time is: %s</p>" % time.ctime()

form = cgi.FieldStorage()
print "<p>name:", form.getfirst("name", "")
print "<p>password:", form.getfirst("password", "")
