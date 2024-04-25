import random
import string
import time

def generate_word():
    return ''.join(random.choices(string.ascii_lowercase, k=random.randint(1, 10)))

def generate_text_file(file_path, size_gb, block_size=1024*1024):
    # words_needed = size_gb * 1024 * 1024 * 128 # 1 GB ≈ 1024*1024*128 palabras
    words_needed = size_gb * 1024 * 1024  # 1 MB ≈ 1024*1024 palabras
    
    start_time = time.monotonic()
    with open(file_path, 'w') as f:
        while words_needed > 0:
            block = min(words_needed, block_size)
            words = [generate_word() for _ in range(block)]
            f.write(' '.join(words) + ' ')
            words_needed -= block
    end_time = time.monotonic()
    
    print("Tiempo de generación del archivo:", end_time - start_time, "segundos")

# file_path = 'texto_20GB.txt'
file_path = "texto_50MB.txt"
size_gb = 10

generate_text_file(file_path, size_gb)