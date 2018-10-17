# TBD: Some remarks here on the script...
# 
#
import sys
import csv
import xml.etree.ElementTree as etree

modelsOfInterest = ('PVR4', 'PVR5', 'PVR6')
namingSubstitutes = {"DRX780UK": "DRX780"}

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
with open('Approval Table Version 108 Sky.csv', 'rb') as f:
    f.readline()
    reader = csv.DictReader(f)
    for row in reader:
        if row ['HW-ID'] and (row['Date of approval'] or ("Approved for production" in row['Approval valid for : '])):
            hw_id = row['HW-ID']
            model = STBmodel(hw_id)
            model.manufacturer = "Sky"
            model.code = row['CODE NAME']
            model.style = row['Style']
            if model.style in namingSubstitutes:
                model.style = namingSubstitutes[model.style]
            model.drive_vendor = row['Drive Vendor'].replace('\n', "")
            model.hdd_size = row['HDD Size (GB)']
            models.append(model)
            
with open('Approval Table Version 108 Pace.csv', 'rb') as f:
    f.readline()
    reader = csv.DictReader(f)
    for row in reader:
        if row ['HW-ID'] and (row['Date of Approval'] or ("Approved for production" in row['Approval valid for : '])):
            hw_id = row['HW-ID']
            model = STBmodel(hw_id)
            model.manufacturer = "Pace"
            model.code = row['CODE NAME']
            model.style = model.code
            model.drive_vendor = row['Drive Vendor'].replace('\n', "")
            model.hdd_size = row['HDD Size (GB)']
            models.append(model)
            
with open('Approval Table Version 108 Samsung.csv', 'rb') as f:
    f.readline()
    reader = csv.DictReader(f)
    for row in reader:
        if row ['HW-ID'] and (row['Date of approval'] or ("Approved for production" in row['Approval valid'])):
            hw_id = row['HW-ID']
            model = STBmodel(hw_id)
            model.manufacturer = "Samsung"
            model.code = row['CODE NAME']
            model.style = model.code
            model.drive_vendor = row['Drive Vendor'].replace('\n', "")
            model.hdd_size = row['HDD Size (GB)']
            models.append(model)
            
selectedModels = [m for m in models if (m.code in modelsOfInterest or 'DRX595' in m.style)]

futureAllocations = ["DRX595L", "DRX595H", "DRX595HL", "DRX895WL", "DRX890WL"]
for modelType in futureAllocations:
    with open('Future HWID_allocation_25082015_' + modelType + '.csv', 'rb') as f:
        style = f.readline().replace(',','').strip()
        reader = csv.DictReader(f)
        for row in reader:
            hw_id = row['HWID']
            if hw_id:
                model = STBmodel(hw_id)
                model.manufacturer = "Sky"
                model.style = style
                model.drive_vendor = 'N/A'
                selectedModels.append(model)

# check there are no duplicated hw_id values
allocated = set()
error_found = False
for m in selectedModels:
    hw_id = m.hw_id
    if hw_id in allocated:
        print("Error: conflict on hw id = {}".format(hw_id))
        error_found = True
    else:
        allocated.add(hw_id)

if error_found:
    sys.exit()

xmlList = etree.Element('HardwareModels')
for m in selectedModels:
    modelElement = etree.SubElement(xmlList, 'Model')
    modelElement.text = m.hw_id
    modelElement.set('manufacturer', m.manufacturer)
    modelElement.set('style', m.style)
    if m.drive_vendor != 'N/A':
        modelElement.set('hddvendor', m.drive_vendor)
        modelElement.set('hddsize', m.hdd_size)

indent(xmlList)
print """<?xml version="1.0" encoding="utf-8"?>"""
print etree.tostring(xmlList, encoding="utf-8")

