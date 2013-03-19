# Copyright Daniel Wallin 2006. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

'''
 >>> def verify_series(s, expected):
 ...   for x,y in zip(s, expected):
 ...       assert x == y
 >>> from python_series_ext import *
 >>> s1 = dense_series(discretization = 1)
 >>> len(clip(s1,0,4))
 4
 >>> len(s1[-1:3])
 4
 >>> ordered_inserter(s1,-1)(1)(2)(3)(4).commit()
 >>> verify_series(clip(s1,-1,3), [1,2,3,4])
 >>> ordered_inserter(s1)(1,-1)(4,2).commit()
 >>> verify_series(clip(s1,-1,3), [1,0,0,4])
 >>> ordered_inserter(s1)(1,-1,1)(2,1,3).commit()
 >>> verify_series(clip(s1,-1,3), [1,1,2,2])
 >>> s1 = dense_series()
 >>> s2 = dense_series()
 >>> ordered_inserter(s1)(3,-1)(4,2).commit()
 >>> ordered_inserter(s2)(4,2)(5,3).commit()
 >>> s3 = s1 + s2
 >>> verify_series(s3[-1:4], [3,0,0,8,5])
 >>> s4 = dense_series(-1,3,42,1,0)
 >>> verify_series(s4[-2:4], [0,42,42,42,42,0])
 >>> s5 = sparse_series()
 >>> ordered_inserter(s5)(-42,-inf,0)(42,0,inf).commit()
 >>> s5
 (1, 0, [(-42, -inf, 0), (42, 0, inf)])
 >>> s5[-inf:-1]
 (1, 0, [(-42, -inf, -1)])
 >>> s5[1:inf]
 (1, 0, [(42, 1, inf)])
'''
def run(args = None):
    import sys
    import doctest

    if args is not None:
        sys.argv = args
    return doctest.testmod(sys.modules.get(__name__))
    
if __name__ == '__main__':
    print "running..."
    import sys
    status = run()[0]
    if (status == 0): print "Done."
    sys.exit(status)

