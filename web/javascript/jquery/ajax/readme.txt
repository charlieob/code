This sample code is based on the main example in the AJAX tutorial here:
http://w3schools.com/jquery/jquery_ajax.asp

Various browsers have different behaviours in relation to "cross-domain/origin browsing". In general,
there is a security consideration here, see http://en.wikipedia.org/wiki/Same_origin_policy
Two resources are considered to be of the same origin if all the following match exactly:
- protocol
- domain name
- port number

Chrome in particular (Chrome 16) will not allow the fetching of the local file ajax_info.txt due
to its security approach. It will work fine when the file is fetched from a remote server which
is also delivering the html file and javascript.
See http://code.google.com/p/chromium/issues/detail?id=40787 for a discussion of this.
The suggested work-around is to use the command-line switch --allow-file-access-from-files
Open a command window and run open_in_chrome.bat from there, or simply double-click on the .bat file.
You will have to close any Chrome browsers that are already open as otherwise this will only open a
new tab in the existing browser and the command-line option won't have been activated.

Note: you can check the command-line options active in Chrome by browsing to
chrome://version
