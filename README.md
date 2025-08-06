# usage
```
#include <ParallelBus.h>

ParallelBus8 busname(pins..., -1);
or
ParallelBus16 busname(pins..., -1);
or
ParallelBus<width> busname(pins..., -1);
```
Standard Arduino functions digitalRead, digitalWrite and pinMode are overloaded to use with bus instances and work the way you would expect.
Busses are little endian (first pin specified is the least significant bit).
To create big endian busses, just reverse the order of the pins in the constructor.

# limitations

Max width is 16 bits. This keeps the read/written value compatible with AVR integers.
No attempt is made to optimize speed when pins happen to be in the same port or even in port-pin order.
This keeps the library simple and universal.
