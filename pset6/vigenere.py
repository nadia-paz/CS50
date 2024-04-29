import sys

def main():
    
    if len(sys.argv) != 2:
        print("Usage phyton vigenere.py keyword")
        exit(1)
    key = sys.argv[1]
    is_word = True if key.isalpha() else False
    if not is_word:
        print("Usage phyton vigenere.py keyword")
        exit(2)
    else:
        print("plaintext: ", end="")
        plaintext = input()
        print("ciphertext: {}".format(cipher(plaintext, key)))

def cipher(text, key):
    textlen = len(text)
    keylen = len(key)
    ctext = []
    #keys = list(keylen)
    keys = []
    
    for i in range(keylen):
        ch = key[i]
        if ch.isupper():
            keys.append(ord(ch) - 65)
        if ch.islower():
            keys.append(ord(ch) - 97)
    
    j = 0
    for i in range(textlen):
        c = text[i]
        ckey = keys[j]
        control = ord(c) + ckey
        if c.isalpha():
            if c.isupper():
                if control > ord('Z'):
                    c = chr(control - 26)
                else:
                    c = chr(control)
            if c.islower():
                if control > ord('z'):
                    c = chr(control - 26)
                else:
                    c = chr(control)
            j += 1
            if j >= keylen:
                j = 0
        
        ctext.append(c)
        
            
        
    return "".join(ctext)
    
    
if __name__ == "__main__":
    main()