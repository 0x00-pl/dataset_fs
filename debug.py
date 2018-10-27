import random

import numpy as np


def test():
    import numpy as np
    from build.src import libcpython_test_lib
    fw = libcpython_test_lib.fwarp('/data/rand_file')
    for item in [46745, 439598, 656019, 197860, 611032, 66026, 571133, 263861, 750976, 976852]:
        print(fw.read_int8(item, 1024))

    print(libcpython_test_lib.return_100_zeros())
    print(fw.read_int8(1, 1024))
    print(fw.read_int8(2, 1024))
    print(fw.read_int16(1, 30))


def gen_rands(start, end, count):
    return [random.randrange(start, end) for i in range(count)]


def test1(rand_list):
    """test c"""
    import numpy as np
    from build.src import libcpython_test_lib
    fw = libcpython_test_lib.fwarp('rand_file')
    for item in rand_list:
        data = fw.read_int8(item, 1024)


def test2(rand_list):
    """test py"""
    import numpy as np
    with open('rand_file', 'rb') as f:
        for item in rand_list:
            f.seek(item)
            data = np.array(f.read(1024))


if __name__ == '__main__':
    rand_list = gen_rands(0, 1024 * 1024 * 1000, 1000000)
    test1(rand_list)
    rand_list = gen_rands(0, 1024 * 1024 * 1000, 1000000)
    test2(rand_list)
