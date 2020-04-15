import os

def save_strings(data, filepath):
    dir = os.path.dirname(filepath)
    if not os.path.exists(dir):
        os.makedirs(dir)

    # write file in binary mode
    with open(filepath, 'wb') as output:
        output.write(data)
