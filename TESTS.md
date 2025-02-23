## Format: input | expected output


### General tests

246 | 246 -> two dollars and forty six cents

50 | 50 -> zero dollars and fifty cents

12345 | 12345 -> one hundred twenty three dollars and forty five cents

999 | 999 -> nine dollars and ninety nine cents

10101010 | 10101010 -> one hundred one thousand ten dollars and ten cents


### Test that negatives return same words as above numbers (besides the "minus")

-123 | -123 -> minus one dollar and twenty three cents

-45 | -45 -> minus zero dollars and forty five cents

-999 | -999 -> minus nine dollars and ninety nine cents

-10101010 | -10101010 -> minus one hundred one thousand ten dollars and ten cents


### Test that -0 is same as 0

0 | 0 -> zero dollars and zero cents

-0 | 0 -> zero dollars and zero cents


### Test that "dollar" and "cent" are singular

1 | 1 -> zero dollars and one cent

-1 | -1 -> minus zero dollars and one cent

100 | 100 -> one dollar and zero cents

-100 | -100 -> minus one dollar and zero cents

101 | 101 -> one dollar and one cent

-101 | -101 -> minus one dollar and one cent


### Test if leading zeros are ignored

00055 | 55 -> zero dollars and fifty five cents

55 | 55 -> zero dollars and fifty five cents

-012 | -12 -> minus zero dollars and twelve cents

-12 | -12 -> minus zero dollars and twelve cents

000000 | 0 -> zero dollars and zero cents


### Test highest and lowest integer supported

9223372036854775807 | 9223372036854775807 -> ninety two quadrillion two hundred thirty three trillion seven hundred twenty billion three hundred sixty eight million five hundred forty seven thousand seven hundred fifty eight dollars and seven cents

-9223372036854775807 | -9223372036854775807 -> minus ninety two quadrillion two hundred thirty three trillion seven hundred twenty billion three hundred sixty eight million five hundred forty seven thousand seven hundred fifty eight dollars and seven cents


# Tests for errors


### This fits within 64 bits, but it cannot be converted to a positive value
### Since conversion internally uses absolute value, this means program should return an out of range error (code 2)

-9223372036854775808 | Received error code 2 with an input of '-9223372036854775808'


### Out of range

9999999999999999999999999999 | Received error code 2 with an input of '9999999999999999999999999999'

-10000000000000000000 | Received error code 2 with an input of '-10000000000000000000'


### Non-digits, extra numbers

hello | Received error code 1 with an input of 'hello'

98765432 one | Received error code 1 with an input of '98765432 one'

52! | Received error code 1 with an input of '52!'

1, 2, 4, 8 | Received error code 1 with an input of '1, 2, 4, 8'

https://github.com/ | Received error code 1 with an input of 'https://github.com/'

./run_virus | Received error code 1 with an input of './run_virus'


### Test how robust the leading 0's and negative num detection is

0-876 | Received error code 1 with an input of '0-876'

--351 | Received error code 1 with an input of '--351'

- | Received error code 1 with an input of '-'

-0- | Received error code 1 with an input of '-0-'


### Test input larger than buffer size (100)
### Testing if fgets() truncates extra characters, and buffer is cleared to not be read in future inputs

000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001234567890 | 1234 -> twelve dollars and thirty four cents

99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999991234567890 | Received error code 2 with an input of '999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999912345678'
