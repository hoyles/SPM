import string
import re

p = re.compile('\{.*?\}',re.IGNORECASE)
q = re.compile('\}.*?\{',re.IGNORECASE)
r = re.compile('[label]',re.IGNORECASE)

FILE = [open('Syntax/PopulationSyntax.tex', 'r'),
        open('Syntax/EstimationSyntax.tex', 'r'),
        open('Syntax/ObservationSyntax.tex', 'r'),
        open('Syntax/ReportSyntax.tex', 'r'),
        open('Syntax/GeneralSyntax.tex', 'r')]
OUTFILE = open('QuickReference.tex', 'w')
VERSION = open('SPMversion.tex', 'r')
ERROR_OUTFILE = open('syntax_errors.txt', 'w')

OUTFILE.write("\section{Quick reference\label{sec:quick-reference}}\n")

Keywords1 = []
Keywords2 = []
Keywords3 = ["F","False","T","True","t","f","true","false"]
Keywords4 = []
Keywords5 = []
Keywords6 = []

for i in range(len(FILE)):
  count=0
  for line in FILE[i]:
    # strip white space at start & end
    line = line.strip()
    # if a line has a label, then delete it
    find = line.find('\label')
    # if line if not blank, add a closing bracket
    if line != "":
      line = line[0:find] + "}"
    # recode subsections, subsubsection to be excluded from the contents
    if line[1:14] == "subsubsection" :
      mat = line.find(']')+1
      line = "\\par\\textbf" + line[mat:] + "\\par"
    if line[1:11] == "subsection" :
      #line = "\\subsubsection" + line[11:]
      line = ""
    if line[1:8] == "section" :
      line = "\\subsection" + line[8:]
    # keep line only if it is a section, defCom, defComlab, defComArg, or defSub
    if (line[1:11]=="subsection" or line[1:14]=="subsubsection" or line[1:11]=="par\\textbf" or line[1:7]=="defCom" or line[1:7]=="defSub") :
      if (line[1:11]=="subsection") :
        count=0
        line = line + "\\par"
      if (line[1:7]=="defCom") :
        m = p.search(line)
        Keywords1.append(m.group()[1:(len(m.group())-1)])
        line = line + "\\par"
        if (count!=0):
          line = "\\par" + line
        else:
          count=1
        if(i > 0):
          line = line + "\\par"
      if (line[1:7]=="defSub") :
        m = p.search(line)
        Keywords2.append(m.group()[1:(len(m.group())-1)])
      if (line[1:11]=="par\\textbf") :
        m = q.search(line[::-1])
        n = m.group()[::-1]
        Keywords4.append(n[1:(len(n)-2)])

      OUTFILE.write(line)
      OUTFILE.write('\n')

# Remove duplicates
Keywords1 = list(set(Keywords1))
Keywords2 = list(set(Keywords2))
Keywords3 = list(set(Keywords3))
Keywords4 = list(set(Keywords4))
Keywords5 = list(set(Keywords5))
Keywords6 = list(set(Keywords6))
# Sort
Keywords1.sort()
Keywords2.sort()
Keywords3.sort()
Keywords4.sort()
Keywords5.sort()
Keywords6.sort()
# remove subscript [label]

#Remove '\' in each string
for i in range(len(Keywords1)):
  Keywords1[i] = Keywords1[i].replace("\\", "")
for i in range(len(Keywords2)):
  Keywords2[i] = Keywords2[i].replace("\\", "")
  if(Keywords2[i].count('[label]')):
    Keywords2[i]=Keywords2[i][0:(Keywords2[i].find('[label]')-1)]
for i in range(len(Keywords3)):
  Keywords3[i] = Keywords3[i].replace("\\", "")
for i in range(len(Keywords4)):
  Keywords4[i] = Keywords4[i].replace("\\", "")
for i in range(len(Keywords5)):
  Keywords5[i] = Keywords5[i].replace("\\", "")
for i in range(len(Keywords6)):
  Keywords6[i] = Keywords6[i].replace("\\", "")

# Compare against translations file
r = re.compile('\".*?\"')
TRANSLATION = open('../../Source/Translations/English_UK.h', 'r')
DEFINED = []
for line in TRANSLATION:
  if line[0:13] == "#define PARAM" :
    o = r.search(line)
    DEFINED.append(o.group()[1:(len(o.group())-1)])
DEFINED = list(set(DEFINED))
DEFINED.sort()
# Generate a list of all keywords
KEYWORDS = Keywords1 + Keywords2 + Keywords4
# Make all lower case
for i in range(len(DEFINED)):
  DEFINED[i] = DEFINED[i].lower()
for i in range(len(KEYWORDS)):
  KEYWORDS[i] = KEYWORDS[i].lower()

# In translation file, but not in Manual?
ERROR_OUTFILE.write('\nIn SPM, but not in manual\n')
for i in range(len(DEFINED)):
  if (DEFINED[i] not in KEYWORDS) :
    ERROR_OUTFILE.write("  " + DEFINED[i] + "\n")
ERROR_OUTFILE.write('\n\n\nIn manual, but not in SPM\n')
for i in range(len(KEYWORDS)):
  if (KEYWORDS[i] not in DEFINED) :
    ERROR_OUTFILE.write("  " + KEYWORDS[i] + "\n")
