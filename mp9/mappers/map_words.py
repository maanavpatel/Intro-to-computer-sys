import re
# map function:
def map(filename):

  wordcount = {}
  with open(filename, "r", encoding="utf-8") as f:
    lines = f.read()
    lines = lines.lower()

    l = re.sub(r'[^a-z\s]+', ' ', lines)
    
    words = l.split()
    for w in words:
      if len(w) > 0:
        if not w in wordcount.keys():
          wordcount[w] = 1
        else:
          wordcount[w] += 1
  return wordcount
