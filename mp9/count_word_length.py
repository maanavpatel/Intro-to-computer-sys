#
# Count the length of words in the input files.  Deliminate on all non-word characters.
#
# Example: "data/taylor/*.txt"
#  4: 140
#  3: 123
#  2: 112
#  1: 80


import sys
from MapReduce import MapReduce

import mappers.map_word_lengths as map_word_lengths
import reducers.reduce_sum as reduce_sum

if __name__ == '__main__':
  mr = MapReduce(map_word_lengths.map, reduce_sum.reduce)
  mr( sys.argv[1:] )