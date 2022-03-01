import re
# map function:
def map(filename):

  friendmap = {}
  with open(filename, "r", encoding="utf-8") as f:
    # lines = f.read()
    
    # x = lines.split('\n')

    # for entry in x:
    #   #map each node individually
    #   a = entry.split('->')
    #   me = a[0]
    #   friends = a[1].split(',')
    #   for f in friends:
    #     friendmap.append({me:[f]})
    # # me = x[0]
    # # friends = x[1]

    # # friends = friends.split(',')
    # # friendmap[me] = friends

    # # for f in friends:
    # #   friendmap[f] = [me]

    # print(friendmap)

    lines = f.read()
    
    nodes = lines.split('\n')
    
    #A->B,C,D = (A B): C, D, (A C): B,D ...

    for node in nodes:
      print('NODE:', node)
      nodeinfo = node.split('->')
      friends = nodeinfo[1]
      friendlist = friends.split(',')
      for friend in friendlist:
        topair = friend
        me = nodeinfo[0]
        key = f'({me} {topair})'
        mapentry = []
        for fr in friendlist:
          if fr != topair:
            mapentry.append(fr)
        friendmap[key] = mapentry
    print('->', friendmap)
    return friendmap
