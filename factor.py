'''

Made by: Nicholas Klein
Section: TuTh 10:00
Project Part 2

'''

import random

# Implementation of Pollard Rho based on pseudocode from textbook
def pollardRho(n):

    # Generate variables used later in algorithm 
    x = random.randint(0, n - 1)
    y = x
    d = 1

    # As long as the gcd is still 1
    while d == 1:

        x = pow(x, 2) - 1
        z = pow(y, 2) - 1
        
        x = x % n
        z = z % n
        
        y = pow(z, 2) - 1
        y = y % n
        
        arg = x - y

        # Argument to gcd function can't be negative, so if it is, make it positive
        if arg < 0:
            arg = 0 - arg

        # Try to find a factor based on the arguments provided in this iteration
        d = euclid(arg, n)

    # If a factor was found, return it
    if d != n:
        return(d)


# Basic function to find gcd of two numbers
def euclid(a, b):
    if b == 0:
        return a
    else:
        return euclid(b, a % b)

# Main function to find prime factors of a given input
def main():
    n = int(input("Enter modulus: "))
    p = pollardRho(n)
    q = n // p
    
    print("Modulus: ", (p * q))
    print("Factor P: ", p)
    print("Factor Q: ", q)

    
main()
