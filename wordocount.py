import time


def word_count(file_path):
    word_count = 0
    with open(file_path, 'r') as file:
        for line in file:
            words = line.split()
            word_count += len(words)
    return word_count

# Example usage:
file_path = 'texto_20GB.txt'
start_time = time.monotonic()
print(f"Word count: {word_count(file_path)}")
end_time = time.monotonic()
print("Tiempo de conteo:", end_time - start_time, "segundos")