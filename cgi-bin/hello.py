#! /usr/bin/python3

import os
import cgi
import cgitb
import logging
import datetime

cgitb.enable()
logging.basicConfig(filename="hello.log", level=logging.DEBUG)
def hello():
	envp = os.environ
	content = "<h1>A script that doesn't sleep: {}</h1>".format(datetime.datetime.now())
	response = "HTTP/1.1 200 OK\r\n"
	response += "Content-Type: text/html\r\n"
	response += "Content-Length: {}\r\n".format(len(content + "\r"))
	response += "\r\n"
	response += content
	print(response)
	## touch or append to a file to check if the script is called
	with open("hello.txt", "a") as f:
		f.write(f"Hello {datetime.datetime.now()}\n")
	

# will be called from CGI script

hello()