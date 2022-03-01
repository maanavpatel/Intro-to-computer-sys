#
# Count the words in the input files.  Deliminate on all non-word characters and count as lower-case.
#
# Example: "data\lincoln-addresses\*.txt"
#   the: 746
#   of: 456
#   to: 366
#   and: 298
#   or: 236
#   in: 217
#   you: 213
#   a: 210
#   gutenberg: 207
#   project: 204
#

import sys
from MapReduce import MapReduce

import mappers.map_words as map_words
import reducers.reduce_sum as reduce_sum

if __name__ == '__main__':
  mr = MapReduce(map_words.map, reduce_sum.reduce)
  mr( sys.argv[1:] )