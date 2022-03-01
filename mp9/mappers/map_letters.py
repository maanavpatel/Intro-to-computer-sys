# map function:
import re
def map(filename):
  lettercount = {}
  with open(filename, "r", encoding="utf-8") as f:
    lines = f.read()
    
    lines = lines.lower()
    l = re.sub(r'[^a-z\s]+', ' ', lines)
    for c in lines:
      if c >= 'a' and c <= 'z':
        if not c in lettercount.keys():
          lettercount[c] = 1
        else:
          lettercount[c] += 1
  return lettercount