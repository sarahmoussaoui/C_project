import random

def generate_random_elements(n, start=1, end=100):
    """Generates a string of n random elements separated by spaces within a given range."""
    random_elements = [str(random.randint(start, end)) for _ in range(n)]
    return " ".join(random_elements)

# Example usage
n = 10  # Number of random elements to generate
random_elements_with_spaces = generate_random_elements(n)
print(random_elements_with_spaces)
