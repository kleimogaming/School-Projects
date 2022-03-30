'''

Made by: Nicholas Klein
Section: TuTh 10:00
Project 1a

'''
import random

# Returns true if prime, false if composite
# Pseudocode provided in textbook
def primality(a, n):
    r = n - 1
    t = 0
    if pow(a, n - 1, n) != 1:
        return False
    
    while (r % 2 == 0):
        r = r // 2
        t = t + 1
    b = pow(a, r, n)
    for i in range(1, t + 1):
        c = pow(b, 2, n)
        if c == 1 and b != 1 and b != n - 1:
            return False
        b = c
    return True

# Returns true if prime, false if composite
# Pseudocode provided in textbook
def millerRabin(n, s):
    for j in range(1, s + 1):
        a = random.randint(1, n - 1)
        if primality(a, n) == False:
            return False
    return True

# Calculates the gcd, integer coefficients for d = gcd(a, b) = ax + by
# Pseudocode provided in textbook
def extendedEuclidean(a, b):
    if b == 0:
        return (a, 1, 0)
    else:
        (dprime, xprime, yprime) = extendedEuclidean(b, a % b)
        (d, x, y) = (dprime, yprime, (xprime - (yprime * (a // b))))
        return (d, x, y)


# Main function used for testing
def main():
    # Generate two random integers based off the given modulus bitsize
    size = int(input("Enter number of bits for modulus: "))
    p = random.randint(2, pow(2, int(size/2) - 1))
    q = random.randint(2, pow(2, int(size/2) - 1))
    
    # Make sure p and q are odd numbers
    if p % 2 == 0:
        p = p + 1
    if q % 2 == 0:
        q = q + 1
    
    # Increment p and q to higher odd numbers until they pass the primality test
    # Pass error of 20 to Miller-Rabin test, should be sufficient for testing purposes
    while millerRabin(p, 20) != True:
        p = p + 2
    while millerRabin(q, 20) != True:
        q = q + 2
        
    # Calculate the other important values
    n = p * q
    e = pow(2, 16) + 1
    phi = (p - 1) * (q - 1)

    # Compute the gcd and integer coefficients
    (gcd, x, y) = extendedEuclidean(e, phi)
    
    # gcd should be 1 if prime
    print("GCD should be 1: ", gcd)
    d = x % phi
    
    # Encrypt the message
    M = "I deserve an A"
    x = 0

    # Provided by project documentation
    for c in M:
        x = x << 8
        x = x ^ ord(c)
        
    # d can't be negative
    if d < 0:
        d = d % phi
    
    encrypted = pow(x, e, n)
    decrypted = pow(encrypted, d, n)
    
    # Print all the necessary information to the console
    print("Public Key (e, n): ")
    print(e, ", ", n)
    
    print("Secret Key (d): ")
    print(d)
    
    print(M)
    print("Signature using d: ", x)
    print("Encrypted: ", encrypted)
    print("Decrypted: ", decrypted)
    
    # Visual aid to see if algorithm worked properly
    if (x == decrypted):
        print("M = M' ")
    else:
        print("failed")
   
main()
