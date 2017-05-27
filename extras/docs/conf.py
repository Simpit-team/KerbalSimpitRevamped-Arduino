import sys
import os
import shlex
import subprocess

read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

if read_the_docs_build:
    subprocess.call('doxygen', shell=True)

extensions = ['breathe']
breathe_projects = { 'KerbalSimpit': 'xml' }
breathe_default_project = "KerbalSimpit"
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'
project = u'Kerbal Simpit'
copyright = u'2017, Peter Hardy'
author = u'Peter Hardy'
version = '1.0'
release = '1.0'
language = None
exclude_patterns = ['_build']
pygments_style = 'sphinx'
todo_include_todos = False
html_static_path = ['_static']
htmlhelp_basename = 'KerbalSimpitdoc'
latex_elements = {
}
latex_documents = [
  (master_doc, 'KerbalSimpit.tex', u'KerbalSimpit Documentation',
   u'KerbalSimpit', 'manual'),
]
