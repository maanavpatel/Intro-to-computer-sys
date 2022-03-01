import re
# map function:
def map(filename):

  wordcount = {}
  with open(filename, "r", encoding="utf-8") as f:
    lines = f.read()
    lines = lines.lower()

    l = re.sub(r'[^a-z\s]+', ' ', lines)
    
    words = l.split()
    for i in range(len(words) - 1):
      first = words[i]
      sec = words[i+1]
      if len(first) > 0 and len(sec) > 0:
        pair = f'({first} {sec})'
        if not pair in wordcount.keys():
          wordcount[pair] = 1
        else:
          wordcount[pair] += 1
  return wordcount
