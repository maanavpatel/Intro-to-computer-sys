# reduce function:
from os import major
from sys import float_repr_style


def reduce(left, right):
  print("Reduce: ", left, '\n\t',right)

  if type(left) == list:
    left = left[0]
  if type(right) == list:
    right = right[0]

  mutualrelations = {}
  #find the set intersections
  for l_entry in left:
    L_friends = left[l_entry]
    l1 = l_entry[1]
    l2 = l_entry[3]
    
    if l2 < l1: #alphabetize
      temp  = l2
      l2 = l1
      l1 = temp
    
    for r_entry in right:
      R_friends = right[r_entry]
      r1 = r_entry[1]
      r2 = r_entry[3]

      if r2 < r1: #alphabetize
        temp  = r2
        r2 = r1
        r1 = temp
      
      #find the matching keys to update friends
      if l1 == r1 and l2 == r2:
        mapkey = f'({l1} {l2})'
        if not L_friends and not R_friends: #both have no friends
          continue
        if not R_friends and L_friends: #right has no friends, map entry is just left
          mutualrelations[mapkey] = L_friends
          continue
        if not L_friends and R_friends: #left has no friends, map entry is just right
          mutualrelations[mapkey] = R_friends
          continue

        #both are not empty lets look for mutual friends
        mutuals = []
        for friend in L_friends:
          if friend in R_friends:
            mutuals.append(friend)
        if len(mutuals) > 0 :
          mutualrelations[mapkey] = mutuals
      
      mapkey = f'({r1} {r2})'
      if not mapkey in mutualrelations.keys():
        mutualrelations[mapkey] = R_friends
      
    mapkey = f'({l1} {l2})'
    if not mapkey in mutualrelations.keys():
        mutualrelations[mapkey] = L_friends
    


  print('mUT',mutualrelations)
  return mutualrelations
