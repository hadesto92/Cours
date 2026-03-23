#!"E:\Projekty\AWS_SERVER\runtime\python.exe" 
# -*- coding: utf-8 -*-
import sys
from awscli.clidriver import main
if __name__ == '__main__':
    sys.argv[0] = 'aws'
    sys.exit(main())
