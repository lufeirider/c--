import re

text = """
7c992906
7c862f34
7c921cf9
62c21094
7c922877
7c922877
77d23ad9
"""
m = re.findall("\w\w", text)

print m

SumFour= 0
Str = ""
for i in m:
       SumFour=SumFour+1
       Str =  "\\x"+i + Str
       if 4 == SumFour:
              print Str
              Str=""
              SumFour=0
