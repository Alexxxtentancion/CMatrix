from distutils.core import setup, Extension

setup(name='myMatrix',
      version='1.0', ext_modules=[Extension('myMatrix', ['matrix.cpp'])])
