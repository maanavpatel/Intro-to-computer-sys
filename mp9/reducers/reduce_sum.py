# reduce function:
def reduce(left, right):
  for k in left.keys():
    if k in right.keys():
      right[k] += left[k]
    else:
      right[k] = left[k]
  return right
