import sys
import string
import crypt
if len(sys.argv) != 2:
    print("Usage: python crack.py hashedPassword")
    exit(1)
salt = '50'
hash = sys.argv[1]
letters = string.ascii_letters

word = ""

L = len(letters)

for i in range(L):
    for j in range(L):
        for k in range(L):
            for m in range(L):
                if crypt.crypt(word,salt) == hash:
                    print(word)
                    exit(0)
                word = letters[m] + word[1:]
            word = word[:1] + letters[k] + word[2:]
        word = word[:2] + letters[j] + word[3:]
    word = word[:3] + letters[i] + word[4:]
