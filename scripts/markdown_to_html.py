#!/usr/env/python
import markdown
import sys

def main(argv):
    md = markdown.Markdown()
    
    try:
        md_file = argv[0]
    except IndexError:
        print 'No file passed'
        sys.exit(1)
        
    with open(md_file) as f:
        print md.convert(f.read())
        return 0


if __name__ == "__main__":
    main(sys.argv[1:])
