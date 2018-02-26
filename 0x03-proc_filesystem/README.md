## Process Heap Overwriter

This nifty little program allows us to overwrite any string that exists in the heap with one of our own.
Keep in mind, if you plan on overwriting with a string larger than the previous string, you could be overwriting memory for other variables, and that can lead to unintended consequences. Use responsibly!

Usage:

./read_write_heap.py PID "Search String" "Replacement String"

This program also ends every replacement string with a \0 byte, so you replace the old string with a smaller string, the variable holding that string will also end at the new string's location.