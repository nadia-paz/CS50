import cs50

def main():
    while True:
        print("Height: ", end="")
        height = cs50.get_int()
        if height >= 0 or height < 23:
            break
    
    for i in range(height):
        spaces = height - (i + 1)
        print(" " * spaces, end="")
        print("#" * (i + 1), end="")
        print(" " * 2, end="")
        print("#" * (i + 1), end="")
        print()

if __name__ == "__main__":
    main()