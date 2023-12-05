def MemDump(Title, data, size = 16):
    from string import printable as PrintableString
    printable = bytes(PrintableString, 'ascii').replace(b'\r', b'').replace(b'\n', b'') \
                                               .replace(b'\t', b'').replace(b'\x0b', b'') \
                                               .replace(b'\x0c', b'')

    if( isinstance(data, bytes) == False ):
        raise TypeError("Expected bytes for data")

    print("{} [{}]:".format(Title, len(data)))
    TheDump = "\n"

    for line in [data[i:i+size] for i in range(0, len(data), size)]:
        
        for c in line:
            try: # python 3
                TheDump += "{:02x} ".format(c) 
            except: # python 2.x
                TheDump += "{:02x} ".format(ord(c))
            
        TheDump += "   " * (size - len(line))
            
        TheDump += ' | '
        for c in line:
            if c in printable:
                TheDump += "{:1c}".format(c)
            else:
                TheDump += '.'
            
        TheDump += "\n"       
    print(TheDump)