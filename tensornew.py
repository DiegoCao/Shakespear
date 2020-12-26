import collections
import functools
import os
import time

import numpy as np
import tensorflow as tf
import tensorflow_federated as tff

np.random.seed(0)

# Test the TFF is working:
tff.federated_computation(lambda: 'Hello, World!')()