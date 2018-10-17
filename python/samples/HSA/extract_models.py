# TBD: Some remarks here on the script...
# 
#
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
            model = STBmodel(row['HW-ID'])
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
            model = STBmodel(row['HW-ID'])
            model.manufacturer = "Pace"
            model.code = row['CODE NAME']
            model.style = row['Style']
            if model.code in modelsOfInterest:
                model.style = model.code
            model.drive_vendor = row['Drive Vendor'].replace('\n', "")
            model.hdd_size = row['HDD Size (GB)']
            models.append(model)
            
with open('Approval Table Version 108 Samsung.csv', 'rb') as f:
    f.readline()
    reader = csv.DictReader(f)
    for row in reader:
        if row ['HW-ID'] and (row['Date of approval'] or ("Approved for production" in row['Approval valid'])):
            model = STBmodel(row['HW-ID'])
            model.manufacturer = "Samsung"
            model.code = row['CODE NAME']
            model.style = row['Style']
            if model.code in modelsOfInterest:
                model.style = model.code
            model.drive_vendor = row['Drive Vendor'].replace('\n', "")
            model.hdd_size = row['HDD Size (GB)']
            models.append(model)
            
selectedModels = [m for m in models if (m.code in modelsOfInterest or 'DRX595' in m.style)]

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

