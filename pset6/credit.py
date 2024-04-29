import cs50

def main():
    while True:
        print("Number of CC: ", end="")
        cc = cs50.get_int()
        if type(cc) == int:
            break
    validation(cc)
    
# returns the # of didits if CC is 13, 15 or 16 digits long
# else returns -1
def check_length(number):
    length = len(str(number))
    if length == 14:
        return -1
    elif length < 13 or length > 16:
        return -1
    else:
        return length
        

def checksum(number):
    length = check_length(number)
    if length == -1:
        return False
    sum1 = 0
    sum2 = 0
    if length % 2 == 1:
        index1 = 1
        index2 = 0
    else:
        index1 = 0
        index2 = 1
    
    while(index1 < length):
        digit1 = int(str(number)[index1]) * 2
        if digit1 > 9:
            digit1 = digit1 % 10 + 1
        sum1 += digit1
        index1 += 2
    
    while(index2 <= length):
        digit2 = int(str(number)[index2])
        sum2 += digit2
        index2 += 2
    
    if (sum1 + sum2) % 10 == 0:
        return True
    else:
        return False

def validation(number):
    visa = int(str(number)[0])
    am_master = visa * 10 + int(str(number)[1])
    length = check_length(number)
    if checksum(number) or not length == -1:
        if visa == 4 and (length == 13 or length == 16):
            print("VISA")
        elif (am_master == 34 or am_master == 37) and length == 15:
            print("AMEX")
        elif (am_master >= 51 and am_master <= 55) and length == 16:
            print("MASTERCARD")
        else:
            print("INVALID")
        
    else:
        print("INVALID")
    
if __name__ == "__main__":
    main()