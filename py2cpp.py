#!/usr/bin/python

from ctypes import *
import os 

lib = cdll.LoadLibrary('extraction.so') 
print lib.extractCM() 

