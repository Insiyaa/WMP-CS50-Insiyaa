def main():
    card = list(map(int,str(input("Enter: "))))
    L = list(map(lambda x: list(map(int, str(x))), list(map(lambda x : 2 * x, card[1::2]))))
    total = sum(card[0::2]) + sum( sum(x) if isinstance(x, list) else x for x in L )
    if total%10 == 0:
        print("Valid")
        if card[:2] in [[3, 4],[3, 7]]:
            print("AMEX")
        elif card[:2] in [[5,1], [5,2], [5,3], [5,4], [5,5]]:
            print("MasterCard")
        elif card[0] == 4:
            print("VISA")
        else:
            print("Other")
    else:
        print("Invalid")


if __name__ == "__main__":
    main()