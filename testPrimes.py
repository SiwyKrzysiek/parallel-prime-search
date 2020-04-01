import sympy

n = 10000000
file_name = 'primes.txt'

actual_primes = set(sympy.sieve.primerange(1, n+1))

with open(file_name, 'r') as file:
    my_primes = {int(line) for line in file.readlines()}

missing_primes = actual_primes - my_primes
aditional_numbers = my_primes - actual_primes

print(f'Missing primes number: {len(missing_primes)}')
# print(missing_primes)
print(f'Additional numbers number: {len(aditional_numbers)}')
# print(aditional_numbers)
