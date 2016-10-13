from xml.etree.ElementTree import ElementTree as ET

# build a tree structure
root = xml.etree.ElementTree.Element("html")
#root = ET.Element("html")

head = ET.SubElement(root, "head")

title = ET.SubElement(head, "title")
title.text = "Page Title"

body = ET.SubElement(root, "body")
body.set("bgcolor", "#ffffff")

body.text = "Hello, World!"

# wrap it in an ElementTree instance, and save as XML
tree = ET.ElementTree(root)
tree.write("page.xhtml")