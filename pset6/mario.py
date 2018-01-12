def main():
    while True:
        n = input("Enter height: ")
        try:
            n = int(n);
        except ValueError:
            continue
        if n in range(24):
            break

    for i in range(n):
        #spaces
        print(" " * (n - 1 - i), end = "")
        # hashes
        print("#" * (i + 1), end = "")
        print("  ", end = "")
        print("#" * (i + 1), end = "")
        print()


if __name__ == "__main__":
    main()