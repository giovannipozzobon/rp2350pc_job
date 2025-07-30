# Proposed External API

The proposed API is based on the ideas of FORTH. 

There is a 32 bit constant circular stack, probably 32 deep. Parameters are pushed on this stack, then commands are executed, which remove the parameters and process the commands, then push any results back on the stack.

The circularity stops crashes if it doesn't work.

Strings are pushed in packed 4 byte chunks.

It allows seperation of code and API functionality, so some could potentially be offloaded to core 1.

Both decoding and API constants are generated automatically.