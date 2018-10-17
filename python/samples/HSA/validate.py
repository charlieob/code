import sys
import json
import jsonschema

def usage():
    print "usage: validate.py <schema> <data>"

def main(args):
    schema = open(args[0]).read()
    print schema

    data = open(args[1]).read()
    print data
    
    # print "validating schema and data..."

    # try:
        # jsonschema.validate(json.loads(data), json.loads(schema))
    # except jsonschema.ValidationError as e:
        # print "validation error"
        # print e.message
    # except jsonschema.SchemaError as e:
        # print "schema error"
        # print e

    # # Use a Draft4Validator
    # try:
        # jsonschema.Draft4Validator(json.loads(schema)).validate(json.loads(data))
    # except jsonschema.ValidationError as e:
        # print e.message

    print "validating data..."

    # Lazily report all errors in the instance
    v = jsonschema.Draft4Validator(json.loads(schema))
    # errors = sorted(v.iter_errors(json.loads(data)), key=str)
    # print errors
    for error in sorted(v.iter_errors(json.loads(data)), key=str):
        print('\n' + error.message)

if __name__=='__main__':

    if len(sys.argv) != 3:
        usage()
        sys.exit(2)

    main(sys.argv[1:])
