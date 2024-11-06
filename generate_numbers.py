import random

def generate_random_elements(n, start=0, end=2000):
    """Generates a list of n random elements within a given range."""
    return [random.randint(start, end) for _ in range(n)]

# Example usage
n = 1000  # Number of random elements to generate
random_elements = generate_random_elements(n)
print(random_elements)
