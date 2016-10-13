from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText

fromaddr = "charlie.obrien@gmail.com"
toaddr = "charlie.obrien@gmail.com"
msg = MIMEMultipart()
msg['From'] = fromaddr
msg['To'] = toaddr
msg['Subject'] = "Python email"

body = '''Python test mail
that was easy, wasn't it?'''
msg.attach(MIMEText(body, 'plain'))

import smtplib
server = smtplib.SMTP('smtp.gmail.com', 587)
server.ehlo()
server.starttls()
server.ehlo()
server.login("juliet.obrien", "delbrook")
text = msg.as_string()
server.sendmail(fromaddr, toaddr, text)
#print server

