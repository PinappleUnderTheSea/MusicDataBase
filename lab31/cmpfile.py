
import hashlib

def md5_file(filename):
    md5 = hashlib.md5()
    with open(filename, 'rb') as f:
        for chunk in iter(lambda: f.read(4096), b''):
            md5.update(chunk)
    return md5.hexdigest()

if __name__ == '__main__':
    std_md5 = md5_file("./stdmatrix.txt")
    matrix_md5 = md5_file("./build/matrix.txt")
    if std_md5 != matrix_md5:
        print("matrix.txt is not the same as stdmatrix.txt")
    else:
        print("Congratulation! matrix.txt is the same as stdmatrix.txt")