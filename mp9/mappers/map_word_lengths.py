import re
# map function:
def map(filename):

  wordlen = {}
  with open(filename, "r", encoding="utf-8") as f:
    lines = f.read()
    lines = lines.lower()

    l = re.sub(r'[^a-z\s]+', ' ', lines)
    
    words = l.split()
    for w in words:
      if len(w) > 0:
        length = len(w)
        if not length in wordlen.keys():
          wordlen[length] = 1
        else:
          wordlen[length] += 1
  return wordlen
