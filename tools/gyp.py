import sys

sys.path.append('../..')

from bonavista import gyp
gyp.Run(__file__, '../..', '../chaparral.gyp');
