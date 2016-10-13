import smtplib

def prompt(prompt):
    return raw_input(prompt).strip()

fromaddr = prompt("From: ")
toaddrs  = prompt("To: ").split()
print "Enter message, blank line to end:"

# Add the From: and To: headers at the start!
msg = ("From: %s\r\nTo: %s\r\n"
       % (fromaddr, ", ".join(toaddrs)))
msg += "Subject: python email test\r\n\r\n"

while 1:
    try:
        line = raw_input()
    except EOFError:
        break
    if not line:
        break
    msg = msg + line + '\r\n'

print "Message length is " + repr(len(msg))
print "Message is:"
print msg

#server = smtplib.SMTP('localhost')
server = smtplib.SMTP('smtp.gmail.com', 587)
server.set_debuglevel(1)
server.ehlo()
server.starttls()
#server.ehlo()
server.login("juliet.obrien", "delbrook")
server.sendmail(fromaddr, toaddrs, msg)
server.quit()
