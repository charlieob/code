import csv
import xml.etree.ElementTree as etree

modelsOfInterest = ('PVR4', 'PVR5', 'PVR6')

class STBmodel(object):
    def __init__(self, id):
        self.hw_id = id

def indent(elem, level=0):
  i = "\n" + level*"  "
  if len(elem):
    if not elem.text or not elem.text.strip():
      elem.text = i + "  "
    if not elem.tail or not elem.tail.strip():
      elem.tail = i
    for elem in elem:
      indent(elem, level+1)
    if not elem.tail or not elem.tail.strip():
      elem.tail = i
  else:
    if level and (not elem.tail or not elem.tail.strip()):
      elem.tail = i

models = []
with open('Approval Table Version 107 Sky.csv', 'rb') as f:
#with open('Approval Table Version 107 Samsung.csv', 'rb') as f:
#with open('sample.csv', 'rb') as f:
    f.readline()
    reader = csv.DictReader(f)
    for row in reader:
        if row ['HW-ID'] and row['Date of approval']:
            #print("%s %s %s %s %s" % (row['HW-ID'], row['CODE NAME'], row['Style'], row['Drive Vendor'], row['HDD Size (GB)']))
            model = STBmodel(row['HW-ID'])
            model.code = row['CODE NAME']
            model.style = row['Style']
            model.drive_vendor = row['Drive Vendor']
            model.hdd_size = row['HDD Size (GB)']
            models.append(model)
            
myModels = [m for m in models if m.code in modelsOfInterest]

modelList = etree.Element('BoxModels')
for m in myModels:
        modelElement = etree.SubElement(modelList, 'Model')
        modelElement.text = m.hw_id
        modelElement.set('type', m.code)
        modelElement.set('style', m.style)

indent(modelList)
print etree.tostring(modelList)

