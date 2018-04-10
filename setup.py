from setuptools import setup, find_packages
from codecs import open
from os import path

here = path.abspath(path.dirname(__file__))

# Get the long description from the README file
with open(path.join(here, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

setup(
    name='pyBIE2D_Speed_Tests',
    version='0.0.1',
    description='Speed Tests for Boundary integral equation tools in 2D',
    long_description=long_description,
    url='https://bitbucket.org/dstein_flatiron/pybie2d_speed_tests/',
    author='David Stein',
    author_email='dstein@flatironinstitute.org',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Scientists/Mathematicians',
        'License :: Apache 2',
        'Programming Language :: Python :: 2',
    ],
    packages=find_packages(),
    install_requires=['numpy', 'numexpr', 'numba'],
)
